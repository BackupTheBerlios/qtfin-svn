#ifndef GHOSTLINE_H
#define GHOSTLINE_H

#include <QGraphicsItem>
#include <QRectF>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>

class PaintingScene;
class BoundingPoint;
class GhostPoint;

class GhostLine: public QGraphicsItem{

public:
    GhostLine(BoundingPoint* p1, GhostPoint* p2, PaintingScene* scene);

    virtual QRectF boundingRect() const;
    bool canBePlaced(){return _canBePlaced;}
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
    void setCanBePlaced(bool p){_canBePlaced = p;}
    void setP1(BoundingPoint* p1){_p1 = p1;}

protected:
    bool _canBePlaced;
    BoundingPoint* _p1;
    GhostPoint* _p2;
    PaintingScene* _scene;



};

#endif // GHOSTLINE_H
