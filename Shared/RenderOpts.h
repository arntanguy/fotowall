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

#ifndef __RenderOpts_h__
#define __RenderOpts_h__

#include <QColor>

class RenderOpts
{
    public:
        // defaults
        static bool LastMirrored;

        // global options
        static bool HQRendering;
        static bool PDFExporting;
        static bool ARGBWindow;
        static bool OpenGLWindow;

        // other options
        static bool OxygenStyleQuirks;

        // global highlight color
        static QColor hiColor;
};

#endif
