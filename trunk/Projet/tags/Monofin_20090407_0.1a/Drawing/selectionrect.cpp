#include "selectionrect.h"

SelectionRect::SelectionRect(QRectF rectangle)
        :QGraphicsRectItem(rectangle){}

void SelectionRect::setBottomRight(QPointF p){
    this->setRect(QRectF(this->rect().topLeft(), p));
}

void SelectionRect::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget){
    painter->setPen(Qt::DashLine);
    painter->setOpacity(0.8);
    painter->drawRect(this->rect());

}
