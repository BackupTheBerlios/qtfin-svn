#ifndef GHOSTPOINT_H
#define GHOSTPOINT_H

#include <QGraphicsItem>
#include "boundingpoint.h"

class BoundingPoint;
class PaintingScene;

class GhostPoint: public QGraphicsItem{

public:

    /**
    * Contructs a ghost point at the given coordinates with a new bounding
    * point as attribute.
    *@param coord the coordinates to place the point as a QPointF
    *@param scene a pointer to the main PaintingScene
    **/
    GhostPoint(QPointF coord, PaintingScene* scene);

    /**
    * Destroys the bounding point attribute.
    **/
    ~GhostPoint();


    /**
    * Returns the rectangle of a bounding point as bounding rectangle.
    *@return the bounding rectangle of the item
    **/
    virtual QRectF boundingRect() const{return _p->rect();}

    /**
    * Returns a boolean to indicate if a bounding point can be placed at
    * the position of the ghost point. This boolean is set by the scene by
    * calling setCanBePlaced.
    *@return true means that the ghost line does not intersect with any other
    * line ; false means that the ghost line intersects with another line
    **/
    bool canBePlaced(){return _canBePlaced;}

    /**
    *@return the coordinates of the ghost point as a QPointF
    **/
    QPointF coord(){return _p->coord();}

    /**
    * Moves the ghost point to the coordinates of the given point.
    *@param p the new coordinates of the ghost point as a QPointF
    **/
    void moveTo(const QPointF& p){_p->moveTo(p);}

    /**
    * Paints the point as a transparent rectangle, which is the same as
    * a rectangle painted for a bounding point. If the boolean _canBePlaced
    * is false, it is painted in red and if it can be placed,
    * the painter has to paint it black (sorry...).
    **/
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

    /**
    * Sets the boolean _canBePlaced to the given boolean. This function is
    * called by the scene to indicate if a bounding point can be placed on
    * the coordinates of the ghost point or not.
    *@param p true means that the point will consider that a bounding point
     can be placed on its coordinates and will be painted in black ;
     false means that it will be painted in red
    **/
    void setCanBePlaced(bool p){_canBePlaced = p;}

protected:

//ATTRIBUTES

    bool _canBePlaced;  //if true, it means that a bounding point can be
                //created on the scene at the position of this ghost point

    BoundingPoint* _p;  //a bounding point used to be painted ; it is useful
                        //because a ghost point should have the
                        //same appearance as a bounding point.
            //This bounding point also keeps the position of the ghost point

    PaintingScene* _scene; //a pointer to the main scene

};

#endif // GHOSTPOINT_H
