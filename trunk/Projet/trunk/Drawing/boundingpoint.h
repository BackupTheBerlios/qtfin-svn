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
class ControlPoint;

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
    *@param scene a pointer to the Painting scene in which the point is placed
    **/
    BoundingPoint(qreal x, qreal y, PaintingScene* scene);

    /**
    * Constructs a bounding point with the coordinates of a QPointF.
    * When constructed, the point has no right or left lines, the lines
    * must be set with setLeftLine or setRightLine.
    *@param coord the QPointF which will give the coordinates (x,y)
    * to create the bounding point
    *@param scene a pointer to the Painting scene in which the point is placed
    **/
    BoundingPoint(const QPointF& coord, PaintingScene* scene);

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
    *@return true if the left line of the point has been set ;
    * otherwise return false
    **/
    bool hasLeftLine(){return _hasLeftLine;}

    /**
    *@return true if the right line of the point has been set ;
    * otherwise return false
    **/
    bool hasRightLine(){return _hasRightLine;}

    /**
    * Function used when a line is deleted : the line first hide itself to
    * the point before being deleted. The pointer _leftLine will probably
    * become null after the use of this function, but the boolean _hasLeftLine
    * or is set to false.
    **/
    void hideLeftLine(){_hasLeftLine = false;}

    /**
    * Function used when a line is deleted : the line first hide itself to
    * the point before being deleted. The pointer _rightLine will probably
    * become null after the use of this function, but the boolean _hasRightLine
    * or is set to false.
    **/
    void hideRightLine(){_hasRightLine = false;}

    /**
    * Returns the key of the point in the internal structure of the monofin.
    * Make sure the internal key has been set by setInternalKey before,
    * or verify that the value is not the default value :
    * Data::MONOFIN_SURFACE_NOT_CREATED_POINT.
    *@return the internal key of the point
    **/
    int internalKey(){return _internalKey;}

    /**
    *@return true if the new given x-position is in the zone where the
    * bounding points are allowed to move ; otherwise return false
    **/
    bool isAllowedToMoveAtPosX(qreal posX);

    /**
    *@return true if the new given y-position is in the zone where the
    * bounding points are allowed to move ; otherwise return false
    **/
    bool isAllowedToMoveAtPosY(qreal posY);

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
    *@return a pointer to the left line of the point if it has one,
     else return 0
    **/
    BrLine* leftLine(){
        if(this->_hasLeftLine){return _leftLine;}
        else{return 0;}
    }

    /**
    * Makes the bounding point moving to the coordinates of the given QPointF,
    * but ONLY in the zone of the scene where points can move (see
    * PaintingScene::pointsBoundingZone). If the magnet is activated in the
    * scene, the function can use it to move the point only on the
    * intersections of the grid, depending on the parameter.
    * It also calls the function move() of the lines if they exist.
    *@param p the new coordinates of the bounding point as a QPointF
    *@param useMagnetIfActivated if true, the point will be placed on the
    * intersections of the grid, but ONLY if the magnet is activated on the
    * scene ; if false, the point is placed just as described before
    **/
    void moveTo(const QPointF& p, bool useMagnetIfActivated = true);

    /**
    * Compares the given pointer with the pointers of the two lines of the
    * point if they exist. If the given line is one of those of the point, the function
    * returns the other if it exists. If the given line is not linked to this
    * point, or if the point has only one line, IT WILL RETURN 0.
    *@param l a pointer to a line linked to this point
    *@return a pointer to the other line linked to this point as the given one
    **/
    BrLine* otherLineAs(BrLine* l);

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
    *@return the size of the rectangle painted in the view
    **/
    inline qreal rectangleSize() const{
        return BOUNDINGPOINTSIZE;// / _scene->scaleFactor();
    }


    /**
    * Hides the left line to the bounding point (set the boolean to false)
    * and calls the function removeLine of the scene with the left line.
    **/
    void removeLeftLine();
    
    /**
    * Hides the right line to the bounding point (set the boolean to false)
    * and calls the function removeLine of the scene with the right line.
    **/
    void removeRightLine();

    /**
    *@return a pointer to the right line of the point if it has one,
     else return 0
    **/
    BrLine* rightLine(){
        if(this->_hasRightLine){return _rightLine;}
        else{return 0;}
    }

    /**
    * Allows or not the point to move. This function is called by the scene
    * when the user activates or deactivates certain features.
    *@param c a boolean to say if the point is allowed to move
    * (true : it allows it to move ; false : it forbids it to move)
    **/
    void setCanMove(bool c){_canMove = c;}

    /**
    * Changes the color in which the point is painted when it is highlighted.
    * Note that the color must be valid, or it will do nothing.
    *@param color a reference to the new valid color of the point when it
    * is highlighted
    **/
    void setColorWhenHighlighted(const QColor& color);

    /**
    * Changes the color in which the point is painted when it is not
    * highlighted or selected.
    * Note that the color must be valid, or it will do nothing.
    *@param color a reference to the new valid color of the point when it
    * is not highlighted or selected
    **/
    void setColorWhenNormal(const QColor& color);

    /**
    * Changes the color in which the point is painted when it is selected.
    * Note that the color must be valid, or it will do nothing.
    *@param color a reference to the new valid color of the point when it
    * is selected
    **/
    void setColorWhenSelected(const QColor& color);

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
    * Indicates that the mouse is in a short zone around the bounding point
    * or not anymore.
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

    /*bool willMoveToGoCloseToPosX(qreal posX);
    bool willMoveToGoCloseToPosY(qreal posY);*/

protected:
    /**
    * Receives the mouse move event and place the point under the mouse if it
    * is allowed to move. It also moves all the others selected points, and
    * the concerned control points (see the mousePressEvent).
    * See mouseMoveEvent of the QGraphicsItem.
    **/
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);

    /**
    * Receives the mouse press event, and if the user has clicked with the
    * left button, it decides if the point is allowed to move or not. If it is,
    * it gets back from the scene the list of the control points which will
    * move with this point.
    * See mousePressEvent of the QGraphicsItem
    **/
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);


    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);
    //void dragEnterEvent(QGraphicsSceneDragDropEvent *event);



//ATTRIBUTES

    bool _atLeastOneClick;  //if true, at least one mousePressEvent
                            //with a left click has been received

    bool _canMove;  //used to determine if the item is allowed to move or not

    QColor _colorWhenNormal;  //the color used by the QPainter to draw the
                            //point when it is not selected or highlighted

    QColor _colorWhenSelected; //color used by the QPainter to draw the point
                               //when it is selected

    QColor _colorWhenHighlighted; //color used by the QPainter to draw the point
                                  //when it is highlighted

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

    QList<ControlPoint*> _movingControlPoints; //a list to remember which
                        //control points are moving with the bounding point

    QPointF* _pos;  //The coordinates of the point

    BrLine* _rightLine;  //a pointer to the right BrLine of the point

    PaintingScene* _scene;  //a pointer to the main scene to call
                            //some useful functions

};
#endif // BOUNDINGPOINT_H
