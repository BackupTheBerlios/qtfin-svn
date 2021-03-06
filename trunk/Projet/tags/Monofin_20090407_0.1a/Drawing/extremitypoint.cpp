#include "extremitypoint.h"
#include "brline.h"

//PUBLIC

ExtremityPoint::ExtremityPoint(const QPointF& coord, PaintingScene* scene)
        :BoundingPoint(coord, scene){}

void ExtremityPoint::mouseMoveEvent(QGraphicsSceneMouseEvent* event){
    if(_canMove){
        qDebug("mouse move");

        QRectF zone = _scene->pointsBoundingZone();
        QPointF pos(event->scenePos().x(), 0);
        if(pos.x() < zone.bottomLeft().x()){
            pos.setX(zone.bottomLeft().x());
        }else if(pos.x() > zone.bottomRight().x()){
            pos.setX(zone.bottomRight().x());
        }

        this->moveTo(pos);
        /*_rect->moveTo(event->scenePos().x() - (BOUNDINPOINTSIZE / 2.0),
                      0 - (BOUNDINPOINTSIZE / 2.0));*/

        if(_hasLeftLine) {_leftLine->move();}
        if(_hasRightLine) {_rightLine->move();}
    }else{
        QGraphicsItem::mousePressEvent(event);
    }
    _scene->update(_scene->sceneRect());
}
