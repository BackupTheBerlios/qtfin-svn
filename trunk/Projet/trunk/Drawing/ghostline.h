#ifndef GHOSTLINE_H
#define GHOSTLINE_H

#include <QGraphicsItem>
#include <QRectF>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>

class PaintingScene;
class BoundingPoint;
class GhostPoint;

class GhostLine: public QGraphicsItem{

public:
    /**
    * Constructs a straight ghost line between the given
    * bounding point and ghost point.
    *@param p1 a pointer to the bounding point which is the start of the line
    *@param p2 a pointer to the ghost point which is the end of the line
    *@param scene a pointer to the main PaintingScene
    **/
    GhostLine(BoundingPoint* p1, GhostPoint* p2, PaintingScene* scene);


    /**
    * Gives the bounding rectangle of the line, the top left point of which
    * is the left point of the line and the bottom right point the right
    * point of the line.
    *@return the bounding rectangle of the item
    **/
    virtual QRectF boundingRect() const;

    /**
    * Returns a boolean to indicate if the ghost line intersects or not with
    * another line. This boolean is set by the scene by calling setCanBePlaced.
    *@return true if the line does not intersect with other lines,
    * and false if it intersects
    **/
    bool canBePlaced(){return _canBePlaced;}

    /**
    * Paints the ghost line as a straight black line with transparency and
    * paints its symetric too. If the boolean _canBePlaced is false, the two
    * lines (included the symetric) are painted in red.
    *@param painter the painter which will be used to draw the line
    *@param option not used here, see the documentation of QGraphicsItem
    *@param widget not used here, see the documentation of QGraphicsItem
    **/
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

    /**
    * Sets the boolean _canBePlaced to the given boolean. This function is
    * called by the scene when the ghost line intersects with a BrLine
    * or does not intersects anymore.
    *@param p true means that the line will consider that it intersects with
    * another line and it will be painted in red ; false means that the line
    * will no longer consider that there is an intersection and
    * it will be painted in black
    **/
    void setCanBePlaced(bool p){_canBePlaced = p;}

    /**
    * Sets the left point of the line to the given bounding point.
    * This function is called when a bounding point is placed on the scene ;
    * this bounding point becomes the new left point of the ghost line.
    **/
    void setP1(BoundingPoint* p1){_p1 = p1;}

protected:

//ATTRIBUTES

    bool _canBePlaced;  //if true, it means that the ghost line does not
                        //intersect with another line in the scene

    BoundingPoint* _p1;  //a pointer to the left point of the line,
                         //which is a bounding point

    GhostPoint* _p2;  //a pointer to the right point of the line,
                      //which is a ghost point

    PaintingScene* _scene; //a pointer to the main scene



};

#endif // GHOSTLINE_H
