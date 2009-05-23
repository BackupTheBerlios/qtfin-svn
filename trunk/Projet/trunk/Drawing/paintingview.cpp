#include "paintingview.h"

//PUBLIC

PaintingView::PaintingView(PaintingScene* scene, QWidget* parent)
    :QGraphicsView(scene, parent), _scene(scene){

    this->scale(1,-1);
}

//PUBLIC SLOTS

void PaintingView::zoomIn(){
    //on évite de zoomer trop pour éviter les bugs...
    if(_scene->scaleFactor() < ZOOMMAX){
        this->scale(ZOOMINFACTOR, ZOOMINFACTOR);
        _scene->scale(ZOOMINFACTOR);
    }
}

void PaintingView::zoomOut(){
    //au-delà de certaines valeurs, ça ne sert plus à rien de dézoomer...
    if(_scene->scaleFactor() > ZOOMMIN){
        this->scale(ZOOMOUTFACTOR, ZOOMOUTFACTOR);
        _scene->scale(ZOOMOUTFACTOR);
    }
}

//PROTECTED

void PaintingView::drawBackground(QPainter* painter, const QRectF& rect){

    //painter->drawRect(_scene->itemsBoundingRect());

    painter->setPen(Qt::DashDotLine);
    painter->drawRect(_scene->sceneRect());
    painter->drawRect(_scene->pointsBoundingZone());

    painter->setPen(Qt::DotLine);
    painter->setOpacity(0.2);
    QRect r = _scene->sceneRect().toRect();
    qreal gridUnit = _scene->gridUnit();
    for(qreal i = 0; i < r.bottomRight().x(); i+=gridUnit){
        painter->drawLine(QLineF(i, r.bottomLeft().y(), i, r.topLeft().y()));
    }
    for(qreal i = 0; i > r.topLeft().y(); i-=gridUnit){
        painter->drawLine(QLineF(r.bottomLeft().x(), i, r.bottomRight().x(), i));
    }
    for(qreal i = 0; i < r.bottomLeft().y(); i+=gridUnit){
        painter->drawLine(QLineF(r.bottomLeft().x(), i, r.bottomRight().x(), i));
    }

}

void PaintingView::wheelEvent(QWheelEvent* event){
    //QPointF p = this->mapFromScene(event->pos());
    if(event->delta() > 0){
        this->zoomIn();
    }else{
        this->zoomOut();
    }
}
