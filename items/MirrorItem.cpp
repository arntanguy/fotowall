/***************************************************************************
 *                                                                         *
 *   This file is part of the FotoWall project,                            *
 *       http://code.google.com/p/fotowall                                 *
 *                                                                         *
 *   Copyright (C) 2009 by Enrico Ros <enrico.ros@gmail.com>               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "MirrorItem.h"
#include <QGraphicsScene>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <math.h>

#define MIRROR_HEIGHT 100

MirrorItem::MirrorItem(QGraphicsItem * sourceItem, QGraphicsItem * parent)
    : QGraphicsItem(parent)
    , m_source(sourceItem)
{
    // read current values
    sourceUpdated();
    setVisible(m_source->isVisible());

    // add to the scame scene
    Q_ASSERT(m_source->scene());
    m_source->scene()->addItem(this);
}

MirrorItem::~MirrorItem()
{
}

QRectF MirrorItem::boundingRect() const
{
    return m_boundingRect;
}

void MirrorItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * /*widget*/)
{
    // generate Reflection pixmap, if needed
    if (m_pixmap.isNull()) {

        // start with a blank pixmap
        m_pixmap = QPixmap(m_boundingRect.width(), m_boundingRect.height());
        m_pixmap.fill(Qt::transparent);

        // find out the Transform chain to mirror a rotated item
        QRectF sceneRectF = m_source->mapToScene(m_source->boundingRect()).boundingRect();
        QTransform tFromItem = m_source->transform() * QTransform(1, 0, 0, 1, m_source->pos().x(), m_source->pos().y());
        QTransform tFromPixmap = QTransform(1, 0, 0, -1.0, sceneRectF.left(), sceneRectF.bottom());
        QTransform tItemToPixmap = tFromItem * tFromPixmap.inverted();

        // draw the transformed item onto the pixmap
        QPainter p(&m_pixmap);
        p.setRenderHint(QPainter::Antialiasing, true);
        p.setTransform(tItemToPixmap, true);
        m_source->paint(&p, 0, 0);
        p.end();

        // add a linear alpha channel to the image
        QPixmap alphaPixmap(m_pixmap.size());
        QPainter alphaPainter(&alphaPixmap);
        QLinearGradient alphaGradient(0, 0, 0, alphaPixmap.height());
        alphaGradient.setColorAt(0.0 , QColor(128, 128, 128));
        alphaGradient.setColorAt(0.50, QColor( 32,  32,  32));
        alphaGradient.setColorAt(1.0 , QColor(  0,   0,   0));
        alphaPainter.fillRect(alphaPixmap.rect(), alphaGradient);
        alphaPainter.end();
        m_pixmap.setAlphaChannel(alphaPixmap);
    }

    // draw the reflection pixmap
    if (!option)
        painter->drawPixmap(0, 0, m_pixmap);
    else
        painter->drawPixmap(option->rect, m_pixmap, option->rect);
}

void MirrorItem::sourceUpdated()
{
    // find out the item's polygon in scene coordinates
    QRectF itemRect = m_source->boundingRect();
    QPolygonF itemScenePolygon = m_source->mapToScene(itemRect);

    // resize and reposition as an axis-aligned rect on the bottom of the Picture
    QRect sceneRect = itemScenePolygon.boundingRect().toRect();
#if 1
    setPos(sceneRect.bottomLeft());
#else
    setPos(sceneRect.bottomLeft() + QPoint(0, -5));
#endif
    prepareGeometryChange();
    m_boundingRect = sceneRect.translated(-sceneRect.topLeft());
    if (m_boundingRect.height() > MIRROR_HEIGHT)
        m_boundingRect.setHeight(MIRROR_HEIGHT);

    // ### updated directly from caller
    //setVisible(m_source->isVisible());
    //setZValue(m_source->zValue() - 0.1);

    // invalidate current rendering
    m_pixmap = QPixmap();
    update();
}