#include "boundingpoint.h"
#include "brline.h"
#include "paintingscene.h"

//PUBLIC

BoundingPoint::BoundingPoint(qreal x, qreal y, PaintingScene* scene):
        QGraphicsItem(), _atLeastOneClick(false), _canMove(false),
      /*_canBeSelected(true),*/
     _color(Qt::black), _hasLeftLine(false), _hasRightLine(false),
     _internalKey(Data::MONOFIN_SURFACE_NOT_CREATED_POINT),
     _isMouseOnPoint(false),  _isMoving(false),
     _scene(scene){

    this->setAcceptHoverEvents(true);
    //this->setFlag(QGraphicsItem::ItemIsSelectable, true);

    _pos = new QPointF(x, y);

}

BoundingPoint::BoundingPoint(const QPointF& coord, PaintingScene* scene):
        QGraphicsItem(), _atLeastOneClick(false), _canMove(false),
      /*_canBeSelected(true),*/
     _color(Qt::black), _hasLeftLine(false), _hasRightLine(false),
     _internalKey(Data::MONOFIN_SURFACE_NOT_CREATED_POINT),
     _isMouseOnPoint(false),  _isMoving(false),
     _scene(scene){

    this->setAcceptHoverEvents(true);
    //this->setFlag(QGraphicsItem::ItemIsSelectable, true);

    _pos = new QPointF(coord);
}

BoundingPoint::BoundingPoint(const BoundingPoint& p):
        _atLeastOneClick(p._atLeastOneClick), _canMove(p._canMove),
     _color(p._color), _hasLeftLine(false), _hasRightLine(false),
     _isMouseOnPoint(p._isMouseOnPoint), _isMoving(p._isMoving),
     _scene(p._scene){

    this->setAcceptHoverEvents(true);
    //this->setFlag(QGraphicsItem::ItemIsSelectable, true);

    _pos = new QPointF(p.coord());

}

BoundingPoint::~BoundingPoint(){
    delete _pos;
    //les objets BrLine* sont détruits dans la PaintingScene
}

void BoundingPoint::alignTangents(){
    QPointF oldPos = this->coord();
    if(_hasLeftLine && _hasRightLine){
        QPointF leftContr;
        QPointF rightContr;
        if(_leftLine->isControlPointActivated()){
            leftContr = _leftLine->coordControlPoint();
        }else{
            leftContr = _leftLine->p1();
        }
        if(_rightLine->isControlPointActivated()){
            rightContr = _rightLine->coordControlPoint();
        }else{
            rightContr = _rightLine->p2();
        }

        /*
        //trouve la perpendiculaire à la ligne formée par les
        //deux points entre lesquels il faut aligner
        QLineF l(leftContr, rightContr);
        QLineF norm(l.normalVector());
        QLineF offset(leftContr, this->coord());
        norm.translate(offset.dx(), offset.dy());
        QPointF pos(0,0);
        norm.intersect(l,&pos);
        */
        qreal leftLength = QLineF(leftContr, this->coord()).length();
        qreal rightLength = QLineF(rightContr, this->coord()).length();
        QLineF l(leftContr, rightContr);
        qreal t = leftLength / (leftLength + rightLength);
        QPointF pos = l.pointAt(t);
        this->moveTo(pos);
    }else{
        qreal x = 0;
        if(_hasLeftLine){
            if(_leftLine->isControlPointActivated()){
                x = _leftLine->coordControlPoint().x();
            }else{
                x = _leftLine->p1().x();
            }

        }else if(_hasRightLine){
            if(_rightLine->isControlPointActivated()){
                x = _rightLine->coordControlPoint().x();
            }else{
                x = _rightLine->p2().x();
            }
        }
        this->moveTo(QPointF(x, this->coord().y()));
    }

    if(oldPos != this->coord()){
        _scene->boundingPointHasMoved(this, false);
    }

   _scene->update(_scene->sceneRect());
}

QRectF BoundingPoint::boundingRect() const{
    QRectF r(_pos->x() - (this->rectangleSize() / 2.0) - 5,
             _pos->y() - (this->rectangleSize() / 2.0) - 5,
             this->rectangleSize() + 10,
             this->rectangleSize() + 10);
    return r;

}

bool BoundingPoint::isAllowedToMoveAtPosX(qreal posX){
    QRectF zone = _scene->pointsBoundingZone();
    return (posX > zone.left() && posX < zone.right());
}

bool BoundingPoint::isAllowedToMoveAtPosY(qreal posY){
    QRectF zone = _scene->pointsBoundingZone();
    return (posY > 0 && posY < zone.bottom());
}

void BoundingPoint::moveTo(const QPointF& p){
    this->prepareGeometryChange();
    QPointF pos = p;

    QRectF zone = _scene->pointsBoundingZone();
    if(pos.x() < zone.bottomLeft().x()){
        pos.setX(zone.bottomLeft().x());
    }else if(pos.x() > zone.bottomRight().x()){
        pos.setX(zone.bottomRight().x());
    }
    if(pos.y() < 0){
        pos.setY(0);
    }else if(pos.y() > zone.bottomLeft().y()){
        pos.setY(zone.bottomLeft().y());
    }

    _pos->setX(pos.x());
    _pos->setY(pos.y());

    if(_hasLeftLine) {_leftLine->move();}
    if(_hasRightLine) {_rightLine->move();}
}

void BoundingPoint::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget){
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if(!_scene->isSimplifyViewActivated()){
        QPen pen;
        if(this->isSelected()){
            _color = Qt::red;
            painter->setPen(_color);
            //painter->drawEllipse(*_pos, this->rectangleSize(), this->rectangleSize());
            /*QRadialGradient gradient(this->coord(), this->rectangleSize() + 1);
            QBrush brush(gradient);
            painter->setBrush(brush);*/

            //painter->drawEllipse(this->coord(), this->rectangleSize() + 0.0, this->rectangleSize() + 0.0);
        }else{
            if(_isMouseOnPoint){
                _color = Qt::blue;
                pen.setWidthF(2.2);
                /*pen.setColor(_color);
                painter->setPen(pen);
                painter->drawEllipse(this->coord(), this->rectangleSize() + 0.0, this->rectangleSize() + 0.0);*/
            }else{
                _color = Qt::black;
            }
        }

        pen.setColor(_color);
        painter->setPen(pen);
        painter->drawRect((int)(_pos->x() - (this->rectangleSize() / 2.0)),
               (int)(_pos->y() - (this->rectangleSize() / 2.0)),
               (int)this->rectangleSize(),
               (int)this->rectangleSize());

    }else{
        Q_UNUSED(painter);
    }
}


void BoundingPoint::removeLeftLine(){
    if(_hasLeftLine){
        _hasLeftLine = false;
        //on appelle la fonction de destruction d'une ligne
        _scene->removeLine(_leftLine);
    }
}

void BoundingPoint::removeRightLine(){
    if(_hasRightLine){
        _hasRightLine = false;
        //on appelle la fonction de destruction d'une ligne
        _scene->removeLine(_rightLine);
    }
}

void BoundingPoint::setLeftLine(BrLine* l){
    _leftLine = l;
    _hasLeftLine = true;

}

void BoundingPoint::setMouseOnPoint(bool on){
    _isMouseOnPoint = on;
    /*if(on){
        if(!this->isSelected()){
        _color = Qt::blue;
        }
    }else{
        if(!this->isSelected()){
        _color = Qt::black;
        }
    }*/
    _scene->update(_scene->sceneRect());
}

void BoundingPoint::setRightLine(BrLine* l){
    _rightLine = l;
    _hasRightLine = true;

}

bool BoundingPoint::willMoveToGoCloseToPosX(qreal posX){
    QRectF zone = _scene->pointsBoundingZone();
    if(posX < zone.left()){
        return _pos->x() != zone.left();
    }else if(posX > zone.right()){
        return _pos->x() != zone.right();
    }else{
        return _pos->x() != posX;
    }
}

bool BoundingPoint::willMoveToGoCloseToPosY(qreal posY){
    QRectF zone = _scene->pointsBoundingZone();
    if(posY < 0){
        return _pos->y() != 0;
    }else if(posY > zone.bottom()){
        return _pos->y() != zone.bottom();
    }else{
        return _pos->y() != posY;
    }
}

//PROTECTED

void BoundingPoint::mouseMoveEvent(QGraphicsSceneMouseEvent* event){
    if(_canMove){
        QPointF pos(event->scenePos().x(), event->scenePos().y());

        bool canMoveOnX = true;
        bool canMoveOnY = true;

        QList<QGraphicsItem*> si = _scene->selectedItems();
        BoundingPoint* bp;
        ExtremityPoint* ep;
        //ControlPoint* cp;
        foreach(QGraphicsItem* item, si){
            if(item->type() == BoundingPoint::Type){
                bp = qgraphicsitem_cast<BoundingPoint*>(item);
                QLineF l(bp->coord(), this->coord());
                canMoveOnX = canMoveOnX &&
                             bp->isAllowedToMoveAtPosX(pos.x() - l.dx());
                canMoveOnY = canMoveOnY &&
                             bp->isAllowedToMoveAtPosY(pos.y() - l.dy());

            }else if(item->type() == ExtremityPoint::Type){
                ep = qgraphicsitem_cast<ExtremityPoint*>(item);
                QLineF l(ep->coord(), this->coord());
                canMoveOnX = canMoveOnX &&
                             ep->isAllowedToMoveAtPosX(pos.x() - l.dx());
                canMoveOnY = false;
            }
        }//foreach

        foreach(ControlPoint* cp, _movingControlPoints){
            QLineF l(cp->coord(), this->coord());
            canMoveOnX = canMoveOnX &&
                         cp->isAllowedToMoveAtPosX(pos.x() - l.dx());
            canMoveOnY = canMoveOnY &&
                         cp->isAllowedToMoveAtPosY(pos.y() - l.dy());
        }//foreach


        if(!canMoveOnY){
            pos.setY(_pos->y());
        }
        if(!canMoveOnX){
            pos.setX(_pos->x());
        }

        if(canMoveOnX || canMoveOnY){
            //QList<QGraphicsItem*> si = _scene->selectedItems();
            foreach(QGraphicsItem* item, si){
                if(item->type() == BoundingPoint::Type && item != this){
                    QLineF l(qgraphicsitem_cast<BoundingPoint*>(item)->coord(),
                             this->coord());
                    qgraphicsitem_cast<BoundingPoint*>(item)->
                            moveTo(pos - QPointF(l.dx(), l.dy()));

                }else if(item->type() == ExtremityPoint::Type){
                    QLineF l(qgraphicsitem_cast<ExtremityPoint*>(item)->coord(),
                             this->coord());
                    qgraphicsitem_cast<ExtremityPoint*>(item)->
                            moveTo(pos - QPointF(l.dx(), l.dy()));
                }
            }//foreach

            foreach(ControlPoint* c, _movingControlPoints){
                QLineF l(c->coord(), this->coord());
                c->moveTo(pos - QPointF(l.dx(), l.dy()));
            }//foreach

        }//if

        //qDebug("number of selected items : %d", si.size());

        if(pos != *_pos){
            _isMoving = true;
        }

        this->moveTo(pos);


    }else{
        //QGraphicsItem::mousePressEvent(event);
    }
    _scene->update(_scene->sceneRect());
 }

void BoundingPoint::mousePressEvent(QGraphicsSceneMouseEvent* event){
    if(event->button() == Qt::LeftButton){
        if(!this->isSelected()){
            //_scene->clearSelection();
            //this->setSelected(true);
            _canMove = false;
            //QGraphicsItem::mousePressEvent(event);
        }
        else{
            _canMove = true;

            _movingControlPoints.clear();
            _movingControlPoints = _scene->controlPointsSurroundedBySelectedPoints();
        }

    }else{
        _canMove = false;
        QGraphicsItem::mousePressEvent(event);
    }
}

void BoundingPoint::mouseReleaseEvent(QGraphicsSceneMouseEvent* event){
    if(event->button() == Qt::LeftButton){
        _canMove = false;

        if(_isMoving){
            _isMoving = false;

            _scene->startModifications();

            QList<QGraphicsItem*> si = _scene->selectedItems();
            foreach(QGraphicsItem* item, si){
                if(item->type() == BoundingPoint::Type){
                    _scene->boundingPointHasMoved(
                            qgraphicsitem_cast<BoundingPoint*>(item), false);
                }else if(item->type() == ExtremityPoint::Type){
                    _scene->boundingPointHasMoved(
                            qgraphicsitem_cast<ExtremityPoint*>(item), false);
                }
            }
            foreach(ControlPoint* c, _movingControlPoints){
                _scene->controlPointHasMoved(c, false);
            }
            _movingControlPoints.clear();

            _scene->stopModifications();
        }

        if(_atLeastOneClick){}else{
            //this->setSelected(false);
            _atLeastOneClick = true;
        }

    }else{
        _canMove = false;
        QGraphicsItem::mousePressEvent(event);
    }
    //_isMoving = false;
    _scene->update(_scene->sceneRect());
}

void BoundingPoint::hoverEnterEvent(QGraphicsSceneHoverEvent* event){
    //qDebug("enter hover");

    if(!(_scene->isAddControlActivated()||
         _scene->isAddPointActivated()||
         _scene->isRemoveControlPointActivated())){

        _scene->setCanCreateSelectionRect(false);
        //if(!this->isSelected()){
            this->setMouseOnPoint(true);
            _scene->boundingPointIsHighlighted(this, true);
        //}

        //Décommenter le bloc suivant rendra
        //les points plus durs à attraper...

            /*QLineF l(this->coord(), event->scenePos());
            if(l.dx() > 0){
                this->moveTo(QPointF(this->coord().x()-10, this->coord().y()));
            }else{
                this->moveTo(QPointF(this->coord().x()+10, this->coord().y()));
            }
            if(l.dy() > 0){
                this->moveTo(QPointF(this->coord().x(), this->coord().y()-10));
            }else{
                this->moveTo(QPointF(this->coord().x(), this->coord().y()+10));
            }*/


        }//if

}


void BoundingPoint::hoverLeaveEvent(QGraphicsSceneHoverEvent* event){
    this->setMouseOnPoint(false);
    _scene->boundingPointIsHighlighted(this, false);
    _scene->setCanCreateSelectionRect(true);

}

/*
void BoundingPoint::dragEnterEvent(QGraphicsSceneDragDropEvent *event){
    qDebug("enter drag");
    event->setAccepted(true);
    _scene->update(_scene->sceneRect());
}*/







