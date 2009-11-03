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

#ifndef __HomeAppliance_h__
#define __HomeAppliance_h__

#include "Shared/PlugGui/AbstractAppliance.h"
class HomeScene;
class UrlHistoryBox;

class HomeAppliance : public QObject, public PlugGui::AbstractAppliance
{
    Q_OBJECT
    public:
        HomeAppliance(QObject * parent = 0);
        ~HomeAppliance();

        // ::Appliance::AbstractAppliance
        QString applianceName() const { return tr("Home"); }

    private:
        HomeScene * m_scene;
        UrlHistoryBox * m_historyBox;

    private Q_SLOTS:
        void slotSceneKeyPressed(int qtKey);
        void slotLoadCanvas(const QUrl & url);
        void slotStartCanvas();
        void slotStartWordcloud();
        void slotStartWizard();
};

#endif
