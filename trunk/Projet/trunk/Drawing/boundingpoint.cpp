#include "boundingpoint.h"
#include "brline.h"
#include "paintingscene.h"

//PUBLIC

BoundingPoint::BoundingPoint(qreal x, qreal y, PaintingScene* scene):
        QGraphicsItem(), _atLeastOneClick(false), _canMove(false),
      /*_canBeSelected(true), _isMoving(false),*/
     _color(Qt::black), _hasLeftLine(false), _hasRightLine(false),
     _isMouseOnPoint(false), _scene(scene){

    this->setAcceptHoverEvents(true);
    this->setFlag(QGraphicsItem::ItemIsMovable,true);
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);

    _rect = new QRectF(x - (BOUNDINPOINTSIZE / 2.0),
               y - (BOUNDINPOINTSIZE / 2.0),
               BOUNDINPOINTSIZE,
               BOUNDINPOINTSIZE);

}

BoundingPoint::BoundingPoint(const QPointF& coord, PaintingScene* scene):
        QGraphicsItem(), _atLeastOneClick(false), _canMove(false),
      /*_canBeSelected(true), _isMoving(false),*/
     _color(Qt::black), _hasLeftLine(false), _hasRightLine(false),
     _isMouseOnPoint(false), _scene(scene){

    this->setAcceptHoverEvents(true);
    this->setFlag(QGraphicsItem::ItemIsMovable,true);
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);

    _rect = new QRectF(coord.x() - (BOUNDINPOINTSIZE / 2.0),
               coord.y() - (BOUNDINPOINTSIZE / 2.0),
               BOUNDINPOINTSIZE,
               BOUNDINPOINTSIZE);
}

void BoundingPoint::alignTangents(){
    if(_hasLeftLine && _hasRightLine){
        QPointF leftContr = _leftLine->coordControlPoint();
        QPointF rightContr = _rightLine->coordControlPoint();
        QLineF l(leftContr, rightContr);
        //for(qreal t = 0; t <= 1; t+= 0.1){
        QPointF pos((leftContr.x()+rightContr.x())/2.0,
                    (leftContr.y()+rightContr.y())/2.0);
        //this->moveTo(l.pointAt(0.5));
        this->moveTo(pos);
        _scene->update(_scene->sceneRect());
    }
}

QRectF BoundingPoint::boundingRect() const{
    //qDebug("bounding rect : %f, %f, %f, %f", _rect->x(), _rect->y(), _rect->width(), _rect->height());
    return *_rect;

}

QPointF BoundingPoint::coord() const{
    return _rect->center();
}

void BoundingPoint::moveTo(const QPointF& p){
    this->prepareGeometryChange();
    _rect->moveCenter(p);/*moveTo(pos.x()- (BOUNDINPOINTSIZE / 2.0),
                  pos.y() - (BOUNDINPOINTSIZE / 2.0));*/

    if(_hasLeftLine) {_leftLine->move();}
    if(_hasRightLine) {_rightLine->move();}
}

void BoundingPoint::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget){
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QPen pen;
    if(this->isSelected()){
        _color = Qt::red;
        painter->setPen(_color);
        painter->drawEllipse(*_rect);
    }else{
        if(_isMouseOnPoint){
            _color = Qt::blue;
            pen.setWidthF(2.2);
        }else{
            _color = Qt::black;
        }
    }

    pen.setColor(_color);
    painter->setPen(pen);
    painter->drawRect(*_rect);

    //affichage du symétrique
    /*painter->setPen(Qt::black);
    painter->setOpacity(0.5);
    painter->drawRect(QRectF(_rect->bottomLeft().x(), -_rect->bottomLeft().y(), _rect->width(), _rect->height()));
    */
}

void BoundingPoint::removeLeftLine(){
    if(_hasLeftLine){
        _hasLeftLine = false;
        _leftLine->removeFromScene();
    }
}

void BoundingPoint::removeRightLine(){
    if(_hasRightLine){
        _hasRightLine = false;
        _rightLine->removeFromScene();
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

//PROTECTED

void BoundingPoint::mouseMoveEvent(QGraphicsSceneMouseEvent* event){
    if(_canMove){
        //_canBeSelected = false;
        //_isMoving = true;
        qDebug("mouse move");

        QRectF zone = _scene->pointsBoundingZone();
        QPointF pos(event->scenePos().x(), event->scenePos().y());
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

        this->moveTo(pos);

    }else{
        QGraphicsItem::mousePressEvent(event);
    }
    _scene->update(_scene->sceneRect());
 }

void BoundingPoint::mousePressEvent(QGraphicsSceneMouseEvent* event){
    if(event->button() == Qt::LeftButton){
        qDebug("mouse click");
        if(!this->isSelected()){_canMove = !_atLeastOneClick;}
        else{_canMove = true;}
        /*if(!_atLeastOneClick){
            this->setSelected(false);
        }*/

    }else{
        _canMove = false;
        QGraphicsItem::mousePressEvent(event);
    }
}

void BoundingPoint::mouseReleaseEvent(QGraphicsSceneMouseEvent* event){
    if(event->button() == Qt::LeftButton){
        qDebug("mouse release");
        _canMove = false;
        if(_atLeastOneClick){
            /*if(_canBeSelected){
                this->setSelected(true);
                qDebug("point selected");
                _scene->addSelectedPoint(this);
                _color = Qt::red;
                _canBeSelected = false;
            }else{
                if(this->isSelected() && !_isMoving){
                    this->setSelected(false);
                    qDebug("point unselected");
                    _scene->removeSelectedPoint(this);
                    _color = Qt::black;
                    _canBeSelected = true;
                }
                if(_isMoving){
                    _canBeSelected = true;
                }
            }*/
            /*if(this->isSelected()){
                this->setSelected(false);
                _color = Qt::black;
            }else{
                this->setSelected(true);
                _color = Qt::red;
            }*/
        }else{
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

/*void BoundingPoint::hoverEnterEvent(QGraphicsSceneHoverEvent* event){
    //qDebug("enter hover");

}
void BoundingPoint::hoverLeaveEvent(QGraphicsSceneHoverEvent* event){
    //qDebug("leave hover");

}

void BoundingPoint::dragEnterEvent(QGraphicsSceneDragDropEvent *event){
    qDebug("enter drag");
    event->setAccepted(true);
    _scene->update(_scene->sceneRect());
}*/







