#include "edgesextractionview.h"
#include <QPainter>

EdgesExtractionView::EdgesExtractionView(EdgesExtractionScene* scene, QWidget* parent):
        QGraphicsView(scene, parent), _scene(scene)
{
    _initWidth = 900;
    _initHeight = 700;

    this->baseSize();

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


/*void EdgesExtractionView::resizeEvent(QResizeEvent *event)
{
   // this->scale(width() / _firstWidth, height() / _firstHeight);
}*/
