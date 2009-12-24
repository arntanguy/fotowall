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

#ifndef __PixmapButton_h__
#define __PixmapButton_h__

#include <QAbstractButton>
#include <QPixmap>
#include <QFont>

class PixmapButton : public QAbstractButton
{
    Q_OBJECT
    Q_PROPERTY(QSize fixedSize READ fixedSize WRITE setFixedSize)
    Q_PROPERTY(QPixmap hoverPixmap READ hoverPixmap WRITE setHoverPixmap)
    Q_PROPERTY(QPixmap pixmap READ pixmap WRITE setPixmap)
    Q_PROPERTY(bool fadeInactive READ fadeInactive WRITE setFadeInactive)
    public:
        PixmapButton(QWidget * parent = 0);

        // change pixmap
        void setPixmap(const QPixmap & pixmap);
        QPixmap pixmap() const;

        // hover text (accelerator like)
        void setHoverText(const QString & text);
        QString hoverText() const;

        // change mouse-over pixmap
        void setHoverPixmap(const QPixmap & pixmap);
        QPixmap hoverPixmap() const;

        // fade out if non-hovered/focused
        void setFadeInactive(bool);
        bool fadeInactive() const;

        // enforce the fixed size notion
        void setFixedSize(const QSize & size);
        void setFixedSize(int w, int h);
        QSize fixedSize() const;

    protected:
        // ::QAbstractButton
        void enterEvent(QEvent *);
        void leaveEvent(QEvent *);
        void paintEvent(QPaintEvent *);

    private:
        QSize m_fixedSize;
        QPixmap m_fixedPixmap;
        QFont m_hoverFont;
        QString m_hoverText;
        QPixmap m_hoverPixmap;
        bool m_fadeInactive;
        bool m_hovering;
};

#endif
