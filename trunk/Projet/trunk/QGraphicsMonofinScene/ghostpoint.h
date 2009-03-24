#ifndef GHOSTPOINT_H
#define GHOSTPOINT_H

#include <QGraphicsItem>
#include "boundingpoint.h"

class BoundingPoint;
class PaintingScene;

class GhostPoint: public QGraphicsItem{

public:
    GhostPoint(QPointF coord, PaintingScene* scene);

    virtual QRectF boundingRect() const{return _p->boundingRect();}
    bool canBePlaced(){return _canBePlaced;}
    QPointF coord(){return _p->coord();}
    void moveTo(const QPointF& p){_p->moveTo(p);}
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
    void setCanBePlaced(bool p){_canBePlaced = p;}

protected:
    bool _canBePlaced;
    BoundingPoint* _p;
    PaintingScene* _scene;

};

#endif // GHOSTPOINT_H
