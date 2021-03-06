#ifndef EXTREMITYPOINT_H
#define EXTREMITYPOINT_H

#include "boundingpoint.h"

class BoundingPoint;

class ExtremityPoint: public BoundingPoint{

public:
    /**
    * Defines the type of the item as an int (by default,
    * all the others items have the value UserType as type, except
    * the BoundingPoint)
    **/
    enum{Type = BoundingPoint::Type + 1};

    /**
    * Constructs an extremity point of the monofin only by calling the
    * constructor of BoundingPoint with the same arguments.
    *@param coord the QPointF which will give the coordinates (x,y)
    * to create the extremity point
    *@param scene a pointer to the Painting scene
    * in which the point is placed
    **/
    ExtremityPoint(const QPointF& coord, PaintingScene* scene);

    /**
    * Makes the extremity point moving to the coordinates of the given QPointF,
    * but ONLY along the extremity axis of the scene in the zone where points
    * can move (see PaintingScene::pointsBoundingZone). If the magnet is
    * activated in the scene, the function can use it to move the point only
    * on the intersections of the grid, depending on the parameter.
    * It also calls the function move() of the lines if they exist.
    *@param p the new coordinates of the bounding point as a QPointF
    *@param useMagnetIfActivated if true, the point will be placed on the
    * intersections of the grid, but ONLY if the magnet is activated on the
    * scene ; if false, the point is placed just as described before
    **/
    void moveTo(const QPointF& p, bool useMagnetIfActivated = true);

    /**
    * Returns the type of the extremity point as an int. This int is different
    * of UserType. It is used by the scene to make a difference between
    * a BoundingPoint and an ExtremityPoint.
    *@return the type of the item as an int
    **/
    int type() const{return Type;}

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
};

#endif // EXTREMITYPOINT_H
