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
    BrLine(BoundingPoint* p1, BoundingPoint* p2, PaintingScene* scene, int id);

    virtual QRectF boundingRect() const;
    QPointF coordControlPoint(){return _contr->coord();}
    QPointF cubicBezierPoint(QPointF p0, QPointF p1, QPointF p2, QPointF p3, qreal t);
    QPointF cubicBezierPoint(qreal t);
    bool intersect(BrLine& line);
    bool intersect(QLineF& line);
    bool intersects(const QRectF& rect)const{return _path->intersects(rect);}
    bool isMouseOnLine(){ return _isMouseOnLine;}
    QLineF line(){return *_line;}
    int lineId(){return _lineId;}
    void move();
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
    QPointF p1() const{return _p1->coord();}
    QPointF p2() const{return _p2->coord();}
    void removeFromScene();
    void setControlPoint(bool on);
    void setLineId(int i);
    void setMouseOnLine(bool on){_isMouseOnLine = on;}
    void updatePath();


protected:

    QColor _color;
    ControlPoint* _contr;
    bool _isControlPointActivated;
    bool _isMouseOnLine;
    QLineF* _line;
    int _lineId;
    BoundingPoint* _p1;
    BoundingPoint* _p2;
    QPainterPath* _path;
    PaintingScene* _scene;
    Tangent* _tangent1;
    Tangent* _tangent2;

};
#endif // BRLINE_H
