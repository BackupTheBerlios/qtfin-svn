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

#define CONTRPOINTHEIGHT 10

public:

    ControlPoint(qreal x, qreal y, BrLine* line, PaintingScene* scene);
    ControlPoint(const QPointF& coord, BrLine* line, PaintingScene* scene);
    ~ControlPoint();

    virtual QRectF boundingRect() const{return *_rect;}
    QPointF coord() const{return _rect->center();}
    void moveTo(const QPointF& p){_rect->moveCenter(p);}
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
    void setLeftLine(BrLine* l);
    void setRightLine(BrLine* l);
    void setTangent1(Tangent* t){ _tangent1 = t;}
    void setTangent2(Tangent* t){ _tangent2 = t;}

/*signals:
    void moving();*/

protected:
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

    bool _canMove;
    QColor _color;
    BrLine* _line;
    QRectF* _rect;
    PaintingScene* _scene;
    Tangent* _tangent1;
    Tangent* _tangent2;

};

#endif // CONTROLPOINT_H
