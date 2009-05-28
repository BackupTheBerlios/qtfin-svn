#include "drawpreviewview.h"

using namespace Data;

DrawPreviewView::DrawPreviewView(QWidget *parent, ProjectFile* preview, qreal width, qreal height):
        QGraphicsView(new QGraphicsScene(0, 0, width, height), parent), _preview(preview)
{
    _initWidth = this->width() - 10;
    _initHeight = this->height() - 10;


    _sceneWidth = width;
    _sceneHeight = height;

    qreal scaleX = _initWidth / _sceneWidth;
    qreal scaleY = _initHeight / _sceneHeight;


    if(scaleX <= scaleY){
        this->scale(scaleX, scaleX);
        _scale = scaleX;
    }
    else{
        this->scale(scaleY, scaleY);
        _scale = scaleY;
    }

}

void DrawPreviewView::reScale(){

    qreal scaleX = _initWidth / _sceneWidth;
    qreal scaleY = _initHeight / _sceneHeight;


    if(scaleX <= scaleY){
        this->scale(scaleX / _scale, scaleX / _scale);
        _scale = scaleX;
    }
    else{
        this->scale(scaleY /_scale, scaleY / _scale);
        _scale = scaleY;
    }
}

void DrawPreviewView::resizeEvent(QResizeEvent *event)
{
   _initWidth = this->width() - 10;
   _initHeight = this->height() - 10;
   this->reScale();
}



void DrawPreviewView::drawBackground(QPainter* painter, const QRectF & rect){
    //QPainter painter(this);
    Q_UNUSED(rect)

    painter->setPen(QPen(QBrush("red"), 3));
    painter->drawLine(QPointF(0, this->scene()->height() / 2), QPointF(this->scene()->width(), this->scene()->height() / 2));
    painter->setPen(QPen(QBrush("black"), 3));
    painter->drawRect(this->sceneRect());
    QList<int> listSeg = _preview->getAllSegmentKeys();
    qreal offsetY = this->scene()->height() / 2;
    for(int i = 0; i < listSeg.size(); i++){
        int p1;
        int p2;
        int c;
        _preview->getSegment(listSeg.value(i), p1, p2, c);

        float p1x;
        float p1y;
        _preview->getIntersectionPoint(p1, p1x, p1y);
        float p1yS = offsetY + p1y;
        p1y = offsetY - p1y;

        float p2x;
        float p2y;
        _preview->getIntersectionPoint(p2, p2x, p2y);
        float p2yS = offsetY + p2y;
        p2y = offsetY - p2y;

        float cx;
        float cy;
        _preview->getControlPoint(c, cx, cy);
        float cyS = offsetY + cy;
        cy = offsetY - cy;

        QPainterPath bezier(QPointF(p1x, p1y));
        bezier.cubicTo(cx, cy, cx, cy, p2x, p2y);
        QPainterPath bezierS(QPointF(p1x, p1yS));
        bezierS.cubicTo(cx, cyS, cx, cyS, p2x, p2yS);

        painter->setPen(QPen(QBrush("black"), 2));
        painter->drawPath(bezier);
        painter->drawPath(bezierS);

        painter->setPen(QPen(QBrush("blue"), 4));
        painter->drawPoint((int)p1x, (int)p1y);
    }
    int p, lp, c;
    _preview->getSegment(listSeg.last(), p, lp, c);
    float x, y;
    _preview->getIntersectionPoint(lp, x , y);
    y = offsetY - y;
    painter->drawPoint(qRound(x), qRound(y));

    painter->setBrush(QBrush("black"));
    painter->setPen(Qt::NoPen);
    painter->setOpacity(0.75);

    QPainterPath path1;
    path1.addRect(rect);
    QPainterPath path2;
    path2.addRect(this->sceneRect());
    path1 -= path2;
    painter->drawPath(path1);



    this->scene()->update();

}
