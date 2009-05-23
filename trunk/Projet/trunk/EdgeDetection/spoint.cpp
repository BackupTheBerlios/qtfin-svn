/*
 * SPoint.cpp
 *
 *  Created on: 1 mars 2009
 *      Author: Nicolas
 */

#include "spoint.h"
#include <stdio.h>
#include <math.h>
#include <iostream>

#define PI 3.14159265358979323846

using namespace std;

int SPoint::almost(float f){
    if(f - (int)f > .5)
        return (int)f + 1;
    else
        return (int)f;
}

SPoint::SPoint(int radPos, int N, const QPointF& center, qreal radius):
        _radPos(radPos), _radius(radius), _grayValue(-1), _rayonFixe(false){
    _carthPos = QPointF();
    radToCarth(N, center, radius);
}


void SPoint::radToCarth(int N, const QPointF& center, qreal radius){
    _carthPos.setX( center.x() + radius * cos(2 * PI * _radPos / N));
    _carthPos.setY( center.y() + radius * sin(2 * PI * _radPos / N));
    _radius = radius;
}

QPointF SPoint::posRotate(int N, const QPointF& center, qreal angle){
    return QPointF(center.x() + _radius * cos(2 * PI * _radPos / N + angle * PI / 180),
                  center.y() + _radius * sin(2 * PI * _radPos / N + angle * PI / 180));
}

