/***************************************************************************
 *                                                                         *
 *   This file is part of the Fotowall project,                            *
 *       http://www.enricoros.com/opensource/fotowall                      *
 *                                                                         *
 *   Copyright (C) 2007-2009 by Enrico Ros <enrico.ros@gmail.com>          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef __Canvas_h__
#define __Canvas_h__

#include "Shared/AbstractScene.h"
#include <QDataStream>
#include <QPainter>
#include <QPixmap>
#include <QRect>
#include <QTime>
#include <QDomElement>

class AbstractContent;
class AbstractConfig;
struct PictureEffect;
class ColorPickerItem;
class CanvasModeInfo;
class CanvasViewContent;
class HelpItem;
class HighlightItem;
class PictureContent;
class PictureSearchItem;
class QNetworkAccessManager;
class QTimer;
class TextContent;
class WebcamContent;
class WordcloudContent;

class Canvas : public AbstractScene
{
    Q_OBJECT
    public:
        Canvas(QObject * parent = 0);
        ~Canvas();

        // add/remove content
        void addAutoContent(const QStringList & fileNames);
        void addCanvasViewContent(const QStringList & fileNames);
        QList<PictureContent*> addPictureContent(const QStringList & fileNames);
        TextContent* addTextContent();
        WebcamContent* addWebcamContent(int input);
        void addWordcloudContent();
        void addManualContent(AbstractContent * content, const QPoint & pos);
        void clearContent();

        // ::AbstractScene
        void resize(const QSize & size);
        void resizeEvent();

        // item interaction
        void selectAllContent(bool selected = true);

        // selectors
        void setSearchPicturesVisible(bool visible);
        bool searchPicturesVisible() const;

        // arrangement
        void setForceFieldEnabled(bool enabled);
        bool forceFieldEnabled() const;
        void randomizeContents(bool position, bool rotation, bool opacity);

        // decorations
        void setBackContent(AbstractContent * content);

        enum BackMode { BackNone = 0, BackBlack = 1, BackWhite = 2, BackGradient = 3 };
        void setBackMode(BackMode mode);
        BackMode backMode() const;
        void clearBackContent();
        bool backContent() const;

        void setBackContentRatio(Qt::AspectRatioMode mode);
        Qt::AspectRatioMode backContentRatio() const;
        void setTopBarEnabled(bool enabled);
        bool topBarEnabled() const;
        void setBottomBarEnabled(bool enabled);
        bool bottomBarEnabled() const;
        void setTitleText(const QString & text);
        QString titleText() const;
        void setCDMarkers();
        void setDVDMarkers();
        void clearMarkers();

        // save, restore, load, help
        bool pendingChanges() const;
        void showIntroduction();
        void blinkBackGradients();

        // change size and project mode (CD, DVD, etc)
        CanvasModeInfo * modeInfo() const;

        void toXml(QDomElement & canvasElement) const;
        void fromXml(QDomElement & canvasElement);

        // render contents, but not the invisible items
        void renderVisible(QPainter * painter, const QRectF & target = QRectF(), const QRectF & source = QRectF(), Qt::AspectRatioMode aspectRatioMode = Qt::KeepAspectRatio, bool hideTools = true);
        QImage renderedImage(const QSize & size, Qt::AspectRatioMode aspectRatioMode = Qt::KeepAspectRatio, bool hideTools = true);
        bool printAsImage(int printerDpi, const QSize & pixelSize, bool landscape, Qt::AspectRatioMode aspectRatioMode = Qt::KeepAspectRatio);

    Q_SIGNALS:
        void backConfigChanged();
        void requestContentEditing(AbstractContent * content);
        void showPropertiesWidget(QWidget * widget);

    protected:
        void dragEnterEvent( QGraphicsSceneDragDropEvent * event );
        void dragMoveEvent( QGraphicsSceneDragDropEvent * event );
        void dropEvent( QGraphicsSceneDragDropEvent * event );
        void keyPressEvent( QKeyEvent * keyEvent );
        void mouseDoubleClickEvent( QGraphicsSceneMouseEvent * event );
        void contextMenuEvent( QGraphicsSceneContextMenuEvent * event );
        void drawBackground( QPainter * painter, const QRectF & rect );
        void drawForeground( QPainter * painter, const QRectF & rect );

    private:
        void initContent(AbstractContent * content, const QPoint & pos);
        CanvasViewContent * createCanvasView(const QPoint & pos);
        PictureContent * createPicture(const QPoint & pos);
        TextContent * createText(const QPoint & pos);
        WebcamContent * createWebcam(int input, const QPoint & pos);
        WordcloudContent * createWordcloud(const QPoint & pos);
        void deleteContent(AbstractContent * content);
        void deleteConfig(AbstractConfig * config);

        CanvasModeInfo * m_modeInfo;
        QNetworkAccessManager * m_networkAccessManager;
        QList<AbstractContent *> m_content;
        QList<AbstractConfig *> m_configs;
        QList<HighlightItem *> m_highlightItems;
        HelpItem * m_helpItem;
        ColorPickerItem * m_titleColorPicker;
        ColorPickerItem * m_foreColorPicker;
        ColorPickerItem * m_grad1ColorPicker;
        ColorPickerItem * m_grad2ColorPicker;
        BackMode m_backMode;
        AbstractContent * m_backContent;
        Qt::AspectRatioMode m_backContentRatio;
        bool m_topBarEnabled;
        bool m_bottomBarEnabled;
        QString m_titleText;
        QPixmap m_backTile;
        QPixmap m_backCache;
        QList<QGraphicsItem *> m_markerItems;   // used by some modes to show information items, which won't be rendered
        PictureSearchItem * m_pictureSearch;
        QTimer * m_forceFieldTimer;
        QTime m_forceFieldTime;

    private Q_SLOTS:
        friend class AbstractConfig; // HACK here, only to call 1 method
        friend class PixmapButton; // HACK here, only to call 1 method
        void slotSelectionChanged();
        void slotBackgroundContent();
        void slotConfigureContent(const QPoint & scenePoint);
        void slotEditContent();
        void slotStackContent(int);
        void slotCollateContent();
        void slotDeleteContent();
        void slotDeleteConfig();
        void slotApplyLook(quint32 frameClass, bool mirrored, bool allContent);
        void slotApplyEffect(const PictureEffect & effect, bool allPictures);
        void slotCrop();
        void slotFlipHorizontally();
        void slotFlipVertically();

        void slotTitleColorChanged();
        void slotForeColorChanged();
        void slotGradColorChanged();
        void slotBackContentChanged();

        void slotCloseIntroduction();
        void slotApplyForce();
};

#endif
