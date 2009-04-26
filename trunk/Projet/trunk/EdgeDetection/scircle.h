/*
 * SCircle.h
 *
 *  Created on: 3 mars 2009
 *      Author: Nicolas
 */

#ifndef SCIRCLE_H_
#define SCIRCLE_H_

#include <QList>
#include <QPoint>
#include "pixmapitem.h"
#include "spoint.h"

class PixmapItem;

class SCircle{

private:
    QList<SPoint*> _circle;
    QPoint _center;
    int _radius;
    int _spointNb;

    PixmapItem* _pixItem;

public:
    SCircle(int cX, int cY, int radius = 0, PixmapItem* pixItem = NULL);
    ~SCircle();

    QPoint* getQPoint(int i);
    SPoint* getSPoint(int i);
    QPoint getQPointRotate(int i, qreal angle);

    int getRadius(){
        return _radius;
    }
    int getSPointNb(){
        return _spointNb;
    }

    void translateCenter(int x, int y);
    void setRadius(int radius);
    void setParam(int cX, int cY, int radius);

    bool isAllPointsFixed();

    void addSPoint();
    void addSPoint(int nb);
    void removeSPoint();
    void removeSPoint(int nb);
    void changeSPointNb(int nb);

};


#endif /* SCIRCLE_H_ */
