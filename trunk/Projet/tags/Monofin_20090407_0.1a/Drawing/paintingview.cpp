#include "paintingview.h"

//PUBLIC

PaintingView::PaintingView(PaintingScene* scene, QWidget* parent)
    :QGraphicsView(scene, parent), _scene(scene){

    this->scale(1,-1);
}


//PROTECTED

void PaintingView::drawBackground(QPainter* painter, const QRectF& rect){
    painter->setPen(Qt::DashDotLine);
    painter->drawRect(_scene->sceneRect());
    painter->drawRect(_scene->pointsBoundingZone());

    painter->setPen(Qt::DotLine);
    painter->setOpacity(0.2);
    QRect r = _scene->sceneRect().toRect();
    for(int i = 0; i < r.bottomRight().x(); i+=20){
        painter->drawLine(i, r.bottomLeft().y(), i, r.topLeft().y());
    }
    for(int i = r.bottomLeft().y(); i > r.topLeft().y(); i-=20){
        painter->drawLine(r.bottomLeft().x(), i, r.bottomRight().x(), i);
    }

}

void PaintingView::wheelEvent(QWheelEvent* event){
    //QPointF p = this->mapFromScene(event->pos());
    if(event->delta() > 0){
        this->scale(1.25,1.25);
        _scene->scale(1.25);
    }else{
        this->scale(0.8,0.8);
        _scene->scale(0.8);
    }
}
