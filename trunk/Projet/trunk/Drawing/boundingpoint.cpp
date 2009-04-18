#include "boundingpoint.h"
#include "brline.h"
#include "paintingscene.h"

//PUBLIC

BoundingPoint::BoundingPoint(qreal x, qreal y, PaintingScene* scene):
        QGraphicsItem(), _atLeastOneClick(false), _canMove(false),
      /*_canBeSelected(true),*/
     _color(Qt::black), _hasLeftLine(false), _hasRightLine(false),
     _internalKey(PaintingScene::BADKEY),
     _isMouseOnPoint(false),  _isMoving(false),
     _scene(scene){

    this->setAcceptHoverEvents(true);
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);

    _pos = new QPointF(x, y);

}

BoundingPoint::BoundingPoint(const QPointF& coord, PaintingScene* scene):
        QGraphicsItem(), _atLeastOneClick(false), _canMove(false),
      /*_canBeSelected(true),*/
     _color(Qt::black), _hasLeftLine(false), _hasRightLine(false),
     _internalKey(PaintingScene::BADKEY),
     _isMouseOnPoint(false),  _isMoving(false),
     _scene(scene){

    this->setAcceptHoverEvents(true);
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);

    _pos = new QPointF(coord);
}

BoundingPoint::BoundingPoint(const BoundingPoint& p):
        _atLeastOneClick(p._atLeastOneClick), _canMove(p._canMove),
     _color(p._color), _hasLeftLine(false), _hasRightLine(false),
     _isMouseOnPoint(p._isMouseOnPoint), _isMoving(p._isMoving),
     _scene(p._scene){

    this->setAcceptHoverEvents(true);
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);

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

int BoundingPoint::internalKeyLeftLine(){
    if(_hasLeftLine){
        return _leftLine->internalKey();
    }else{
        return PaintingScene::BADKEY;
    }
}

void BoundingPoint::moveTo(const QPointF& p){
    this->prepareGeometryChange();
    _pos->setX(p.x());
    _pos->setY(p.y());

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

//PROTECTED

void BoundingPoint::mouseMoveEvent(QGraphicsSceneMouseEvent* event){
    if(_canMove){
        _isMoving = true;
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
        //QGraphicsItem::mousePressEvent(event);
    }
    _scene->update(_scene->sceneRect());
 }

void BoundingPoint::mousePressEvent(QGraphicsSceneMouseEvent* event){
    if(event->button() == Qt::LeftButton){
        qDebug("click on bounding point");
        if(!this->isSelected()){_canMove = false;}//!_atLeastOneClick;}
        else{_canMove = true;}

    }else{
        _canMove = false;
        QGraphicsItem::mousePressEvent(event);
    }
}

void BoundingPoint::mouseReleaseEvent(QGraphicsSceneMouseEvent* event){
    if(event->button() == Qt::LeftButton){
        qDebug("mouse released of bounding point");
        _canMove = false;

        if(_isMoving){
            _isMoving = false;
            _scene->boundingPointHasMoved(this);
        }

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

inline qreal BoundingPoint::rectangleSize() const{
    return BOUNDINGPOINTSIZE;// / _scene->scaleFactor();
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







