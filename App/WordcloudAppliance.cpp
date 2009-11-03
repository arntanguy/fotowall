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

#include "WordcloudAppliance.h"

#include "Wordcloud/WordItem.h"

#include <QPushButton>

WordcloudScene::WordcloudScene(Wordcloud::Cloud * cloud, QObject * parent)
  : AbstractScene(parent)
  , m_cloud(cloud)
{
    m_cloud->setScene(this);
    foreach (QGraphicsItem * item, items()) {
        Wordcloud::WordItem * word = dynamic_cast<Wordcloud::WordItem *>(item);
        if (word)
            connect(word, SIGNAL(moved()), this, SLOT(slotWordMoved()), Qt::QueuedConnection);
    }
}

void WordcloudScene::resize(const QSize & size)
{
    QRectF itemsRect = itemsBoundingRect();
    QSize newSize = itemsRect.size().toSize().expandedTo(size);

    AbstractScene::resize(newSize);

    QPointF delta = sceneCenter() - itemsRect.center();
    if (!delta.isNull()) {
        foreach (QGraphicsItem * item, items())
            item->setPos(item->pos() + delta);
    }
}

void WordcloudScene::slotWordMoved()
{
    adjustSceneSize();
}

WordcloudAppliance::WordcloudAppliance(Wordcloud::Cloud * extCloud, QObject * parent)
  : QObject(parent)
  , m_extCloud(extCloud)
  , m_scene(new WordcloudScene(extCloud))
  , ui(new Ui::WordcloudApplianceElements)
  , m_dummyWidget(new QWidget)
  , m_sidebar(0)
{
    // init UI
    ui->setupUi(m_dummyWidget);

    // set the target scene of the cloud to this
    m_sidebar = new QWidget();
    m_sidebar->setAutoFillBackground(true);
    QPalette pal;
    pal.setBrush(QPalette::Window, Qt::lightGray);
    m_sidebar->setPalette(pal);
    QVBoxLayout * lay = new QVBoxLayout(m_sidebar);

    QPushButton * btn1 = new QPushButton(tr("Regen"), m_sidebar);
    connect(btn1, SIGNAL(clicked()), this, SLOT(slotRegenCloud()));
    lay->addWidget(btn1);

    QPushButton * btn2 = new QPushButton(tr("Randomize"), m_sidebar);
    connect(btn2, SIGNAL(clicked()), this, SLOT(slotRandomizeCloud()));
    lay->addWidget(btn2);

    lay->addStretch(10);

    // configure the appliance
    sceneSet(m_scene);
    sidebarSetWidget(m_sidebar);
}

WordcloudAppliance::~WordcloudAppliance()
{
    if (m_extCloud) {
        qWarning("WordcloudAppliance::~WordcloudAppliance: cloud's still here.. take it!");
        m_extCloud->setScene(0);
    }
    delete m_sidebar;
    delete m_scene;
    delete m_dummyWidget;
    delete ui;
}

Wordcloud::Cloud * WordcloudAppliance::takeCloud()
{
    Wordcloud::Cloud * cloud = m_extCloud;
    m_extCloud = 0;
    cloud->setScene(0);
    sceneClear();
    return cloud;
}

Wordcloud::Cloud * WordcloudAppliance::cloud() const
{
    return m_extCloud;
}

void WordcloudAppliance::saveToFile(const QString &__fileName)
{
    if (__fileName.isEmpty()) {
        // ###
        qWarning("WordcloudAppliance::saveToFile: not implemented");
    }
    qWarning("WordcloudAppliance::saveToFile: not implemented");
}

void WordcloudAppliance::slotRegenCloud()
{
    m_extCloud->regenCloud();
}

void WordcloudAppliance::slotRandomizeCloud()
{
    m_extCloud->randomCloud();
}
