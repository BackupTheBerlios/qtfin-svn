#include "tangent.h"

//PUBLIC

Tangent::Tangent(QPointF p1, QPointF p2)
        :QGraphicsItem(){
    _line = new QLineF(p1, p2);

}

QRectF Tangent::boundingRect() const{
    return QRectF(_line->p1(), _line->p2());
}

void Tangent::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget){
    painter->setPen(Qt::DashLine);
    painter->drawLine(*_line);
}

void Tangent::setLine(QPointF p1, QPointF p2){
    _line->setPoints(p1,p2);

}
