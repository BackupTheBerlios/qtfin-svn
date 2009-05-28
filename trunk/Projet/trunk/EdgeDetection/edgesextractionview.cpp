#include "edgesextractionview.h"
#include <QPainter>

EdgesExtractionView::EdgesExtractionView(EdgesExtractionScene* scene, QWidget* parent):
        QGraphicsView(scene, parent), _scene(scene)
{
    _initWidth = this->width() - 10;
    _initHeight = this->height() - 10;

    //this->resize(1000, 700);

    qreal sceneWidth = this->scene()->width();
    qreal sceneHeight = this->scene()->height();

    qreal scaleX = _initWidth / sceneWidth;
    qreal scaleY = _initHeight / sceneHeight;

    qDebug("scaleX : %f\nscaleY : %f", this->scene()->width(), this->scene()->height());

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
