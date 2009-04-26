/*
 * SPoint.h
 *
 *  Created on: 1 mars 2009
 *      Author: Nicolas
 */

#ifndef SPOINT_H
#define SPOINT_H


#include <QPoint>

class SPoint{

private:
    int _radPos;
    QPoint _carthPos;
    int _radius;

    int _grayValue;
    bool _rayonFixe;

    int almost(float f);

public:

    SPoint(int radPos = 0, int N = 0, const QPoint& center = QPoint(0,0), int rayon = 0);

    int getRadPos(){
        return _radPos;
    }

    QPoint& getCarthPos(){
        return _carthPos;
    }

    int getGrayValue(){
        return _grayValue;
    }

    void setGrayValue(int v){
        _grayValue = v;
    }

    bool isRadiusFixed(){
        return _rayonFixe;
    }

    void setRadiusFixed(bool v){
        _rayonFixe = v;
    }

    void radToCarth(int N, const QPoint& center, int rayon);

    QPoint posRotate(int N, const QPoint& center, qreal angle);
};

#endif /* SPOINT_H_ */
