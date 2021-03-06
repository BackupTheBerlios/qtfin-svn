#include "extremitypoint.h"
#include "brline.h"

//PUBLIC

ExtremityPoint::ExtremityPoint(const QPointF& coord, PaintingScene* scene)
        :BoundingPoint(coord, scene){}

void ExtremityPoint::moveTo(const QPointF& p, bool useMagnetIfActivated){
    this->prepareGeometryChange();
    QPointF pos(p.x(), 0);

    if(useMagnetIfActivated && _scene->isMagnetActivated()){
        qreal gu = _scene->gridUnit();

        //We add to the position of the point, half of the grid unit, that is
        //to make the point change of grid line, when the cursor is between
        //lines and not on a line (a bit complicated, but remove "+ gu/2.0"
        //to better understand).
        //Then, we divide by the grid unit to know the number of the line
        //where the point should go.
        //And we multiply this number by the grid unit to place the point
        //at the correct position.
        pos.setX((int)((pos.x()+ gu/2.0) / gu) * gu);

    }

    QRectF zone = _scene->pointsBoundingZone();
    if(pos.x() < zone.bottomLeft().x()){
        pos.setX(zone.bottomLeft().x());
    }else if(pos.x() > zone.bottomRight().x()){
        pos.setX(zone.bottomRight().x());
    }

    _pos->setX(pos.x());

    if(_hasLeftLine) {_leftLine->move();}
    if(_hasRightLine) {_rightLine->move();}

}


void ExtremityPoint::mouseMoveEvent(QGraphicsSceneMouseEvent* event){
    if(_canMove){

        QPointF pos(event->scenePos().x(), 0);

        bool canMoveOnX = true;

        QList<QGraphicsItem*> si = _scene->selectedItems();
        BoundingPoint* bp;
        ExtremityPoint* ep;
        foreach(QGraphicsItem* item, si){
            if(item->type() == BoundingPoint::Type){
                bp = qgraphicsitem_cast<BoundingPoint*>(item);
                QLineF l(bp->coord(), this->coord());
                canMoveOnX = canMoveOnX &&
                             bp->isAllowedToMoveAtPosX(pos.x() - l.dx());

            }else if(item->type() == ExtremityPoint::Type && item != this){
                ep = qgraphicsitem_cast<ExtremityPoint*>(item);
                QLineF l(ep->coord(), this->coord());
                canMoveOnX = canMoveOnX &&
                             ep->isAllowedToMoveAtPosX(pos.x() - l.dx());
            }
        }//foreach

        canMoveOnX = canMoveOnX && this->isAllowedToMoveAtPosX(pos.x());

        foreach(ControlPoint* cp, _movingControlPoints){
            QLineF l(cp->coord(), this->coord());
            canMoveOnX = canMoveOnX &&
                         cp->isAllowedToMoveAtPosX(pos.x() - l.dx());
        }//foreach

        if(canMoveOnX){
            QList<QGraphicsItem*> si = _scene->selectedItems();
            foreach(QGraphicsItem* item, si){
                if(item->type() == BoundingPoint::Type){
                    QLineF l(qgraphicsitem_cast<BoundingPoint*>(item)->coord(),
                             this->coord());
                    qgraphicsitem_cast<BoundingPoint*>(item)->moveTo(pos - QPointF(l.dx(), l.dy()));

                }else if(item->type() == ExtremityPoint::Type && item != this){
                    QLineF l(qgraphicsitem_cast<ExtremityPoint*>(item)->coord(),
                             this->coord());
                    qgraphicsitem_cast<ExtremityPoint*>(item)->moveTo(pos - QPointF(l.dx(), l.dy()));
                }
            }//foreach

            foreach(ControlPoint* c, _movingControlPoints){
                QLineF l(c->coord(), this->coord());
                c->moveTo(pos - QPointF(l.dx(), l.dy()));
            }//foreach

            if(pos != *_pos){
                _isMoving = true;
            }

            this->moveTo(pos);
        }//if


    }else{
        QGraphicsItem::mousePressEvent(event);
    }
    _scene->update(_scene->sceneRect());
}
