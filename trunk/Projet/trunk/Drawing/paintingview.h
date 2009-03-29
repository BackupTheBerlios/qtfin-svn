#ifndef PAINTINGVIEW_H
#define PAINTINGVIEW_H

#include <QGraphicsView>
#include <QWidget>
#include <QWheelEvent>
#include "paintingscene.h"

class PaintingView: public QGraphicsView{

public:
    PaintingView(PaintingScene* scene, QWidget* parent = 0);

protected:
    PaintingScene* _scene;


    virtual void drawBackground(QPainter* painter, const QRectF& rect);
    virtual void wheelEvent(QWheelEvent* event);

};


#endif // PAINTINGVIEW_H
