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

SPoint::SPoint(int radPos, int N, const QPoint& center, int rayon):
        _radPos(radPos), _radius(rayon), _grayValue(-1), _rayonFixe(false){
    _carthPos = QPoint();
    radToCarth(N, center, rayon);
}


void SPoint::radToCarth(int N, const QPoint& center, int rayon){
    _carthPos.setX( almost(center.x() + rayon * cos(2 * PI * _radPos / N)));
    _carthPos.setY( almost(center.y() + rayon * sin(2 * PI * _radPos / N)));
    _radius = rayon;
}

QPoint SPoint::posRotate(int N, const QPoint& center, qreal angle){
    return QPoint(almost(center.x() + _radius * cos(2 * PI * _radPos / N + angle * PI / 180)),
                  almost(center.y() + _radius * sin(2 * PI * _radPos / N + angle * PI / 180)));
}

