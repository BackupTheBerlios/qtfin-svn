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
    PaintingScene(qreal width, qreal height, QObject* parent = 0);

    void addControlPoint(ControlPoint* p, int i);
    BrLine* createLine(BoundingPoint* p1, BoundingPoint* p2);
    qreal height(){return this->sceneRect().height();}
    bool isAddControlActivated(){return _isAddControlActivated;}
    bool isCreateLineActivated(){return _isCreateLineActivated;}
    bool isRemoveControlPointActivated(){return _isRemoveControlPointActivated;}
    QRectF pointsBoundingZone();
    void removeAllPoints();
    void removeControlPoint(ControlPoint* p);
    void removeLine(BrLine* l);
    void removePoint(BoundingPoint* p);
    void scale(qreal factor);
    qreal scaleFactor(){return _scaleFactor;}
    void setCanCreateSelectionRect(bool c){_canCreateSelectionRect = c;}
    qreal width(){return this->sceneRect().width();}

public slots:
    void activateAddControl(bool a);
    void activateAddPoint(bool a);
    void activateCreateLine(bool a);
    void activateRemoveControlPoint(bool a);
    void alignTangents();
    void cleanPoints();
    void removeSelectedPoints();
    void showCoords();

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
    bool _isOneSelection;
    bool _isRemoveControlPointActivated;
    QList<BrLine*> _lineList;
    QList<BoundingPoint*> _pointList;
    qreal _scaleFactor;
    SelectionRect* _selectionRect;

    void keyPressEvent(QKeyEvent* event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
    void updateLinesId();

signals:
    void lineFinished(bool);

};

#endif // PAINTINGSCENE_H
