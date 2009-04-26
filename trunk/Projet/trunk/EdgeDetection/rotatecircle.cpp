#include "rotatecircle.h"
#include <stdio.h>
#include <math.h>
#include <QPen>
#include <QgraphicsScene>
#include "edgesextractionscene.h"

#define PI 3.14159265358979323846

RotateCircle::RotateCircle(PixmapItem * parent):
        QGraphicsRectItem(parent)

{
    QRectF rect = parent->boundingRect();
    QPointF center = rect.center();
    qreal diameter = sqrt(rect.width() * rect.width() +
                        rect.height() * rect.height());
    _radius = diameter / 2;
    this->setRect(center.x() - 10,
                  center.y() - _radius - 8,
                  20,
                  20);
    _enter = false;
    _isMoved = false;
    this->setAcceptHoverEvents(true);
    this->setFlag(QGraphicsItem::ItemIsMovable,true);
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
}

void RotateCircle::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget){
    painter->setPen(Qt::NoPen);
    if(_enter)
        painter->setBrush(QBrush(QColor("red")));
    else
        painter->setBrush(QBrush(QColor("green")));

    painter->drawEllipse(this->rect());
}

void RotateCircle::hoverEnterEvent(QGraphicsSceneHoverEvent * event){
    _enter = true;
    _isMoved = true;
    this->setCursor(QCursor(Qt::OpenHandCursor));
    this->scene()->update(this->rect());
}

void RotateCircle::hoverLeaveEvent(QGraphicsSceneHoverEvent * event){
    _enter = false;
    _isMoved = false;
    this->setCursor(QCursor(Qt::ArrowCursor));
    this->scene()->update(this->rect());
}

void RotateCircle::setPosition(qreal angle){
    this->setPos(cos((angle - 90) * PI / 180) * _radius - this->parentItem()->pos().x(),
                 sin((angle - 90) * PI / 180) * _radius + _radius);
}

void RotateCircle::mouseMoveEvent(QGraphicsSceneMouseEvent* event){
    this->prepareGeometryChange();
    QPointF centerP = this->parentItem()->boundingRect().center();

    qreal x = event->scenePos().x() - centerP.x();
    qreal y = - event->scenePos().y() + centerP.y();

    qreal angle = 0;
    if(x > 0 && y >= 0){
        angle = -(atan(y/x) - PI / 2) * 180 / PI;
    }

    if(x < 0 && y >= 0){
        angle = -(PI + atan(y/x) - PI / 2) * 180 / PI;
    }

    if(x > 0 && y < 0){
        angle = -(atan(y/x) - PI / 2) * 180 / PI;
    }

    if(x < 0 && y < 0){
        angle = -(PI + atan(y/x) - PI / 2) * 180 / PI;
    }

    this->setPosition(angle);
    ((PixmapItem*)(this->parentItem()))->rotate2(angle);
    ((EdgesExtractionScene*)(this->scene()))->rotateCircleMoved(angle);
    this->scene()->update();
}
