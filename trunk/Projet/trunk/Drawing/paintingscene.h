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

class BrLine;
class BoundingPoint;
class ControlPoint;
class SymmetryAxis;
class GhostPoint;
class GhostLine;
class ExtremityPoint;
class SelectionRect;

class PaintingScene: public QGraphicsScene{

#define BEZIERSTEP 0.05

    Q_OBJECT

public:
    /**
    * Contructor with the size of the scene rectangle
    *@param width the width of the scene rectangle
    *@param height the height of the scene rectangle
    *@param parent the parent o the scene : passed to the QObject constructor
    **/
    PaintingScene(qreal width, qreal height, QObject* parent = 0);


    /**
    * Inserts a control point at the right place in the list.
    * Only used by the lines to register their control points.
    * DO NOT add the point to the scene.
    *@param p the control point to be inserted in the list
    *@param i the id o the line which insert the control point
    **/
    void addControlPoint(ControlPoint* p, int i);

    /**
    * Creates a line between two bounding points, add it to the line list,
    * add it to the scene and SET THE RIGHT LINE of p1 and THE LEFT LINE o p2
    * to it.
    *@param p1 the first point of the line
    *@param p2 the second point of the line
    **/
    BrLine* createLine(BoundingPoint* p1, BoundingPoint* p2);

    /**
    * Return the height of the scene rectangle.
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
    * Removes all the points and all the lines from the scene and from the lists.
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
    **/
    void removePoint(BoundingPoint* p);

    /**
    * Update the scale factor of the scene by a factor,
    * but it has (FOR NOW) no effect
    *@param factor the factor used to multiply the scale factor
    **/
    void scale(qreal factor);

    /**
    * Return the scale factor of the scene
    **/
    qreal scaleFactor(){return _scaleFactor;}

    /**
    * Function used by the control points to allows or not
    * the creation of a selection rectangle
    **/
    void setCanCreateSelectionRect(bool c){_canCreateSelectionRect = c;}

    /**
    * Returns the width of the scene rectangle
    **/
    qreal width(){return this->sceneRect().width();}

public slots:
    void activateAddControl(bool a);
    void activateAddPoint(bool a);
    void activateCreateLine(bool a);
    void activateRemoveControlPoint(bool a);
    void alignTangents();
    void cleanPoints();
    void removeSelectedPoints();

    /**
    ***********************************************************************
    * La signature peut être modifiée pour prendre en compte le chemin
    * du fichier, la taille de l'image, etc.
    ***********************************************************************
    **/
    void savePicture();

    void showCoords();
    void simplifyView(bool a);
    void stopCreateLine();

protected:
    SymmetryAxis* _axis;
    QList<ControlPoint*> _controlPointsList;
    GhostLine* _ghostLine;
    GhostPoint* _ghostPoint;
    bool _canCreateSelectionRect;
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
    QList<BrLine*> _lineList;
    QList<BoundingPoint*> _pointList;
    qreal _scaleFactor;
    SelectionRect* _selectionRect;

    void keyPressEvent(QKeyEvent* event);
    void keyReleaseEvent(QKeyEvent* event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
    void updateLinesId();

signals:
    void lineFinished(bool);
    void lineInterrupted();

};

#endif // PAINTINGSCENE_H
