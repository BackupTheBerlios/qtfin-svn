#ifndef EXTREMITYPOINT_H
#define EXTREMITYPOINT_H

#include "boundingpoint.h"

class BoundingPoint;

class ExtremityPoint: public BoundingPoint{

public:
    enum{Type = BoundingPoint::Type + 1};

    ExtremityPoint(const QPointF& coord, PaintingScene* scene);
    int type() const{return Type;}

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
};

#endif // EXTREMITYPOINT_H
