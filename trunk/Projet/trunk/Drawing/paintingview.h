#ifndef PAINTINGVIEW_H
#define PAINTINGVIEW_H

#include <QGraphicsView>
#include <QWidget>
#include <QWheelEvent>
#include "paintingscene.h"

class PaintingView: public QGraphicsView{

#define ZOOMMAX 100000
#define ZOOMMIN 0.000001

#define ZOOMINFACTOR 1.25
#define ZOOMOUTFACTOR 0.8

    Q_OBJECT

public:
    /**
    * Constructs a Graphic View attached to the given scene and
    * inverts the y axis.
    *@param scene a pointer to the PaintingScene which will be
    * visualized by the PaintingView
    *@param parent the parent widget of the Graphic View
    **/
    PaintingView(PaintingScene* scene, QWidget* parent = 0);

    //void drawPixmap(PixmapItem* pixmap);
    //void stopDrawingPixmap(){_hasAPixmapToDraw = false;}

public slots:
    /**
    * Scales the view by the given factor and modifies the value
    * scaleFactor of the PaintingScene. It does nothing if the scale factor
    * of the PaintingScene is greater than ZOOMMAX.
    *@param factor the factor by which the scale of the view is multiplied
    * (by default : the value of ZOOMINFACTOR, typically around 1.25)
    **/
    void zoomIn(qreal factor = ZOOMINFACTOR);

    /**
    * Scales the view by the given factor and modifies the value
    * scaleFactor of the PaintingScene. It does nothing if the scale factor
    * of the PaintingScene is lower than ZOOMMIN.
    *@param factor the factor by which the scale of the view is multiplied
    * (by default : the value of ZOOMAXFACTOR, typically around 0.8)
    **/
    void zoomOut(qreal factor = ZOOMOUTFACTOR);

protected:
    //virtual void drawBackground(QPainter* painter, const QRectF& rect);
    virtual void wheelEvent(QWheelEvent* event);

    //bool _hasAPixmapToDraw;
    //PixmapItem* _pixmap;
    PaintingScene* _scene;

};


#endif // PAINTINGVIEW_H
