/***************************************************************************
 *                                                                         *
 *   This file is part of the Fotowall project,                            *
 *       http://www.enricoros.com/opensource/fotowall                      *
 *                                                                         *
 *   Copyright (C) 2009 by Enrico Ros <enrico.ros@gmail.com>               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "SelectionProperties.h"

#include "PictureContent.h"
#include "TextContent.h"
#include "WebcamContent.h"

#include <QLabel>
#include <QPushButton>
#include <QToolButton>
#include <QGridLayout>

#define ADD_BUTTON(Object, Name, Slot) \
    QToolButton * Object = new QToolButton(this); \
    Object->setText(Name); \
    connect(Object, SIGNAL(clicked()), this, Slot);

SelectionProperties::SelectionProperties(QList<AbstractContent *> selection, QWidget * parent)
  : QWidget(parent)
{
    // customize box appearance
    setWindowTitle(tr("SELECTION"));

    // gather info on the selection
    m_pictures = projectList<AbstractContent, PictureContent>(selection);
    m_texts = projectList<AbstractContent, TextContent>(selection);
    bool allPictures = m_pictures.size() == selection.size();

    // create the controls
    QLabel * label;
    if (allPictures)
        label = new QLabel(tr("%1 pictures selected").arg(selection.size()), this);
    else
        label = new QLabel(tr("%1 objects selected").arg(selection.size()), this);
    QGridLayout * lay = new QGridLayout(this);
    lay->setMargin(0);
    lay->addWidget(label, 0, 0, 1, 2);
    ADD_BUTTON(deleteButton, tr("Delete"), SIGNAL(deleteSelection()));
    lay->addWidget(deleteButton, 1, 0);

    // pure pictures
    if (allPictures) {
        // TODO emit some signal or call the Collation manager?
        ADD_BUTTON(button, tr("Collate"), SIGNAL(collateSelection()));
        button->setEnabled(false);
        lay->addWidget(button, 1, 1);
    }
}

SelectionProperties::~SelectionProperties()
{
}
