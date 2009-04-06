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
#include <QDrag>
#include <QMimeData>

class BrLine;
class PaintingScene;

class BoundingPoint: public QGraphicsItem{

    //Q_OBJECT

#define BOUNDINPOINTSIZE 8



public:
    enum{Type = UserType + 1};

    BoundingPoint(qreal x, qreal y, PaintingScene* scene);
    BoundingPoint(const QPointF& coord, PaintingScene* scene);
    ~BoundingPoint();

    void alignTangents();
    virtual QRectF boundingRect() const;
    QPointF coord() const;
    void hideLeftLine(){_hasLeftLine = false;}
    void hideRightLine(){_hasRightLine = false;}
    bool isMouseOnPoint(){return _isMouseOnPoint;}
    void moveTo(const QPointF& p);
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
    QRectF rect(){return *_rect;}
    void removeLeftLine();
    void removeRightLine();
    void setCanMove(bool c){_canMove = c;}
    void setLeftLine(BrLine* l);
    void setMouseOnPoint(bool on);
    void setRightLine(BrLine* l);
    int type() const{return Type;}


/*signals:
    void moving();*/

protected:
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent* event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);
    //virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event);
    //virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event);
    //void dragEnterEvent(QGraphicsSceneDragDropEvent *event);

    bool _atLeastOneClick;
    bool _canMove;
    QColor _color;
    bool _hasLeftLine;
    bool _hasRightLine;
    bool _isMouseOnPoint;
    BrLine* _leftLine;
    BrLine* _rightLine;
    QRectF* _rect;
    PaintingScene* _scene;

    //bool _canBeSelected;
    //bool _isMoving;

};
#endif // BOUNDINGPOINT_H
