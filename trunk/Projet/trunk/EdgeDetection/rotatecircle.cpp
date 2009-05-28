#include "rotatecircle.h"
#include <stdio.h>
#include <math.h>
#include <QPen>
#include <QgraphicsScene>
#include "edgesextractionscene.h"

#define PI 3.14159265358979323846

RotateCircle::RotateCircle(PixmapItem * parent):
        QGraphicsRectItem(parent), _scale(1)

{
    QRectF rect = parent->boundingRect();
    QPointF center = rect.center();
    qreal diameter = sqrt(rect.width() * rect.width() +
                        rect.height() * rect.height());
    _radiusInit = diameter / 2;
    _radius = _radiusInit;
    if(((PixmapItem*)(this->parentItem()))->isForAlgo())
        this->setRect(center.x() - 10,
                      center.y() - _radius - 8,
                      20,
                      20);
    else
        this->setRect(center.x() - 10,
                      center.y() - _radius - 10,
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
    qreal x = this->rect().x() + this->rect().width() / 2;
    qreal y = this->rect().y() + this->rect().height() / 2;
    painter->translate(x, y);
    painter->scale(_scale, _scale);
    painter->translate(-x, -y);
    QPixmap pix;
    if(_enter)
        //painter->setBrush(QBrush(QColor("red")));
        pix = QPixmap(":/images/blueSatellite.png");
    else
        //painter->setBrush(QBrush(QColor("green")));
        pix = QPixmap(":/images/redSatellite.png");

    //painter->drawEllipse(this->rect());
    painter->drawPixmap(this->rect().toRect(), pix);
}

void RotateCircle::hoverEnterEvent(QGraphicsSceneHoverEvent * event){
    _enter = true;
    _isMoved = true;
    this->setCursor(QCursor(Qt::OpenHandCursor));
    this->scene()->update(this->rect());
    this->update();
}

void RotateCircle::hoverLeaveEvent(QGraphicsSceneHoverEvent * event){
    _enter = false;
    _isMoved = false;
    this->setCursor(QCursor(Qt::ArrowCursor));
    this->scene()->update(this->rect());
    this->update();
}

void RotateCircle::setPosition(qreal angle){
    this->setPos(cos((angle - 90) * PI / 180) * _radius,
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
    if(((PixmapItem*)(this->parentItem()))->isForAlgo())
      ((EdgesExtractionScene*)(this->scene()))->rotateCircleMoved(angle);
    this->scene()->update();
}

void RotateCircle::setScale(qreal scale){
    _scale = scale;
    qreal radiusP = _radius;
    _radius = _radiusInit * scale;
    qreal angle = ((PixmapItem*)(this->parentItem()))->rotationAngle();
    this->setPosition(0);
    this->translate(0, radiusP - _radius);
    this->setPosition(angle);
}
