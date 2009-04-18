#include "controlpoint.h"
#include "brline.h"

//PUBLIC

ControlPoint::ControlPoint(qreal x, qreal y, BrLine* line, PaintingScene* scene)
    : QGraphicsItem(), _canMove(false), _color(Qt::black),
    _hasFirstTangent(false), _hasSecondTangent(false),
    _isMoving(false),
    _line(line), _scene(scene){

    this->setAcceptHoverEvents(true);

    _pos = new QPointF(x, y);

}

ControlPoint::ControlPoint(const QPointF& coord, BrLine* line, PaintingScene* scene)
    : QGraphicsItem(), _canMove(false), _color(Qt::black),
    _hasFirstTangent(false), _hasSecondTangent(false),
    _isMoving(false),
    _line(line), _scene(scene){

    this->setAcceptHoverEvents(true);

    _pos = new QPointF(coord);

}

ControlPoint::~ControlPoint(){
    delete _pos;
}

void ControlPoint::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget){
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if(!_scene->isSimplifyViewActivated()){
        painter->setPen(_color);
        //painter->setRenderHint(QPainter::Antialiasing);
        painter->drawEllipse(*_pos, CONTRPOINTHEIGHT/2.0, CONTRPOINTHEIGHT/2.0);
    }else{
        Q_UNUSED(painter);
    }
}

void ControlPoint::setTangent1(Tangent* t){
    _hasFirstTangent = true;
    _tangent1 = t;
}

void ControlPoint::setTangent2(Tangent* t){
    _hasSecondTangent = true;
    _tangent2 = t;
}


//PROTECTED

void ControlPoint::hoverEnterEvent(QGraphicsSceneHoverEvent* event){
    qDebug("control hover");
    _scene->setCanCreateSelectionRect(false);
    _color = Qt::green;
    _scene->update(_scene->sceneRect());
}

void ControlPoint::hoverLeaveEvent(QGraphicsSceneHoverEvent* event){
    _scene->setCanCreateSelectionRect(true);
    _color = Qt::black;
    _scene->update(_scene->sceneRect());
}

void ControlPoint::mouseMoveEvent(QGraphicsSceneMouseEvent* event){
    if(_canMove){

        _isMoving = true;

        QRectF zone = _scene->sceneRect();
        QPointF pos(event->scenePos().x(), event->scenePos().y());

       //on contrôle que le point ne dépasse pas sa zone
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

       //on bouge le point
        this->prepareGeometryChange();

        _pos->setX(pos.x());
        _pos->setY(pos.y());

       //on met à jour les tangentes
        if(_hasFirstTangent){
            _tangent1->setLine(_line->line().p1(), this->coord());
        }
        if(_hasSecondTangent){
            _tangent2->setLine(_line->line().p2(), this->coord());
        }

    }else{
        QGraphicsItem::mousePressEvent(event);
    }

    _scene->update(_scene->sceneRect());
 }

void ControlPoint::mousePressEvent(QGraphicsSceneMouseEvent* event){
    if(event->button() == Qt::LeftButton){

//REMOVE CONTROL POINT

        if(_scene->isRemoveControlPointActivated()){
            _line->setControlPoint(false);
            //si on supprime un point de contrôle, celui-ci ne peut pas rétablir
            //la variable _canCreateSelectionRect à true, il faut donc le faire ici :
            _scene->setCanCreateSelectionRect(true);

//MOVE POINT

        }else{
            _canMove = true;
        }

//right click
    }else{
        _canMove = false;
        QGraphicsItem::mousePressEvent(event);
    }
}

void ControlPoint::mouseReleaseEvent(QGraphicsSceneMouseEvent* event){
    if(event->button() == Qt::LeftButton){
        _canMove = false;

        if(_isMoving){
            _isMoving = false;
            _scene->controlPointHasMoved(this);
        }

    }else{
        _canMove = false;
        QGraphicsItem::mousePressEvent(event);
    }
    _scene->update(_scene->sceneRect());
}
