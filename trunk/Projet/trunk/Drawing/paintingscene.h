#ifndef PAINTINGSCENE_H
#define PAINTINGSCENE_H

#include <QGraphicsScene>
#include <QList>
#include <QPointF>
#include <QPolygonF>
#include <QKeyEvent>
#include "boundingpoint.h"
#include <QGraphicsSceneMouseEvent>
#include "controlpoint.h"
#include "extremitypoint.h"
#include <QPainter>
#include <QPixmap>
#include <QMatrix>
#include "../Data/projectfile.h"

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

    static const int BADKEY = -1;

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
    *@param i the id of the line which insert the control point
    **/
    void addControlPoint(ControlPoint* p, int i);

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
    * Creates a line between two bounding points, add it to the line list,
    * add it to the scene and SET THE RIGHT LINE of p1 and THE LEFT LINE o p2
    * to it.
    *@param p1 the first point of the line
    *@param p2 the second point of the line
    **/
    BrLine* createLine(BoundingPoint* p1, BoundingPoint* p2);

    /**
    *@return the height of the scene rectangle.
    **/
    qreal height(){return this->sceneRect().height();}

    bool isAddControlActivated(){return _isAddControlActivated;}
    bool isCreateLineActivated(){return _isCreateLineActivated;}
    bool isRemoveControlPointActivated(){return _isRemoveControlPointActivated;}
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
    *@returns the width of the scene rectangle
    **/
    qreal width(){return this->sceneRect().width();}

public slots:
    /**
    * Activates or deactivates the addition of control points.
    *@param a if true, activates 'add control point' ;
    * if false, deactivates 'add control points'
    **/
    void activateAddControl(bool a);

    /**
    * Activates or deactivates the insertion of points.
    *@param a if true, activates 'insert point' ;
    * if false, deactivates 'insert points'
    **/
    void activateAddPoint(bool a);

    /**
    * Activates or deactivates the creation of the monofin's outline.
    Activate it creates a ghost point in the scene.
    *@param a if true, activates 'draw monofin' ;
    * if false, deactivates 'draw monofin'
    **/
    void activateCreateLine(bool a);

    /**
    * Activates or deactivates the deletion of control points.
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
    void ajustSceneRect(qreal dx, qreal dy);

    /**
    * Calls the function alignTangents() on every selected points. It is
    * NOT ADVISED to call this function when several points are selected,
    * because it only aligns the tangents of the points ONE BY ONE. It will
    * NOT align several points.
    **/
    void alignTangents();

    /**
    * Removes all points of the scene and of the internal structure.
    **/
    void cleanPoints();

    /**
    * Removes the selected points of the scene and of the internal structure.
    **/
    void removeSelectedPoints();


    /**
    ***********************************************************************
    * Sauvegarde un image de la scene dans le dossier de l'exécutable
    * "test.png"

    * La signature peut être modifiée pour prendre en compte le chemin
    * du fichier, la taille de l'image, etc.

    QUELQU'UN STP DIS-MOI CE QUE TU VEUX POUR CETTE FONCTION !!!

    ***********************************************************************
    **/
    void savePicture();

    //void showCoords();

    /**
    * Changes the way the items are painted.
    *@param a if true, the items are more simply drawn ; if false, the items
    * are painted as usually
    **/
    void simplifyView(bool a);

    /**
    * Stops the creation of a monofin's outline if the first point was placed,
    * but not the last. It removes the placed points, destroys the ghost
    * items, and emit a lineInterrupted signal.
    **/
    void stopCreateLine();

    /**
    * Calls the redo function on the internal structure and get the points
    * back from it, by calling getMonofinFromStructure(), which EMIT
    * a pointOnScene(bool) signal.
    **/
    void redo();

    /**
    * Calls the undo function on the internal structure and get the points
    * back from it, by calling getMonofinFromStructure(), which EMIT
    * a pointOnScene(bool) signal.
    **/
    void undo();

signals:

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

    void getMonofinFromStructure();
    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
    void updateLinesId();


//ATTRIBUTES

    SymmetryAxis* _axis;
    bool _canCreateSelectionRect;
    QList<ControlPoint*> _controlPointsList;
    int _firstPointKey;
    GhostLine* _ghostLine;
    GhostPoint* _ghostPoint;
    bool _hasPlacedFirstPoint;
    bool _isAddControlActivated;
    bool _isAddPointActivated;
    bool _isCreateLineActivated;
    bool _isCreatingSelectionRect;
    bool _isMultiSelectionKeyActivated;
    bool _isOneSelection;
    bool _isRemoveControlPointActivated;
    bool _isSimplifyViewActivated;
    BoundingPoint* _itemUnderMouse;
    int _lastPointKey;
    QList<BrLine*> _lineList;
    QList<BoundingPoint*> _pointList;
    qreal _scaleFactor;
    SelectionRect* _selectionRect;
    Data::ProjectFile* _structure;

};

#endif // PAINTINGSCENE_H
