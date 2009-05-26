#ifndef PAINTINGSCENE_H
#define PAINTINGSCENE_H

#include <QGraphicsScene>
#include <QList>
#include <QVector>
#include <QPointF>
#include <QPolygonF>
#include <QKeyEvent>
#include "boundingpoint.h"
#include <QGraphicsSceneMouseEvent>
#include "controlpoint.h"
#include "extremitypoint.h"
#include <QPainter>
#include <QPixmap>
#include <QTransform>
#include "../Data/projectfile.h"
#include "../EdgeDetection/pixmapitem.h"
#include "../EdgeDetection/rotatecircle.h"

class BrLine;
class BoundingPoint;
class ControlPoint;
class SymmetryAxis;
class GhostPoint;
class GhostLine;
class ExtremityPoint;
class SelectionRect;

class PaintingScene: public QGraphicsScene{

    Q_OBJECT

public:

    enum SceneState{NormalState,
                    CreatePointsState,
                    AddControlPointsState,
                    RemoveControlPointsState,
                    InsertPointsState,
                    ModifyBackgroundPictureState
                };



    //the type of the action when a signal somethingChanged is emitted
    enum DrawingAction{ActionCreateLine,
                       ActionInsertBoundingPoint,
                       ActionInsertControlPoint,
                       ActionMoveBoundingPoint,
                       ActionMoveControlPoint,
                       ActionAlignTangents,
                       ActionDeleteBoundingPoint,
                       ActionDeleteControlPoint,
                       ActionRemoveSomePoints,
                       ActionNoAction
                   };

    //the type of the item, which color is requested or must be changed
    enum ColorItem{BoundingPointColor = 0,
                   ControlPointColor = 1,
                   LineColor = 2
               };

    //the type of the requested color or of the color which has to change
    enum ColorType{NormalColor = 0,
                   HighlightingColor = 1,
                   SelectionColor = 2
               };

    /**
    * Contructor with the size of the scene rectangle
    *@param width the width of the scene rectangle
    *@param height the height of the scene rectangle
    *@param structure a pointer to the internal structure of the monofin
    *@param parent the parent of the scene : passed to the QObject constructor
    **/
    PaintingScene(qreal width, qreal height, Data::ProjectFile* structure, QObject* parent = 0);

    ~PaintingScene();

    /**
    * Inserts a control point at the right place in the list.
    * Only used by the lines to register their control points.
    * DO NOT add the point to the scene.
    *@param p the control point to be inserted in the list
    **/
    void addControlPoint(ControlPoint* p);

    /**
    * Notifies the internal structure that a bounding point has
    * moved and if it is a unique modification or if it is a
    * part of many others modifications.
    *@param p a pointer to the bounding point which has moved
    *@param isUniqueModification if true, the structure will be notified that
    * the movement of the point must be considered alone in the modification
    * stack ; if false, NO function to start or stop the modifications in
    * the structure will be called
    **/
    void boundingPointHasMoved(BoundingPoint* p, bool isUniqueModification = true);

    /**
    * Notifies the scene that a bounding point is highlighted or not, which
    * means that the mouse is near or on this point, or not anymore. Generally
    * called by a point in hoverEnterEvent or hoverLeaveEvent.
    *@param p a pointer to the point which is highlighted or not anymore
    *@param isHighlighted a boolean : true if the point becomes highlighted,
    * false if it is not highlighted anymore
    **/
    void boundingPointIsHighlighted(BoundingPoint* p, bool isHighlighted);


    /**
    * Notifies the internal structure that a control point has
    * moved and if it is a unique modification or if it is a
    * part of many others modifications.
    *@param p a pointer to the control point which has moved
    *@param isUniqueModification if true, the structure will be notified that
    * the movement of the point must be considered alone in the modification
    * stack ; if false, NO function to start or stop the modifications in
    * the structure will be called
    **/
    void controlPointHasMoved(ControlPoint* p, bool isUniqueModification = true);

    /**
    * Notifies the internal structure that a control point has been hidden,
    * which means deleted for the structure, and if it is a unique modification
    * or if it is a part of many others modifications.
    *@param p a pointer to the control point which has been hidden
    *@param isUniqueModification if true, the structure will be notified that
    * the destruction of the point must be considered alone in the modification
    * stack ; if false, NO function to start or stop the modifications in
    * the structure will be called
    **/
    void controlPointHasBeenHidden(ControlPoint* p, bool isUniqueModification = true);


    /**
    *@return the list of the control points which lines have two
    * selected points
    **/
    QList<ControlPoint*> controlPointsSurroundedBySelectedPoints();

    /**
    * Creates a line between two bounding points, add it to the line list,
    * add it to the scene and SET THE RIGHT LINE of p1 and THE LEFT LINE o p2
    * to it.
    *@param p1 the first point of the line
    *@param p2 the second point of the line
    **/
    BrLine* createLine(BoundingPoint* p1, BoundingPoint* p2);

    /**
    *Returns true if there is at least an intersection between two lines
    * (straight lines or bezier curves). Two lines linked to a same point are
    * not considered as intersecting, and because Qt considers this situation
    * as an intersection, the algorithm must appoximate the curves to calculate
    * the intersections. But the approximation is pretty good.
    *@return true if there is an intersection between lines, otherwise
    * return false
    **/
    bool existIntersectionsBetweenLines();

    /**
    *Gives the color of the requesting items for the given type. Item and type
    * must be valids and the type must correspond to the requested item, or
    * the returned color will be a non-valid color.
    *@param item the valid type of item for which the color is requested
    * as a ColorItem
    *@param type the valid type of the requested color as a ColorType
    *@return the color of the given for the given item type as a QColor
    **/
    QColor getColor(ColorItem item, ColorType type);

    /**
    * Return a picture from the scene without background and with all items on
    * their normal color. The size of the picture is the given
    * parameter (x X y). If at least one of the parameters is negative or
    * equal to zero, the returned QPixmap is non-valid.
    *@param x the width of the picture
    *@param y the height of the picture
    *@return the scene painted in a QPixmap
    **/
    QPixmap getPictureOfTheScene(unsigned int x, unsigned int y);

    /**
    *@return the unit of the grid (the length between two lines of the grid)
    **/
    qreal gridUnit(){
        return _gridUnit;
    }

    bool hasABackgroundPicture(){return _hasABackgroundPicture;}

    /**
    *@return the height of the scene rectangle.
    **/
    qreal height(){return this->sceneRect().height();}

    bool isAddControlActivated(){return _isAddControlActivated;}
    bool isAddPointActivated(){return _isAddPointActivated;}
    bool isCreateLineActivated(){return _isCreateLineActivated;}
    bool isMagnetActivated(){return _isMagnetActivated;}
    bool isModifyingPicture(){return isModifyingPicture();}
    bool isRemoveControlPointActivated(){return _isRemoveControlPointActivated;}
    bool isRenderingPicture(){return _isRenderingPicture;}
    bool isSimplifyViewActivated(){return _isSimplifyViewActivated;}

    /**
    * Returns the rectangle in which the points can be created and can move.
    *@return the bounding zone for the points
    **/
    QRectF pointsBoundingZone();

    /**
    * Removes all the points and all the lines from the scene and from the
    * lists without affecting the internal structure.
    * The user should reactivate the "create line" tool
    * after calling this function.
    **/
    void removeAllPoints();

    /**
    * Removes a control point from the list and from the scene if it is present
    * and destroy it.
    * This function is only used by the lines and should not
    * be called by the user.
    *@param p the control point to be deleted
    *@param isInScene boolean to avoid warning if the scene try to remove
    * an item not present in it
    **/
    void removeControlPoint(ControlPoint* p, bool isInScene = true);

    /**
    * Removes a line from the scene, from the list and destroys it.
    *@param l the BrLine to be removed
    **/
    void removeLine(BrLine* l);

    /**
    * Removes a bounding point from the scene, from the list and delete it.
    * It also removes its lines and creates a new line between the two points
    * around it, if they exist.
    *@param p a pointer to the bounding point to destroy
    **/
    void removePoint(BoundingPoint* p);

    /**
    * Update the scale factor of the scene by a factor.
    *@param factor the factor used to multiply the scale factor
    **/
    void scale(qreal factor);

    /**
    *@return the scale factor of the scene
    **/
    qreal scaleFactor(){return _scaleFactor;}

    /**
    * Function used by the control points to allows or not
    * the creation of a selection rectangle
    *@param c true means that the user can create a selection rectangle ;
    * false forbid the creation of a selection rectangle
    **/
    void setCanCreateSelectionRect(bool c){_canCreateSelectionRect = c;}

    /**
    * Indicates to the internal structure that the next modifications on the
    * scene will be considered as one, until stopModifications is call.
    * WARNING : be careful to use this function ONLY when you precise that
    * the modifications are NOT unique (see boundingPointHasMoved or
    * controlPointHasMoved).
    **/
    void startModifications(){
        _structure->startHistory(Data::MonofinSurface);
    }

    /**
    *@return the state of the scene as an int, see enum SceneState for details
    **/
    int state(){return _state;}

    /**
    * Indicates to the internal structure that the modifications considered
    * as one are finished. Be careful to have only use functions with NOT
    * unique modifications (see boundingPointHasMoved or controlPointHasMoved).
    * It also emit a signal somethingChanged if several points have moved.
    **/
    void stopModifications(){
        if(this->_hasABoundingPointMoved){
            emit somethingChanged(ActionMoveBoundingPoint);
        }
        _structure->stopHistory(Data::MonofinSurface);
    }

    /**
    *@returns the width of the scene rectangle
    **/
    qreal width(){return this->sceneRect().width();}

public slots:
    /**
    * Activates or deactivates the addition of control points.
    * It changes the state of the scene.
    *@param a if true, activates 'add control point' ;
    * if false, deactivates 'add control points'
    **/
    void activateAddControl(bool a);

    /**
    * Activates or deactivates the insertion of points.
    * It changes the state of the scene.
    *@param a if true, activates 'insert point' ;
    * if false, deactivates 'insert points'
    **/
    void activateAddPoint(bool a);

    /**
    * Activates or deactivates the creation of the monofin's outline.
    * Activate it creates a ghost point in the scene. It also changes the
    * state of the scene.
    * Deactivate it stops the creation of the line if there is one in creation.
    * In this case, the slot stopCreateLine is called.
    *@param a if true, activates 'draw monofin' ;
    * if false, deactivates 'draw monofin'
    **/
    void activateCreateLine(bool a);

    /**
    * Activated or deactivates the magnet, which makes a point move only
    * to the intersections of the grid lines.
    *@param a if true, activates the magnet ; if false, deactivates the magnet
    **/
    void activateMagnet(bool a){ _isMagnetActivated = a;}

    /**
    * Activates or deactivates the deletion of control points.
    * It changes the state of the scene.
    *@param a if true, activates 'remove control point' ;
    * if false, deactivates 'remove control point'
    **/
    void activateRemoveControlPoint(bool a);

    /**
    * Enlarges or shrinks the scene rectangle. The size of the zones where the
    * points can be placed or can move will be changed, but it will never be
    * smaller that a rectangle around all the items on the scene (when using
    * this function).
    *@param dx the size to add to the rectangle on the right side. If the
    * value is positive, the rectangle will be larger, and if the value
    * is negative, the rectangle will be smaller
    *@param dy the size to add to the top AND the bottom of the rectangle. If
    * the value is positive, the rectangle will be larger, and if the value
    * is negative, the rectangle will be smaller
    **/
    void adjustSceneRect(qreal dx, qreal dy);

    /**
    * Calls the function alignTangents() on every selected points. It is
    * NOT ADVISED to call this function when several points are selected,
    * but it will work. The result when several points are selected is
    * generally useless.
    **/
    void alignTangents();

    /**
    * Changes the color of the given type for the given items by the
    * given color.
    * The item must be one of the following ColorItem :
    *   BoundingPointColor, ControlPointColor or LineColor.
    * The type must be one of the following ColorType :
    *   NormalColor, SelectionColor or HighlightingColor.
    * If type or item is not valid, the function does nothing.
    *@param item the items which color has to change, as a ColorItem
    *@param type the type of the color which has to change, as a ColorType
    *@param color a reference to the new color of the items
    **/
    void changeColor(int item, int type, const QColor& color);

    /**
    * Removes all points of the scene and of the internal structure.
    **/
    void cleanPoints();


    /**
    * Determines if a bezier curve is kept when it is cut. It has no effect
    * on straight lines.
    *@param k if true, a bezier curve is cut into two bezier curve when
    * inserting a point ; if false, a bezier curve is cut into two
    * straight lines
    **/
    void keepBezierCurve(bool k);


    /**
    * Activates or deactivates the possibility of modifying the background
    * picture (move, rotate, csale). It changes the scene state. If the scene
    * has no background picture, it only change its state.
    *@param on if true, add the background picture to the scene and allow
    * the user to modify the it; if false, remove the background picture and
    * set it as a background of the scene
    **/
    void modifyBackgroundPicture(bool on);

    /**
    * Calls the redo function on the internal structure and get the points
    * back from it, by calling getMonofinFromStructure(), which EMIT
    * a pointOnScene(bool) signal.
    **/
    void redo();

    /**
    * Removes the background picture from the scene or from its background.
    * It does nothing if the scene has no background picture.
    **/
    void removeBackgroundPicture();

    /**
    * Removes the selected points of the scene and of the internal structure.
    **/
    void removeSelectedPoints();

    /**
    * Sets a background picture for the scene. Depending of the state of the
    * scene, if the user is already allowed to modify this picture or not
    * (see modifyBackgroundPicture), the picture is set on the background of
    * the scene, or as an item added to it. If the pixmap is null,
    * it does nothing.
    *@param pixmap the picture to set on the background as a QPixmap
    **/
    void setBackGroundPicture(QPixmap pixmap);

    /**
    * Sets the size of the unit of the grid (The length between two lines of
    * the grid). It must be greater than zero.
    *@param the new unit of the grid
    **/
    void setGridUnit(qreal unit){
        if(unit > 0){_gridUnit = unit; this->update(this->sceneRect());}
    }

    /**
    * Changes the way the items are painted.
    *@param a if true, the items are more simply drawn ; if false, the items
    * are painted as usually
    **/
    void simplifyView(bool a);

    /**
    * Stops the creation of a monofin's outline if the first point was placed,
    * but not the last. It removes the placed points and emit a
    * lineInterrupted signal. It also emits a somethingChanged signal.
    **/
    void stopCreateLine();

    /**
    * Calls the undo function on the internal structure and get the points
    * back from it, by calling getMonofinFromStructure(), which EMIT
    * a pointOnScene(bool) signal.
    **/
    void undo();

    /**
    * Removes all the points from the scene (NOT from the structure) if there
    * is any, and get new one from the internal structure.
    **/
    void updateMonofinDrawing(){this->getMonofinFromStructure();}

    /**
    * Multiply the scale of the background picture by the given factor
    * (>1 will enlarge the picture, <1 will shrink it). The original scale of
    * the picture is 1. If this value becomes lower than 0.01 or greater
    * than 2, the function will do nothing.
    *@param factor the factor by which we multply the scale of the picture
    **/
    void zoomOnBackgroundPicture(qreal factor);

signals:

    /**
    * Signal emitted when an action which is in the undo/redo is made.
    * The parameter indicates the type of the action.
    **/
    void somethingChanged(int);

    /**
    * Signal emitted with the parameter true when the last point of the
    * monofin's outline has been placed.
    **/
    void lineFinished(bool);

    /**
    * Signal emitted when the slot stopCreateLine() is called. It means that
    * there is no points on the scene anymore and that the user can begin a
    * new drawing of the monofin.
    **/
    void lineInterrupted();

    /**
    * Signal called when the scene get the points back from the internal
    * structure. If the parameter is true, it means that there are points
    * presents on the scene, and that the user is not allowed to create a
    * new monofin's outline. If it is false, it means that the scene is empty
    * of points and lines, and that the user can begin a new drawing.
    **/
    void pointsOnScene(bool);


protected:

    //the position of the colors in the color vector
    enum VectorColor{BoundingPointNormalColor = 0,
                    BoundingPointHighlightingColor = 1,
                    BoundingPointSelectionColor = 2,
                    ControlPointNormalColor = 3,
                    ControlPointHighlightingColor = 4,
                    LineNormalColor = 5,
                    LineHighlightingColor = 6
                };

    /**
    * Modifies the scene rectangle if there are items outside it, so it can
    * contain the items bounding rectangle.
    **/
    void adjustSceneRectangleToItems();

    virtual void drawBackground(QPainter* painter, const QRectF& rect);
    void getMonofinFromStructure();
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);


//ATTRIBUTES

    SymmetryAxis* _axis;
    bool _canCreateSelectionRect;
    QVector<QColor> _colors;
    QList<ControlPoint*> _controlPointsList;
    GhostLine* _ghostLine;
    GhostPoint* _ghostPoint;
    qreal _gridUnit;
    bool _hasABackgroundPicture;
    bool _hasABoundingPointMoved;
    bool _hasPlacedFirstPoint;
    bool _isAddControlActivated;
    bool _isAddPointActivated;
    bool _isAPointHighlighted;
    bool _isCreateLineActivated;
    bool _isCreatingSelectionRect;
    bool _isMagnetActivated;
    bool _isModifyingBackgroundPicture;
    bool _isMultiSelectionKeyActivated;
    bool _isRemoveControlPointActivated;
    bool _isRenderingPicture;
    bool _isSimplifyViewActivated;
    BoundingPoint* _itemUnderMouse;
    bool _keepBezierCurve;
    BoundingPoint* _lastPlacedPoint;
    QList<BrLine*> _lineList;
    PixmapItem* _pixItem;
    QPixmap* _pixmap;
    QList<BoundingPoint*> _pointList;
    RotateCircle* _rotCircle;
    qreal _scaleFactor;
    SelectionRect* _selectionRect;
    SceneState _state;
    Data::ProjectFile* _structure;

};

#endif // PAINTINGSCENE_H
