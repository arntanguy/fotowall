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

#ifndef __BezierCubicItem_h__
#define __BezierCubicItem_h__

#include <QGraphicsItem>
#include <QVector>
class BezierControlLine;
class BezierControlPoint;
class BezierCubicPath;

class BezierCubicItem : public QGraphicsItem
{
    public:
        BezierCubicItem(QGraphicsItem * parent = 0);

        // ::QGraphicsItem
        QRectF boundingRect() const;
        void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0);

        // called by control points
        void controlPointMoved(int index);

    private:
        QVector<BezierControlPoint *> m_cps;
        BezierControlLine * m_l1;
        BezierControlLine * m_l2;
        BezierCubicPath * m_path;
};

#endif