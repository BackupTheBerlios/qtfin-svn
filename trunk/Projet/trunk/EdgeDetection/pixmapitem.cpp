#include <QGraphicsScene>
#include <QTransform>
#include <stdio.h>
#include <math.h>
#include <QLineF>
#include <QpointF>
#include "pixmapitem.h"
#include "edgesextractionscene.h"

PixmapItem::PixmapItem(QPixmap pix):
        QGraphicsPixmapItem(pix),
        _posMousePress(QPointF(0,0)), _posMouseMove(QPointF(0,0)),
        _rotateAngl(0), _scale(1), enter(false), _isForAlgo(false)
{
    dimInitWidth = pixmap().width();
    dimInitHeight = pixmap().height();
    X = dimInitWidth;
    Y = dimInitHeight;

    /*QRectF rect = this->boundingRect();
    qreal radius = sqrt(rect.width() * rect.width() +
                        rect.height() * rect.height()) / 2;
    _scircle = new SCircle(this->pixmap().rect().center().x(),
                           this->pixmap().rect().center().y(),
                           radius, this);
    _scircle->addSPoint(1000);*/

    this->setAcceptHoverEvents(true);
    this->setFlag(QGraphicsItem::ItemIsMovable,true);
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
}

PixmapItem::~PixmapItem(){
    if(_isForAlgo)
        delete _scircle;
}

void PixmapItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget){
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if(!this->pixmap().isNull()){
        if(enter)
            painter->setPen(QColor("red"));
        QPointF c = this->boundingRect().center();
        qreal x = offset().x() + dimInitWidth / 2;
        qreal y = offset().y() + dimInitHeight / 2;
        painter->translate(x, y);
        painter->rotate(_rotateAngl);
        painter->scale(_scale, _scale);
        painter->translate(-x, -y);
        painter->drawPixmap(offset(), pixmap());
        painter->drawRect(this->boundingRect());


        QPen pen(QBrush("red"), 3);
        painter->setPen(pen);

        if(_isForAlgo){
            qreal axeSymetry = ((EdgesExtractionScene*)this->scene())->symetryAxe();
            qreal heal = ((EdgesExtractionScene*)this->scene())->heal();
            QPointF point;
            for(int i = 0; i < _scircle->getSPointNb() - 1; i++){
                point = _scircle->getQPointRotate(i, _rotateAngl);
                if(point.x() + this->offset().x() >= heal  && point.y() + this->offset().y() <= axeSymetry)
                    painter->drawLine(*(_scircle->getQPoint(i)) + this->offset(),
                                      *(_scircle->getQPoint(i+1)) + this->offset());
            }
            point = _scircle->getQPointRotate(_scircle->getSPointNb()-1, _rotateAngl);
            if(point.x() + this->offset().x() >= heal * _scale && point.y() + this->offset().y() <= axeSymetry * _scale)
                painter->drawLine(*(_scircle->getQPoint(_scircle->getSPointNb()-1)) + this->offset(),
                                  *(_scircle->getQPoint(0)) + this->offset());

            QPen pen1(QBrush("yellow"), 5);
            painter->setPen(pen1);
            painter->drawPoint(_scircle->getQPoint(89)->x() + this->offset().x(),
                               _scircle->getQPoint(89)->y() + this->offset().y());
            painter->drawPoint(_scircle->getQPoint(12)->x() + this->offset().x(),
                               _scircle->getQPoint(12)->y() + this->offset().y());
            painter->drawPoint(_scircle->getQPoint(13)->x() + this->offset().x(),
                               _scircle->getQPoint(13)->y() + this->offset().y());
            painter->drawPoint(_scircle->getQPoint(20)->x() + this->offset().x(),
                               _scircle->getQPoint(20)->y() + this->offset().y());
            painter->drawPoint(_scircle->getQPoint(22)->x() + this->offset().x(),
                               _scircle->getQPoint(22)->y() + this->offset().y());
            painter->drawPoint(_scircle->getQPoint(23)->x() + this->offset().x(),
                               _scircle->getQPoint(23)->y() + this->offset().y());
            /*QPen pen2(QBrush("green"), 5);
            painter->setPen(pen2);
            painter->drawPoint(_scircle->getQPoint(753)->x() + this->offset().x(),
                               _scircle->getQPoint(753)->y() + this->offset().y());*/

        }
        else{
            QPen pen(QBrush("black"), 1);
            painter->setPen(pen);
            QRectF rect = this->boundingRect();
            qreal radius = sqrt(rect.width() * rect.width() +
                            rect.height() * rect.height()) / 2;
            painter->drawEllipse(rect.center(), radius, radius);
        }
    }
}


void PixmapItem::center(){
    qreal x = (this->scene()->width() - pixmap().width()) / 2;
    qreal y = (this->scene()->height() - pixmap().height()) / 2;
    setOffset(x, y);
}

void PixmapItem::translate2(qreal x, qreal y){
    setOffset(offset().x() + x, offset().y() + y);
    this->childItems().first()->translate(x, y);
}

void PixmapItem::rotate2(qreal angle){
    _rotateAngl = angle;
    if(_rotateAngl >= 180)
        _rotateAngl -= 360;
    if(_rotateAngl < - 180)
        _rotateAngl += 360;
}

void PixmapItem::scaled(qreal scale){

    qreal scaleW = dimInitWidth * scale / 100;
    qreal scaleH = dimInitHeight * scale / 100;

    //this->scale(scaleW / X, scaleH / Y);

    //this->scale(1 + scale / 100 - _scale, 1 + scale / 100 - _scale);
    _scale = scale / 100;
    ((RotateCircle*)(this->childItems().first()))->setScale(_scale);
    X = dimInitWidth * _scale;
    Y = dimInitHeight * _scale;

    qDebug("----------\nScale : %f\nwidth : %f\nheigth : %f\n----------", _scale, scaleW, scaleH);
    qDebug("Offset : (%f,%f)", this->offset().x(), this->offset().y());
    qDebug("Top left : (%f,%f)", this->boundingRect().topLeft().x(), this->boundingRect().topLeft().y());

}


void PixmapItem::mousePressEvent ( QGraphicsSceneMouseEvent * event ){
    _posMousePress = (event->scenePos() - this->offset());
    _posMouseMove = event->scenePos();
}

void PixmapItem::mouseMoveEvent(QGraphicsSceneMouseEvent* event){

    this->prepareGeometryChange();
    this->setOffset((event->scenePos() - _posMousePress));
    this->childItems().first()
            ->translate(event->scenePos().x() - _posMouseMove.x(),
                        event->scenePos().y() - _posMouseMove.y());
    _posMouseMove = event->scenePos();
    if(_isForAlgo)
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
