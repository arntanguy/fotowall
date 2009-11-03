/***************************************************************************
 *                                                                         *
 *   This file is part of the Fotowall project,                            *
 *       http://www.enricoros.com/opensource/fotowall                      *
 *                                                                         *
 *   Copyright (C) 2009 by TANGUY Arnaud <arn.tanguy@gmail.com>            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "FotowallFile.h"

#include "Canvas/Canvas.h"
#include "App.h"
#include "Settings.h"

#include <QCoreApplication>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QString>
#include <QTextStream>

/** \brief XML File structure

  Old (0.4 to 0.8.96)
    <fotowall>
      <project>
        <mode />
      </project>
      <desk />
      <content>
        <type .. />
        ...
        <type .. />
      </content>
    </fotowall>

  Since Fotowall 0.8.97
    <fotowall format="2" version="0.8.97">
      <canvas>
        <mode />
        <background />
        <content />
      </canvas>
    </fotowall>

**/

bool FotowallFile::read(const QString & filePath, Canvas * canvas, bool inHistory)
{
    // open the file for reading
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(0, QObject::tr("Loading error"), QObject::tr("Unable to load the Fotowall file %1").arg(filePath));
        return false;
    }

    // load the DOM
    QString error;
    QDomDocument doc;
    if (!doc.setContent(&file, false, &error)) {
        QMessageBox::critical(0, QObject::tr("Parsing error"), QObject::tr("Unable to parse the Fotowall file %1. The error was: %2").arg(filePath, error));
        return false;
    }
    file.close();

    // get the Canvas node
    QDomElement root = doc.documentElement();
    QDomElement canvasElement = root.firstChildElement("canvas");
    if (!canvasElement.isElement()) // 'Format 1'
        canvasElement = root;
    if (!canvasElement.isElement())
        return false;

    // restore the canvas
    canvas->fromXml(canvasElement);

    // add to the recent history
    if (inHistory)
        App::settings->addRecentFotowallUrl(QUrl(filePath));
    return true;
}

bool FotowallFile::saveV2(const QString & filePath, const Canvas * canvas)
{
    // create the document
    QDomDocument doc;
     doc.appendChild(doc.createProcessingInstruction("xml","version=\"1.0\" "));

    QDomElement rootElement = doc.createElement("fotowall");
     rootElement.setAttribute("format", 2);
     rootElement.setAttribute("version", QCoreApplication::applicationVersion());
     doc.appendChild(rootElement);

    QDomElement canvasElement = doc.createElement("canvas");
     rootElement.appendChild(canvasElement);

    // save current canvas
    canvas->toXml(canvasElement);

    // open the file for writing
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(0, QObject::tr("File Error"), QObject::tr("Error saving to the Fotowall file '%1'").arg(filePath));
        return false;
    }

    // save in the file (2 indent spaces)
    QTextStream outStream(&file);
    doc.save(outStream, 2);
    file.close();

    // store a reference to the just written file
    App::settings->addRecentFotowallUrl(QUrl(filePath));
    return true;
}

QString FotowallFile::getLoadFotowallFile()
{
    // make up the default load path (stored as 'Fotowall/LoadProjectDir')
    QString defaultLoadPath = App::settings->value("Fotowall/LoadProjectDir").toString();

    // ask the 'load' file name
    QString fileName = QFileDialog::getOpenFileName(0, QObject::tr("Select the Fotowall file"), defaultLoadPath, QObject::tr("Fotowall (*.fotowall)"));
    if (fileName.isNull())
        return QString();

    // store the load path to settings
    App::settings->setValue("Fotowall/LoadProjectDir", QFileInfo(fileName).absolutePath());
    return fileName;
}

QStringList FotowallFile::getLoadFotowallFiles()
{
    // make up the default load path (stored as 'Fotowall/LoadProjectDir')
    QString defaultLoadPath = App::settings->value("Fotowall/LoadProjectDir").toString();

    // ask the 'load' file name
    QStringList fileNames = QFileDialog::getOpenFileNames(0, QObject::tr("Select one or more Fotowall files to add"), defaultLoadPath, QObject::tr("Fotowall (*.fotowall)"));
    if (fileNames.isEmpty())
        return QStringList();

    // store the load path to settings
    App::settings->setValue("Fotowall/LoadProjectDir", QFileInfo(fileNames[0]).absolutePath());
    return fileNames;
}

QString FotowallFile::getSaveFotowallFile()
{
    // make up the default save path
    QString defaultSavePath = QObject::tr("Unnamed %1.fotowall").arg(QDate::currentDate().toString());
    if (App::settings->contains("Fotowall/SaveProjectDir"))
        defaultSavePath.prepend(App::settings->value("Fotowall/SaveProjectDir").toString() + QDir::separator());

    // ask the 'save' file name
    QString fileName = QFileDialog::getSaveFileName(0, QObject::tr("Select the Fotowall file"), defaultSavePath, "Fotowall (*.fotowall)");
    if (fileName.isNull())
        return QString();

    // store the save path to settings
    App::settings->setValue("Fotowall/SaveProjectDir", QFileInfo(fileName).absolutePath());

    // add extension, if missing
    if (!fileName.endsWith(".fotowall", Qt::CaseInsensitive))
        fileName += ".fotowall";
    return fileName;
}
