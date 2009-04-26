#include <QGraphicsScene>
#include <QTransform>
#include <stdio.h>
#include <math.h>
#include "pixmapitem.h"
#include "edgesextractionscene.h"

PixmapItem::PixmapItem(QPixmap pix):
        QGraphicsPixmapItem(pix),
        _posMousePress(QPointF(0,0)), _posMouseMove(QPointF(0,0)),
        _rotateAngl(0), _scale(1), enter(false)
{
    X = pixmap().width();
    Y = pixmap().height();

    QRectF rect = this->boundingRect();
    int radius = (int)sqrt(rect.width() * rect.width() +
                        rect.height() * rect.height()) / 2;
    _scircle = new SCircle((int)(this->pixmap().rect().center().x()),
                           (int)(this->pixmap().rect().center().y()),
                           radius, this);
    _scircle->addSPoint(1000);

    this->setAcceptHoverEvents(true);
    this->setFlag(QGraphicsItem::ItemIsMovable,true);
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
}

PixmapItem::~PixmapItem(){
    delete _scircle;
}

void PixmapItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget){
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if(!this->pixmap().isNull()){
        if(enter)
            painter->setPen(QColor("red"));
        QPointF c = this->boundingRect().center();
        qreal x = offset().x() + X / 2;
        qreal y = offset().y() + Y / 2;
        painter->translate(x, y);
        painter->rotate(_rotateAngl);
        painter->translate(-x, -y);
        painter->drawPixmap(offset(),pixmap());
        painter->drawRect(this->boundingRect());


        QPen pen(QBrush("red"), 3);
        painter->setPen(pen);

        QPoint point;
        for(int i = 0; i < _scircle->getSPointNb() - 1; i++){
            point = _scircle->getQPointRotate(i, _rotateAngl);
            if(point.x() + this->offset().x() > 100 && point.y() + this->offset().y() < 350)
                painter->drawLine(*(_scircle->getQPoint(i)) + this->offset(),
                                  *(_scircle->getQPoint(i+1)) + this->offset());
        }
        point = _scircle->getQPointRotate(_scircle->getSPointNb()-1, _rotateAngl);
        if(point.x() + this->offset().x() > 100 && point.y() + this->offset().y() < 350)
            painter->drawLine(*(_scircle->getQPoint(_scircle->getSPointNb()-1)) + this->offset(),
                              *(_scircle->getQPoint(0)) + this->offset());
    }
}


void PixmapItem::center(){
    qreal x = (this->scene()->width() - pixmap().width()) / 2;
    qreal y = (this->scene()->height() - pixmap().height()) / 2;
    setOffset(x, y);
}

void PixmapItem::translate2(qreal x, qreal y){
    this->translate(x, y);
    setOffset(offset().x() + x, offset().y() + y);
    this->childItems().first()->translate(x, y);
    _scircle->translateCenter((int)x, (int)y);
}

void PixmapItem::rotate2(qreal angl){
    _rotateAngl = angl;
    if(_rotateAngl >= 180)
        _rotateAngl -= 360;
    if(_rotateAngl < - 180)
        _rotateAngl += 360;
}

void PixmapItem::scaled(qreal scale){
    this->scale(1 + scale / 100 - _scale, 1 + scale / 100 - _scale);
    _scale = scale / 100;
}

void PixmapItem::mousePressEvent ( QGraphicsSceneMouseEvent * event ){
    _posMousePress = event->scenePos() - this->offset();
    _posMouseMove = event->scenePos();
}

void PixmapItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event){

    this->prepareGeometryChange();
    this->setOffset(event->scenePos() - _posMousePress);
    this->childItems().first()
            ->translate(event->scenePos().x() - _posMouseMove.x(),
                        event->scenePos().y() - _posMouseMove.y());
    _posMouseMove = event->scenePos();
    ((EdgesExtractionScene*)this->scene())->itemPositionChanged();

    this->scene()->update();
 }


void PixmapItem::hoverEnterEvent(QGraphicsSceneHoverEvent * event){
    enter = true;
    this->setCursor(QCursor(Qt::OpenHandCursor));
    this->scene()->update();
}

 void PixmapItem::hoverLeaveEvent(QGraphicsSceneHoverEvent * event){
    enter = false;
    this->setCursor(QCursor(Qt::ArrowCursor));
    this->scene()->update();
}
