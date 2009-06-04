#ifndef CONTROLPOINT_H
#define CONTROLPOINT_H

#include <QGraphicsItem>
#include <QRectF>
#include <QPointF>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneHoverEvent>
#include <QGraphicsScene>
#include <QColor>
#include "tangent.h"

class BrLine;
class PaintingScene;

class ControlPoint: public QGraphicsItem{

    //Q_OBJECT

#define CONTRPOINTHEIGHT 9

public:

    /**
    * Constructs a control point with the given coordinates. It belongs to
    * the given BrLine.
    * When constructed, the point does not know the two tangents which will
    * move with it, they must be set by setTangent1() and setTangent2().
    *@param x the coordinate x of the point
    *@param y the coordinate y of the point
    *@param line a pointer to the bezier curve which has this control point
    *@param scene a pointer to the Painting scene
    **/
    ControlPoint(qreal x, qreal y, BrLine* line, PaintingScene* scene);

    /**
    * Constructs a control point with the coordinates of the given QPointF.
    * This control point belongs to the given BrLine.
    * When constructed, the point does not know the two tangents which will
    * move with it, they must be set by setTangent1() and setTangent2().
    *@param coord the coordinate of the point as a QPointF
    *@param line a pointer to the bezier curve which has this control point
    *@param scene a pointer to the Painting scene
    **/
    ControlPoint(const QPointF& coord, BrLine* line, PaintingScene* scene);

    /**
    * Destroys the QPointF _pos
    **/
    ~ControlPoint();


    /**
    * Returns the bounding rectangle of the item : it's here the same
    * rectangle as the one painted in the view.
    *@return the bounding rectangle of the item
    **/
    virtual QRectF boundingRect() const{return QRectF(_pos->x() - (CONTRPOINTHEIGHT / 2.0),
                                                      _pos->y() - (CONTRPOINTHEIGHT / 2.0),
                                                      CONTRPOINTHEIGHT,
                                                      CONTRPOINTHEIGHT);}

    /**
    * Returns the coordinates of the control point with a float precision.
    *@return the coordinates of the point as a QPointF
    **/
    QPointF coord() const{return *_pos;}

    /**
    * Returns the key of the point in the internal structure of the monofin.
    * Make sure the internal key has been set by setInternalKey before,
    * or verify that the value is not the default value :
    * Data::MONOFIN_SURFACE_NO_CONTROL_POINT.
    *@return the internal key of the point
    **/
    int internalKey() const{return _internalKey;}

    bool isAllowedToMoveAtPosX(qreal posX);
    bool isAllowedToMoveAtPosY(qreal posY);

    /**
    * Makes the control point moving to the coordinates of the given QPointF,
    * but ONLY in the zone where control points can move, which is the scene
    * rectangle. It also updates the position of the two tangents.
    *@param p the new coordinates of the control point as a QPointF
    *@param moveOnlyInScene if true, it means that the point will place itself
    * at the extremity of the scene rectangle if the new coordinates are
    * outside (it can move only inside this rectangle), this is the default
    * option ; set this option to false can be useful if you do not know the
    * size of the entire edge and if you want to adjust the size of the scene
    * rectangle after this operation
    **/
    void moveTo(const QPointF& p, bool moveOnlyInScene = true);

    /**
    * Paints the item as a black circle when it's not under the mouse
    * When it's under the mouse, it's painted in green.
    * This function is called by Qt when needed, or by others functions with
    * the method update() of the scene.
    *@param painter the painter which will be used to draw the point
    *@param option not used here, see the documentation of QGraphicsItem
    *@param widget not used here, see the documentation of QGraphicsItem
    **/
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

    /**
    *@return the size of the bounding rectangle of the circle painted in
    * the view
    **/
    qreal rectangleSize() const{return CONTRPOINTHEIGHT;}

    /**
    * Changes the color in which the point is painted when it is highlighted.
    * Note that the color must be valid, or it will do nothing.
    *@param color a reference to the new valid color of the point when it
    * is highlighted
    **/
    void setColorWhenHighlighted(const QColor& color);

    /**
    * Changes the color in which the point is painted when it is
    * not highlighted.
    * Note that the color must be valid, or it will do nothing.
    *@param color a reference to the new valid color of the point when it
    * is not highlighted
    **/
    void setColorWhenNormal(const QColor& color);

    /**
    * Sets the internal key of the point with the given key
    * which comes from the internal strucure of the monofin.
    *@param key the new key of the point which has to come from the structure
    **/
    void setInternalKey(int key){_internalKey = key;}

    /**
    * Sets the first tangent to the given tangent and set the
    * control boolean _hasFirstTangent to true.
    *@param t a pointer to the first tangent controlled by the control point
    **/
    void setTangent1(Tangent* t);

    /**
    * Sets the second tangent to the given tangent and set the
    * control boolean _hasSecondTangent to true.
    *@param t a pointer to the second tangent controlled by the control point
    **/
    void setTangent2(Tangent* t);

    bool willMoveToGoCloseToPosX(qreal posX);
    bool willMoveToGoCloseToPosY(qreal posY);

protected:
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);


//ATTRIBUTES

    bool _canMove;  //if true, the point can move when it receives
                    //a mouseMoveEvent ; if false, it can not

    QColor _colorWhenHighlighted;  //the color used by the QPainter to draw
                                   //the point when it is highlighted

    QColor _colorWhenNormal; //color used by the QPainter to draw the point
                             //when it is not highlighted

    bool _hasFirstTangent;  //a control boolean, to be sure that the first
                            //tangent has been set, before using the pointer

    bool _hasSecondTangent;  //a control boolean, to be sure that the second
                             //tangent has been set, before using the pointer

    int _internalKey;  //the key of the point in the internal
                       //structure of the monofin

    bool _isHighlighted; //if true, it means that the mouse is near
                         //or on the point

    bool _isMoving;  //boolean used to determine if the point has changed its
                     //position between a mousePressEvent and a
                     //mouseReleaseEvent : if true, the position has changed

    BrLine* _line;  //a pointer to the line controlled by the control point

    QPointF* _pos;  //The coordinates of the point

    PaintingScene* _scene;  //a pointer to the main scene

    Tangent* _tangent1;  //a pointer to the left tangent which
                         //extremity is this control point

    Tangent* _tangent2;  //a pointer to the right tangent which
                         //extremity is this control point

};

#endif // CONTROLPOINT_H
