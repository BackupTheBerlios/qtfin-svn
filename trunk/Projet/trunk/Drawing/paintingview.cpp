#include "paintingview.h"

//PUBLIC

PaintingView::PaintingView(PaintingScene* scene, QWidget* parent)
    :QGraphicsView(scene, parent), //_hasAPixmapToDraw(false),
    _scene(scene){

    //_pixmap = 0;

    this->scale(1,-1);
}

/*void PaintingView::drawPixmap(PixmapItem* pixmap){
    _pixmap = pixmap;
    _hasAPixmapToDraw = true;

}*/

//PUBLIC SLOTS

void PaintingView::zoom(qreal factor){
    //on évite de zoomer trop pour éviter les bugs...
    if(_scene->scaleFactor() < ZOOMMAX &&
    //au-delà de certaines valeurs, ça ne sert plus à rien de dézoomer...
       _scene->scaleFactor() > ZOOMMIN){

        this->scale(factor, factor);
        _scene->scale(factor);
    }
}

//PROTECTED

//void PaintingView::drawBackground(QPainter* painter, const QRectF& rect){
//
//    Q_UNUSED(rect);
//
//    //painter->drawRect(_scene->itemsBoundingRect());
//
//    /*if(_hasAPixmapToDraw){
//        painter->drawPixmap(QPointF(0,0), _pixmap->pixmap());
//    }*/
//
//    painter->setPen(Qt::DashDotLine);
//    painter->drawRect(_scene->sceneRect());
//    painter->drawRect(_scene->pointsBoundingZone());
//
//    painter->setPen(Qt::DotLine);
//    painter->setOpacity(0.2);
//    QRect r = _scene->sceneRect().toRect();
//    qreal gridUnit = _scene->gridUnit();
//    for(qreal i = 0; i < r.bottomRight().x(); i+=gridUnit){
//        painter->drawLine(QLineF(i, r.bottomLeft().y(), i, r.topLeft().y()));
//    }
//    for(qreal i = 0; i > r.topLeft().y(); i-=gridUnit){
//        painter->drawLine(QLineF(r.bottomLeft().x(), i, r.bottomRight().x(), i));
//    }
//    for(qreal i = 0; i < r.bottomLeft().y(); i+=gridUnit){
//        painter->drawLine(QLineF(r.bottomLeft().x(), i, r.bottomRight().x(), i));
//    }
//
//}

void PaintingView::wheelEvent(QWheelEvent* event){
    //QPointF p = this->mapFromScene(event->pos());
    if(event->delta() > 0){
        this->zoom(ZOOMINFACTOR);
    }else{
        this->zoom(ZOOMOUTFACTOR);
    }
}
