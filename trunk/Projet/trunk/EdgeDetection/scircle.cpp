/*
 * SCircle.cpp
 *
 *  Created on: 3 mars 2009
 *      Author: Nicolas
 */

#include "scircle.h"
#include <iostream>
#include <QGraphicsScene>

using namespace std;

SCircle::SCircle(qreal cX, qreal cY, qreal radius, PixmapItem* pixItem):
        _initRadius(radius), _radius(radius), _spointNb(0), _pixItem(pixItem){
    _circle = QList<SPoint*>();
    _center = QPointF(cX, cY);

}
SCircle::~SCircle(){
    for(int i = 0; i < _spointNb; i++){
        delete _circle.value(i);
    }
}

QPointF* SCircle::getQPoint(int i){
    if(i < 0 || i >= _spointNb)
        return NULL;
    return &_circle.value(i)->carthPos();
}

QPointF SCircle::getQPointRotate(int i, qreal angle, qreal scale){
    if(i < 0 || i >= _spointNb)
        return QPointF(0, 0);
    return _circle.value(i)->posRotate(_spointNb, _center , angle, scale) ;
}

SPoint* SCircle::getSPoint(int i){
    if(i < 0 || i >= _spointNb)
        return NULL;
    return _circle.value(i);
}

void SCircle::translateCenter(qreal dx, qreal dy){
    _center.setX(_center.x() + dx);
    _center.setY(_center.y() + dy);
    for(int i = 0; i < _spointNb; i++){
        _circle.value(i)->radToCarth(_spointNb, _center, _radius);
    }
}

void SCircle::setRadius(qreal radius){
    _radius = radius;
    for(int i = 0; i < _spointNb; i++){
        if(!_circle.value(i)->isRadiusFixed())
                _circle.value(i)->radToCarth(_spointNb, _center, _radius);
    }
    if(_pixItem != NULL)
        _pixItem->scene()->update();
}

bool SCircle::isAllPointsFixed(){
    bool res = true;
    for(int i = 0; i < _spointNb && res; i++){
        if(!_circle.value(i)->isRadiusFixed())
            res = false;
    }
    return res;
}

void SCircle::setParam(qreal cX, qreal cY, qreal radius){
    _center = QPointF(cX, cY);
    _radius = radius;
    for(int i = 0; i < _spointNb; i++){
        _circle.value(i)->radToCarth(_spointNb, _center, _radius);
        _circle.value(i)->setRadiusFixed(false);
        _circle.value(i)->setGrayValue(-1);
    }
}

int SCircle::firstPoint(qreal angle, qreal offsetX, qreal offsetY, qreal scale, QPointF* fPoint){
    fPoint->setX(0);
    fPoint->setY(0);
    int p = -1;
    QPointF offsetI = _pixItem->offset();
    QPointF fp = this->getQPointRotate(0, angle, scale) + offsetI;
    if(fp.y() >= offsetY || fp.x() <= offsetX){
        for(int i = 1; i < _spointNb && p == -1; i++){
            QPointF cp = this->getQPointRotate(i, angle, scale) + offsetI;
            if(cp.x() > offsetX || cp.y() < offsetY){
                p = i;
                int intersect = QLineF(offsetX, 0, offsetX, offsetY * 2).intersect(QLineF(this->getQPointRotate(i - 1, angle, scale) + offsetI,
                                                                 cp),
                                                          fPoint);
                if(intersect != QLineF::BoundedIntersection){
                    intersect = QLineF(0, offsetY, offsetY * 10, offsetY).intersect(QLineF(this->getQPointRotate(i - 1, angle, scale) + offsetI,
                                                                 cp),
                                                          fPoint);
                    if(intersect != QLineF::BoundedIntersection){
                        p = -1;
                    }
                }
            }
        }
    }else{
        for(int i = _spointNb - 1; i > 0 && p == -1; i--){
            QPointF cp = this->getQPointRotate(i, angle, scale) + offsetI;
            if(cp.x() <= offsetX || cp.y() >= offsetY){
                p = i + 1;
                int intersect = QLineF(offsetX, 0, offsetX, offsetY * 2).intersect(QLineF(cp,
                                                                 this->getQPointRotate(i + 1, angle, scale) + offsetI),
                                                          fPoint);
                if(intersect != QLineF::BoundedIntersection){
                    intersect = QLineF(0, offsetY, offsetX * 10, offsetY).intersect(QLineF(cp,
                                                                 this->getQPointRotate(i + 1, angle, scale) + offsetI),
                                                          fPoint);
                    if(intersect != QLineF::BoundedIntersection)
                        p = -1;
                }
            }
        }
    }
    if(p != -1){
        QPointF resPoint = this->getQPointRotate(p, angle, scale) + offsetI;
        if(resPoint.y() >= offsetY || resPoint.x() <= offsetX)
            p = -1;
    }
    return p;
}


void SCircle::addSPoint(){
    SPoint* sp = new SPoint(_spointNb, _spointNb + 1, _center, _radius);
    _circle.push_back(sp);
    for(int i = 0; i < _spointNb; i++){
        _circle.value(i)->radToCarth(_spointNb + 1, _center, _radius);
    }
    _spointNb++;
}

void SCircle::addSPoint(int nb){
    for(int i = 0 ; i < nb; i++){
        addSPoint();
    }
}

void SCircle::removeSPoint(){
    SPoint* sp = _circle.last();
    _circle.removeOne(sp);
    delete sp;
    for(int i = 0; i < _spointNb - 1; i++){
        _circle.value(i)->radToCarth(_spointNb - 1, _center, _radius);
    }
    _spointNb--;
}

void SCircle::removeSPoint(int nb){
    for(int i = 0; i < nb && _spointNb > 0; i++){
        removeSPoint();
    }
}

void SCircle::changeSPointNb(int nb){
    if (nb > _spointNb)
        this->addSPoint(nb - _spointNb);
    if ( nb < _spointNb)
        this->removeSPoint(_spointNb - nb);
}

void SCircle::reinitialize(){
    _radius = _initRadius;
    for(int i = 0; i < _spointNb; i++){
        _circle.value(i)->setRadiusFixed(false);
        _circle.value(i)->radToCarth(_spointNb, _center, _radius);
        _circle.value(i)->setGrayValue(-1);
    }
}
