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

public slots:
    /**
    * Scales the view by the factor ZOOMINFACTOR and modifies the value
    * scaleFactor of the PaintingScene. It does nothing if the scale factor
    * of the PaintingScene is greater than ZOOMMAX.
    **/
    void zoomIn();

    /**
    * Scales the view by the factor ZOOMOUTFACTOR and modifies the value
    * scaleFactor of the PaintingScene. It does nothing if the scale factor
    * of the PaintingScene is lower than ZOOMMIN.
    **/
    void zoomOut();

protected:
    virtual void drawBackground(QPainter* painter, const QRectF& rect);
    virtual void wheelEvent(QWheelEvent* event);

    PaintingScene* _scene;

};


#endif // PAINTINGVIEW_H
