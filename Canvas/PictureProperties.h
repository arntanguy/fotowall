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

#ifndef __PictureProperties_h__
#define __PictureProperties_h__

#include "ContentProperties.h"
#include "ui_PictureProperties.h"

class PictureProperties : public ContentProperties, public Ui::PictureProperties {
    Q_OBJECT
    public:
        friend class PictureContent;
        PictureProperties(QWidget * parent = 0);
};

#endif
