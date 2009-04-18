#ifndef BOUNDINGPOINT_H
#define BOUNDINGPOINT_H

#include <QGraphicsItem>
#include <QRectF>
#include <QPointF>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsScene>
#include <QColor>
#include <QRadialGradient>
#include <QBrush>
#include <QMimeData>

class BrLine;
class PaintingScene;

class BoundingPoint: public QGraphicsItem{

#define BOUNDINGPOINTSIZE 8

public:
    /**
    * Defines the type of the item as an int (by default,
    * all the others items have the value UserType as type, except
    * the ExtremityPoint)
    **/
    enum{Type = UserType + 1};

    /**
    * Constructs a bounding point with the given coordinates.
    * When constructed, the point has no right or left lines, the lines
    * must be set with setLeftLine or setRightLine.
    *@param x the coordinate x of the point
    *@param y the coordinate y of the point
    *@param scene a pointer to the Painting scene
    * in which the point is placed
    **/
    BoundingPoint(qreal x, qreal y, PaintingScene* scene);

    /**
    * Constructs a bounding point with the coordinates of a QPointF.
    * When constructed, the point has no right or left lines, the lines
    * must be set with setLeftLine or setRightLine.
    *@param coord the QPointF which will give the coordinates (x,y)
    * to create the bounding point
    *@param scene a pointer to the Painting scene
    * in which the point is placed
    **/
    BoundingPoint(const QPointF& coord, PaintingScene* scene);

    /**
    * Constructor by copy, useless.
    **/
    BoundingPoint(const BoundingPoint& p);

    /**
    * Destroy the object QPointF _pos, but do not destroy the lines
    **/
    ~BoundingPoint();


    /**
    * Move the bounding point to align the tangents of the two lines around it.
    * It also updates the view.
    **/
    void alignTangents();

    /**
    * Returns the bounding rectangle of the item : it's here a rectangle
    * a bit larger than the painted rectangle on the view.
    *@return the bounding rectangle of the item
    **/
    virtual QRectF boundingRect() const;

    /**
    * Returns the coordinates of the bounding point with a float precision.
    *@return the coordinates of the point as a QPointF
    **/
    QPointF coord() const{return *_pos;}

    /**
    * Functions used when a line is deleted : the line first
    * hide itself to the point before being deleted.
    * The pointer _leftLine or _rightLine will probably become null
    * after the use of this function, but the boolean
    * _hasLeftLine or _hasRightLine is set to false.
    **/
    void hideLeftLine(){_hasLeftLine = false;}
    void hideRightLine(){_hasRightLine = false;}

    /**
    * Returns the key of the point in the internal structure of the monofin.
    * Make sure the internal key has been set by setInternalKey before,
    * or verify that the value is not the default value : PaintingScene::BADKEY.
    *@return the internal key of the point
    **/
    int internalKey(){return _internalKey;}

    /**
    * Returns the key of the left line of the point or PaintingScene::BADKEY
    * if it has no left line.
    *@return the internal key of the left line of the point
    **/
    int internalKeyLeftLine();

    /**
    * If it's true, it means that the mouse is in a short zone
    * around the coordinates of the point and that the color of the
    * bounding point has changed in the view (blue by default).
    * This function only works if the scene has indicated to the point
    * that the mouse is on it by calling setMouseOnPoint.
    *@return true if the mouse is on the bounding point ;
    * otherwise, returns false.
    **/
    bool isMouseOnPoint(){return _isMouseOnPoint;}

    /**
    * Makes the bounding point moving to the coordinates of the given QPointF.
    * It also calls the function move() of the lines if they exist.
    * @param p the new coordinates of the bounding point as a QPointF
    **/
    void moveTo(const QPointF& p);

    /**
    * Paints the item as a black rectangle when it's not under the mouse
    * or not selected. When it's under the mouse, it's painted in blue, and
    * when it's selected, it's painted in red.
    * This function is called by Qt when needed, or by others functions with
    * the method update() of the scene.
    *@param painter the painter which will be used to draw the point
    *@param option not used here, see the documentation of QGraphicsItem
    *@param widget not used here, see the documentation of QGraphicsItem
    **/
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

    /**
    * Returns the rectangle stored by the bounding point.
    * The ghost point uses this function to have the same rectangle
    * as that painted by the bounding point.
    *@return the rectangle around the point
    **/
    QRectF rect(){return QRectF(_pos->x() - (BOUNDINGPOINTSIZE / 2.0),
               _pos->y() - (BOUNDINGPOINTSIZE / 2.0),
               BOUNDINGPOINTSIZE,
               BOUNDINGPOINTSIZE);}

    /**
    * Hides the line to the bounding point (set the boolean to false)
    * and calls the function removeLine of the scene.
    **/
    void removeLeftLine();
    void removeRightLine();

    /**
    * Allows or not the point to move. This function is called by the scene
    * when the user activates or deactivates certain features.
    *@param c a boolean to say if the point is allowed to move
    * (true : it allows it to move ; false : it firbids it to move)
    **/
    void setCanMove(bool c){_canMove = c;}

    /**
    * Sets the internal key of the point with the given key
    * which comes from the internal strucure of the monofin.
    *@param key the new key of the point which has to come from the structure
    **/
    void setInternalKey(int key){_internalKey = key;}

    /**
    * Activates the left line of the point with the given BrLine :
    * it sets the pointer to the given line and set the _hasLeftLine
    * boolean to true
    *@param l the line which will be the left line of the bounding point
    **/
    void setLeftLine(BrLine* l);

    /**
    * Function called by the scene to indicates that the mouse is in a
    * short zone around the bounding point or not anymore.
    *@param on true means that the point will consider that the mouse is on it,
    * false means that the point will no longer consider
    * that the mouse is on it, or it will do nothing if it was already on false
    **/
    void setMouseOnPoint(bool on);

    /**
    * Activates the right line of the point with the given BrLine :
    * it sets the pointer to the given line and set the _hasRightLine
    * boolean to true
    *@param l the line which will be the right line of the bounding point
    **/
    void setRightLine(BrLine* l);

    /**
    * Returns the type of the bounding point as an int. This int is different
    * of UserType. It is used by the scene to make a difference between
    * a BoundingPoint and an ExtremityPoint.
    *@return the type of the item as an int
    **/
    int type() const{return Type;}


protected:
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
    //virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
    //virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);
    //void dragEnterEvent(QGraphicsSceneDragDropEvent *event);

    /**
    *@return the size of the rectangle painted in the view
    **/
    inline qreal rectangleSize() const;


//ATTRIBUTES

    bool _atLeastOneClick;  //if true, at least one mousePressEvent
                            //with a left click has been received

    bool _canMove;  //used to determine if the item is allowed to move or not

    QColor _color;  //the color used by the QPainter to draw the point

    bool _hasLeftLine;  //a control boolean to be sure that the left line
                        //has been defined

    bool _hasRightLine; //a control boolean to be sure that the right line
                        //has been defined

    int _internalKey;  //the key of the point in the internal
                       //structure of the monofin

    bool _isMouseOnPoint; //modified by the scene to indicate if the mouse
                          //is near the point or not

    bool _isMoving;  //boolean used to determine if the point has changed its
                     //position between a mousePressEvent and a
                     //mouseReleaseEvent : if true, the position has changed

    BrLine* _leftLine;  //a pointer to the left BrLine of the point

    QPointF* _pos;  //The coordinates of the point

    BrLine* _rightLine;  //a pointer to the right BrLine of the point

    PaintingScene* _scene;  //a pointer to the main scene to call
                            //some useful functions

};
#endif // BOUNDINGPOINT_H
