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

SCircle::SCircle(int cX, int cY, int radius, PixmapItem* pixItem):
        _radius(radius), _spointNb(0), _pixItem(pixItem){
    _circle = QList<SPoint*>();
    _center = QPoint(cX, cY);

}
SCircle::~SCircle(){
    for(int i = 0; i < _spointNb; i++){
        delete _circle.value(i);
    }
}

QPoint* SCircle::getQPoint(int i){
    if(i < 0 || i >= _spointNb)
        return NULL;
    return &_circle.value(i)->getCarthPos();
}

QPoint SCircle::getQPointRotate(int i, qreal angle){
    if(i < 0 || i >= _spointNb)
        return QPoint(0, 0);
    return _circle.value(i)->posRotate(_spointNb, _center, angle) ;
}

SPoint* SCircle::getSPoint(int i){
    if(i < 0 || i >= _spointNb)
        return NULL;
    return _circle.value(i);
}

void SCircle::translateCenter(int x, int y){
    _center.setX(_center.x() + x);
    _center.setY(_center.y() + y);
    for(int i = 0; i < _spointNb; i++){
        _circle.value(i)->radToCarth(_spointNb, _center, _radius);
    }
}

void SCircle::setRadius(int radius){
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

void SCircle::setParam(int cX, int cY, int radius){
    _center = QPoint(cX, cY);
    _radius = radius;
    for(int i = 0; i < _spointNb; i++){
        _circle.value(i)->radToCarth(_spointNb, _center, _radius);
        _circle.value(i)->setRadiusFixed(false);
        _circle.value(i)->setGrayValue(-1);
    }
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
