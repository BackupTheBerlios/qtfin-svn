#ifndef BRLINE_H
#define BRLINE_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QLineF>
#include <QPainterPath>
#include <QColor>
#include <QPen>
#include "boundingpoint.h"
#include "controlpoint.h"
#include "paintingscene.h"
#include "tangent.h"

class PaintingScene;
class BoundingPoint;
class ControlPoint;

class BrLine: public QGraphicsItem{

    //Q_OBJECT

public:

    /**
    * Constructs a straight line between the two given bounding points. The id
    * is the index of the line in the list in the scene. The internal key is
    * set to PaintingScene::BADKEY ; to set it, use setInternalKey(key).
    * A control point and two tangents are created, but not added to the scene.
    * Until the function setControlPoint(true) is called, the control point
    * will remain invisible and will move with the line to stay at the center.
    *@param p1 the left bounding point of the line
    *@param p2 the right bounding point of the line
    *@param scene a pointer to the main PaintingScene
    *@param id the index of the line in the sorted list of BrLine in the scene
    **/
    BrLine(BoundingPoint* p1, BoundingPoint* p2, PaintingScene* scene, int id);

    /**
    * Delete the QLineF object and the QPainter path.
    **/
    ~BrLine();


    /**
    * Return the bounding rectangle of the line, which is a big rectangle
    * around the bezier curve (the QPainterPath) and its symmetric.
    * This implementation of bounding rectangle allows the view to always
    * draw the entire bezier curve and its symmetric.
    *@return the bounding rectangle of the item
    **/
    virtual QRectF boundingRect() const;

    /**
    @return the coordinates of the control point as a QPointF
    **/
    QPointF coordControlPoint(){return _contr->coord();}

    /**
    * Calculate the coordinates of a bezier point for the given t,
    * the given extremity points and the given control points.
    * Useless in this application.
    *@param p0 the first extremity point
    *@param p1 the first control point
    *@param p2 the second control point
    *@param p3 the second extremity point
    *@param t the parametrized position of the bezier point
    *@return the calculated bezier point at the t position
    **/
    QPointF cubicBezierPoint(QPointF p0, QPointF p1, QPointF p2, QPointF p3, qreal t);

    /**
    * Calculates the bezier point at the parametrized position t for this line.
    * Useless in this application.
    *@param t the parametrized position of the bezier point
    *@return the calculated bezier point at the t position
    **/
    QPointF cubicBezierPoint(qreal t);

    /**
    * Returns the key of the line in the internal structure of the monofin.
    * Make sure the internal key has been set by setInternalKey before,
    * or verify that the value is not the default value : PaintingScene::BADKEY.
    *@return the internal key of the line
    **/
    int internalKey(){return _internalKey;}

    /**
    * Returns true if the current and the given line intersect each other
    * within the start and the end points of each line.
    * ONLY STRAIGHT LINES ARE CONSIDERED, NOT BEZIER CURVES.
    *@param line a reference to the BrLine which intersects or not
    * with the current line
    *@return true if the two lines intersect with a
    * QLineF::BoundedIntersection ; otherwise, returns false
    **/
    bool intersect(BrLine& line);

    /**
    * Returns true if the current and the given line intersect each other
    * within the start and the end points of each line.
    * ONLY STRAIGHT LINES ARE CONSIDERED, NOT BEZIER CURVES.
    *@param line a reference to the QLineF which intersects or not
    * with the current line
    *@return true if the two lines intersect with a
    * QLineF::BoundedIntersection ; otherwise, returns false
    **/
    bool intersect(QLineF& line);

    /**
    * Returns true if the current line (as a BEZIER CURVE) intersects with
    * the given rectangle.
    *@param rect a reference to the rectangle which intersects or not
    * with the current line
    *@return true if the current bezier curve and the rectangle intersect
    * with each other ; otherwise returns false
    **/
    bool intersects(const QRectF& rect)const{return _path->intersects(rect);}

    /**
    *@return true if the control point of this line is activated ;
    * otherwise, returns false
    **/
    bool isControlPointActivated(){return _isControlPointActivated;}

    /**
    * If it's true, it means that the mouse is near the line (as a bezier
    * curve) and that the color of the line has changed in the view (blue
    * by default).
    * This function only works if the scene has indicated to the line
    * that the mouse is on it by calling setMouseOnLine.
    *@return true if the mouse is on the line ; otherwise, returns false.
    **/
    bool isMouseOnLine(){ return _isMouseOnLine;}

    /**
    *@return the length of the line as a STRAIGHT LINE.
    **/
    qreal length(){return _line->length();}

    /**
    *@return the line as a QLineF (a STRAIGHT LINE)
    **/
    QLineF line(){return *_line;}

    /**
    * Returns the index of the line in the list in the PaintingScene.
    * It's NOT the same as the internal key.
    *@return the id of the line
    **/
    int lineId(){return _lineId;}

    /**
    * Updates the position of the line by moving its extremity points
    * to the position of the BoundingPoints if they have moved.
    * This function is called by the bounding points when they move.
    **/
    void move();

    /**
    * Moves the control point to the position of the given coordinates.
    * Make sure the control point has been activated by setControlPoint(true).
    * If the control point is not activated, it will be replaced at the center
    * of the line on the next call to the method move().
    **/
    void moveControlPoint(QPointF pos);

    /**
    * Paints the item as a black straight line when the control point is
    * not activated or as a black bezier curve else.
    * When the mouse is near the line (isMouseOnLine = true), the line
    * (straight or bezier) is painted in blue.
    * It also paints the symetric of the line in black with a
    * little transparency.
    * This function is called by Qt when needed, or by others functions with
    * the method update() of the scene.
    *@param painter the painter which will be used to draw the line
    *@param option not used here, see the documentation of QGraphicsItem
    *@param widget not used here, see the documentation of QGraphicsItem
    **/
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

    /**
    *@return the coordinates of the left bounding point of the line
    * as a QPointF
    **/
    QPointF p1() const{return _p1->coord();}

    /**
    *@return the coordinates of the right bounding point of the line
    * as a QPointF
    **/
    QPointF p2() const{return _p2->coord();}

    /**
    * Returns the coordinates of the point at the parametrized position
    * specified by t. The line is considered as a bezier curve, so it works
    * with or without the control point activated.
    *@param t the parametrized position of the point : 0 give the start of the
    * line and 1 give the end
    *@return the coordinates as a QPointF
    **/
    QPointF pointAt(qreal t){return _path->pointAtPercent(t/2.0);}

    /**
    * If true, activates the control point of the line : the line is no longer
    * considered as a straight line, but as a quadratic bezier curve.
    * The control point and the two tangents are added to the scene.
    * If false, deactivates the control point, removes it from the scene,
    * removes the tangents too and the line as considered as a straight line.
    *@param on true : activates the control point if it's not already
    * activated ; false : deactivates the control point if it is activated
    **/
    void setControlPoint(bool on);

    /**
    * Sets the internal key of the line with the given key
    * which comes from the internal strucure of the monofin.
    *@param key the new key of the line which has to come from the structure
    **/
    void setInternalKey(int key){_internalKey = key;}

    /**
    * Sets the line id (NOT the internal key) to the given value.
    * This function is called by the scene to update the ids of all the lines.
    *@param i the new value of the line id
    **/
    void setLineId(int i);

    /**
    * Function called by the scene to indicates that the mouse is near the
    * line or not anymore.
    *@param on true means that the line will consider that the mouse is on it,
    * false means that the line will no longer consider
    * that the mouse is on it, or it will do nothing if it was already on false
    **/
    void setMouseOnLine(bool on){_isMouseOnLine = on;}

    /**
    * Returns the straight line between the left extremity point of the line
    * and the control point.
    *@return the left tangent of the bezier curve
    **/
    QLineF tangent1(){return _tangent1->line();}

    /**
    * Returns the straight line between the right extremity point of the line
    * and the control point.
    *@return the right tangent of the bezier curve
    **/
    QLineF tangent2(){return _tangent2->line();}

protected:


    /**
    * Removes the control point and the tangents from the scene
    * if they were in it and delete them.
    **/
    void deleteBezier();

    /**
    * Recreates the QPainterPath and the bezier curve with the current
    * positions of the bounding points and the control point.
    * This function is called when the points have moved or before a paint
    * to be sure that the path is always updated.
    **/
    void updatePath();


//ATTRIBUTES

    QColor _color;  //the color used by the QPainter to draw the line

    ControlPoint* _contr; //a pointer to the control point of the line

    int _internalKey;  //the key of the line in the internal
                       //structure of the monofin

    bool _isControlPointActivated; //if true, the control point is added to
                //the scene and the line is considered as a bezier curve ;
                //if false, the line is considered as a straight line

    bool _isMouseOnLine;  //if true, it means that the mouse is near the line

    QLineF* _line;  //the straight line which extremity points are regularly
                //updated to be at the same positions as the two bounding points

    int _lineId;  //the index of the line in the line list of the PaintingScene

    BoundingPoint* _p1; //the bounding point at the left of the line

    BoundingPoint* _p2;  //the bounding point at the right of the point

    QPainterPath* _path;  //the line as a bezier cuve, regularly updated

    PaintingScene* _scene; //a pointer to the main scene

    Tangent* _tangent1;  //a pointer to the left tangent of the line

    Tangent* _tangent2;  //a pointer to the left tangent of the line

};
#endif // BRLINE_H
