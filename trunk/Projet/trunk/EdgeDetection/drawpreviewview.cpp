#include "drawpreviewview.h"

using namespace Data;

DrawPreviewView::DrawPreviewView(QWidget *parent, ProjectFile* preview, qreal width, qreal height, qreal scale):
        QGraphicsView(new QGraphicsScene(0, 0, width, height), parent), _preview(preview), _scale(scale)
{
    //this->resize(1000, 700);
    this->scale(scale, scale);
}



void DrawPreviewView::drawBackground(QPainter* painter, const QRectF & rect){
    //QPainter painter(this);
    Q_UNUSED(rect)

    painter->setPen(QPen(QBrush("red"), 3));
    painter->drawLine(QPointF(0, this->height() / 2), QPointF(this->width(), this->height() / 2));
    QList<int> listSeg = _preview->getAllSegmentKeys();
    for(int i = 0; i < listSeg.size(); i++){
        int p1;
        int p2;
        int c;
        _preview->getSegment(listSeg.value(i), p1, p2, c);

        float p1x;
        float p1y;
        _preview->getIntersectionPoint(p1, p1x, p1y);
        float p1yS = this->height() / 2 + p1y;
        p1y = this->height() / 2 - p1y;

        float p2x;
        float p2y;
        _preview->getIntersectionPoint(p2, p2x, p2y);
        float p2yS = this->height() / 2 + p2y;
        p2y = this->height() / 2 - p2y;

        float cx;
        float cy;
        _preview->getControlPoint(c, cx, cy);
        float cyS = this->height() / 2 + cy;
        cy = this->height() / 2 - cy;

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
    y = this->height() / 2 - y;
    painter->drawPoint((int)x, (int)y);
    this->scene()->update();

    qDebug("DrawBackground");

}
