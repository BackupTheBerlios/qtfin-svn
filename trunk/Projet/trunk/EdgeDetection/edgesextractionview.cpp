#include "edgesextractionview.h"
#include <QPainter>

EdgesExtractionView::EdgesExtractionView(EdgesExtractionScene* scene, QWidget* parent):
        QGraphicsView(scene, parent), _scene(scene)
{
    _initWidth = 1000;
    _initHeight = 700;

    this->resize(1000, 700);

    qreal sceneWidth = this->scene()->width();
    qreal sceneHeight = this->scene()->height();

    qreal scaleX = _initWidth / sceneWidth;
    qreal scaleY = _initHeight / sceneHeight;

    qDebug("scaleX : %f\nscaleY : %f", this->scene()->width(), this->scene()->height());

    if(scaleX <= scaleY)
        this->scale(scaleX, scaleX);
    else
        this->scale(scaleY, scaleY);
}

void EdgesExtractionView::reScale(){
    qreal sceneWidth = this->scene()->width();
    qreal sceneHeight = this->scene()->height();

    qreal scaleX = _initWidth / sceneWidth;
    qreal scaleY = _initHeight / sceneHeight;

    qDebug("scaleX : %f\nscaleY : %f", scaleX, scaleY);

    if(scaleX <= scaleY)
        this->scale(scaleX, scaleX);
    else
        this->scale(scaleY, scaleY);
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



/*void EdgesExtractionView::resizeEvent(QResizeEvent *event)
{
   // this->scale(width() / _firstWidth, height() / _firstHeight);
}*/
