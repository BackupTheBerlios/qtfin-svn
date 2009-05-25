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
        _rotateAngl(0), _scale(1), _enter(false), _isForAlgo(false)
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
        if(_enter)
            painter->setPen(QColor("red"));
        QPointF c = this->boundingRect().center();
        qreal x = offset().x() + dimInitWidth / 2;
        qreal y = offset().y() + dimInitHeight / 2;
        painter->translate(x, y);
        if(_isForAlgo){
            painter->rotate(_rotateAngl);
            painter->scale(_scale, _scale);
        }
        else{
            painter->rotate(_rotateAngl + 180);
            painter->scale(_scale, - _scale);
        }
        painter->translate(-x, -y);
        this->offset();
        painter->drawPixmap(this->boundingRect().topLeft(), pixmap());
        //painter->drawRect(this->boundingRect());

        QPen pen(QBrush("black"), 1);
        painter->setPen(pen);
        QRectF rect = this->boundingRect();
        qreal radius = sqrt(rect.width() * rect.width() +
                            rect.height() * rect.height()) / 2;
        painter->drawEllipse(rect.center() + QPointF(-1, -1), radius, radius);

        pen = QPen(QBrush("red"), 3);
        painter->setPen(pen);

        if(_isForAlgo){

            qreal axeSymetry = ((EdgesExtractionScene*)this->scene())->symetryAxe();
            qreal heal = ((EdgesExtractionScene*)this->scene())->heal();
            QPointF point;

            bool overAxe;
            point = _scircle->getQPointRotate(0, _rotateAngl, _scale);
            overAxe = point.x() + this->offset().x() > heal  && point.y() + this->offset().y() < axeSymetry;
            bool fPOverAxe = overAxe;

            for(int i = 1; i < _scircle->getSPointNb(); i++){
                point = _scircle->getQPointRotate(i, _rotateAngl, _scale);
                if(point.x() + this->offset().x() > heal  && point.y() + this->offset().y() < axeSymetry){
                    if(overAxe)
                        painter->drawLine(*(_scircle->getQPoint(i - 1)) + this->offset(),
                                          *(_scircle->getQPoint(i)) + this->offset());
                    else{
                        QPointF* iP = new QPointF();
                        QLineF segment = QLineF(_scircle->getQPointRotate(i - 1, _rotateAngl, _scale) + this->offset(),
                                                _scircle->getQPointRotate(i, _rotateAngl, _scale) + this->offset());
                        int intersect = segment.intersect(QLineF(0, axeSymetry, heal * 100, axeSymetry), iP);
                        if(intersect != QLineF::BoundedIntersection)
                            segment.intersect(QLineF(heal, 0, heal, axeSymetry * 2), iP);
                        painter->translate(x, y);
                        painter->rotate(-_rotateAngl);
                        painter->scale(1/_scale, 1/_scale);
                        painter->translate(-x, -y);
                        painter->drawLine(*iP, _scircle->getQPointRotate(i, _rotateAngl, _scale) + QPointF(0.5, 0.5) + this->offset());
                        painter->translate(x, y);
                        painter->rotate(_rotateAngl);
                        painter->scale(_scale,_scale);
                        painter->translate(-x, -y);
                        overAxe = true;
                        delete iP;
                    }
                }else{
                    if(overAxe){
                        QPointF* iP = new QPointF();
                        QLineF segment = QLineF(_scircle->getQPointRotate(i - 1, _rotateAngl, _scale)+ this->offset(),
                                                _scircle->getQPointRotate(i, _rotateAngl, _scale) + this->offset());
                        int intersect = segment.intersect(QLineF(0, axeSymetry, heal * 100, axeSymetry), iP);
                        if(intersect != QLineF::BoundedIntersection)
                            segment.intersect(QLineF(heal, 0, heal, axeSymetry * 2), iP);
                        painter->translate(x, y);
                        painter->rotate(-_rotateAngl);
                        painter->scale(1/_scale, 1/_scale);
                        painter->translate(-x, -y);
                        painter->drawLine(_scircle->getQPointRotate(i - 1, _rotateAngl, _scale) + QPointF(0.5, 0.5) + this->offset(), *iP);
                        painter->translate(x, y);
                        painter->rotate(_rotateAngl);
                        painter->scale(_scale,_scale);
                        painter->translate(-x, -y);
                        overAxe = false;
                        delete iP;
                    }
                }
            }
            if(overAxe && fPOverAxe)
                painter->drawLine(*(_scircle->getQPoint(_scircle->getSPointNb()-1)) + this->offset(),
                                  *(_scircle->getQPoint(0)) + this->offset());
            if(!overAxe && fPOverAxe){
                QPointF* iP = new QPointF();
                QLineF segment = QLineF(_scircle->getQPointRotate(_scircle->getSPointNb()-1, _rotateAngl, _scale) + this->offset(),
                                        _scircle->getQPointRotate(0, _rotateAngl, _scale) + this->offset());
                int intersect = segment.intersect(QLineF(0, axeSymetry, heal * 100, axeSymetry), iP);
                if(intersect != QLineF::BoundedIntersection)
                    segment.intersect(QLineF(heal, 0, heal, axeSymetry * 2), iP);
                painter->translate(x, y);
                painter->rotate(-_rotateAngl);
                painter->scale(1/_scale, 1/_scale);
                painter->translate(-x, -y);
                painter->drawLine(*iP, _scircle->getQPointRotate(0, _rotateAngl, _scale) + this->offset());
                painter->translate(x, y);
                painter->rotate(_rotateAngl);
                painter->scale(_scale,_scale);
                painter->translate(-x, -y);
                delete iP;
            }
            if(overAxe && !fPOverAxe){
                QPointF* iP = new QPointF();
                QLineF segment = QLineF(_scircle->getQPointRotate(_scircle->getSPointNb()-1, _rotateAngl, _scale)+ this->offset(),
                                        _scircle->getQPointRotate(0, _rotateAngl, _scale) + this->offset());
               int intersect = segment.intersect(QLineF(0, axeSymetry, heal * 100, axeSymetry), iP);
               if(intersect != QLineF::BoundedIntersection)
                   segment.intersect(QLineF(heal, 0, heal, axeSymetry * 2), iP);
               painter->translate(x, y);
               painter->rotate(-_rotateAngl);
               painter->scale(1/_scale, 1/_scale);
               painter->translate(-x, -y);
               painter->drawLine(_scircle->getQPointRotate(_scircle->getSPointNb()-1, _rotateAngl, _scale) + this->offset(), *iP);
               painter->translate(x, y);
               painter->rotate(_rotateAngl);
               painter->scale(_scale,_scale);
               painter->translate(-x, -y);
               delete iP;
           }

            QPen pen1(QBrush("yellow"), 5);
            painter->setPen(pen1);
            /*painter->drawPoint(_scircle->getQPoint(89)->x() + this->offset().x(),
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
                               _scircle->getQPoint(23)->y() + this->offset().y());*/
            /*QPen pen2(QBrush("green"), 5);
            painter->setPen(pen2);
            painter->drawPoint(_scircle->getQPoint(753)->x() + this->offset().x(),
                               _scircle->getQPoint(753)->y() + this->offset().y());*/

        }
    }
}


void PixmapItem::center(){
    qreal x = (this->scene()->width() - pixmap().width()) / 2;
    qreal y = (this->scene()->height() - pixmap().height()) / 2;
    setOffset(x, y);
}

void PixmapItem::translate2(qreal x, qreal y){
    this->setOffset(offset().x() + x, offset().y() + y);
    if(this->childItems().first() != NULL)
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
    if(scale >= 0.01 && scale <= 2){
        _scale = scale;
        ((RotateCircle*)(this->childItems().first()))->setScale(_scale);
        X = dimInitWidth * _scale;
        Y = dimInitHeight * _scale;
    }
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
    _enter = true;
    this->setCursor(QCursor(Qt::OpenHandCursor));
    this->scene()->update();
}

 void PixmapItem::hoverLeaveEvent(QGraphicsSceneHoverEvent * event){
    _enter = false;
    this->setCursor(QCursor(Qt::ArrowCursor));
    this->scene()->update();
}

void PixmapItem::wheelEvent ( QGraphicsSceneWheelEvent * event ){
    event->accept();
    if(event->delta() > 0 && _scale < 2)
        this->scaled(_scale + 0.01);
    if(event->delta() < 0 &&_scale > 0.01)
        this->scaled(_scale - 0.01);
    if(_isForAlgo)
        ((EdgesExtractionScene*)this->scene())->itemScaleChanged();
    this->scene()->update();
}
