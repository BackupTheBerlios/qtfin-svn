#include "edgesextractionview.h"
#include <QPainter>
#include <QRect>

EdgesExtractionView::EdgesExtractionView(EdgesExtractionScene* scene, QWidget* parent):
        QGraphicsView(scene, parent), _scene(scene)
{
    _initWidth = this->width() - 10;
    _initHeight = this->height() - 10;


    qreal sceneWidth = this->scene()->width();
    qreal sceneHeight = this->scene()->height();

    qreal scaleX = _initWidth / sceneWidth;
    qreal scaleY = _initHeight / sceneHeight;


    if(scaleX <= scaleY){
        this->scale(scaleX, scaleX);
        _scale = scaleX;
    }
    else{
        this->scale(scaleY, scaleY);
        _scale = scaleY;
    }
}

void EdgesExtractionView::reScale(){
    qreal sceneWidth = this->scene()->width();
    qreal sceneHeight = this->scene()->height();

    qreal scaleX = _initWidth / sceneWidth;
    qreal scaleY = _initHeight / sceneHeight;

    qDebug("scaleX : %f\nscaleY : %f", scaleX, scaleY);

    if(scaleX <= scaleY){
        this->scale(scaleX / _scale, scaleX / _scale);
        _scale = scaleX;
    }
    else{
        this->scale(scaleY /_scale, scaleY / _scale);
        _scale = scaleY;
    }
}

qreal EdgesExtractionView::getScale(){
    qreal sceneWidth = this->scene()->width();
    qreal sceneHeight = this->scene()->height();

    qreal scaleX = _initWidth / sceneWidth;
    qreal scaleY = _initHeight / sceneHeight;

    qDebug("scaleX : %f\nscaleY : %f", scaleX, scaleY);

    if(scaleX <= scaleY)
        return scaleX;
    else
        return scaleY;
}



void EdgesExtractionView::resizeEvent(QResizeEvent *event)
{
   _initWidth = this->width() - 10;
   _initHeight = this->height() - 10;
   this->reScale();
}

void EdgesExtractionView::drawForeground(QPainter *painter, const QRectF &rect){

    painter->setBrush(QBrush("black"));
    painter->setOpacity(0.75);

    QPainterPath path1;
    path1.addRect(rect);
    QPainterPath path2;
    path2.addRect(this->sceneRect());
    path1 -= path2;
    painter->drawPath(path1);

}
