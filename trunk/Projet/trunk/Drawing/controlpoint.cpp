#include "controlpoint.h"
#include "brline.h"

//PUBLIC

ControlPoint::ControlPoint(qreal x, qreal y, BrLine* line, PaintingScene* scene)
    : QGraphicsItem(), _canMove(false), _color(Qt::black),
    _line(line), _scene(scene){

    this->setAcceptHoverEvents(true);

    _rect = new QRectF(x + (BOUNDINPOINTSIZE / 2.0),
               y + (BOUNDINPOINTSIZE / 2.0),
               BOUNDINPOINTSIZE,
               BOUNDINPOINTSIZE);

}

ControlPoint::ControlPoint(const QPointF& coord, BrLine* line, PaintingScene* scene)
    : QGraphicsItem(), _canMove(false), _color(Qt::black),
    _line(line), _scene(scene){

    this->setAcceptHoverEvents(true);

    _rect = new QRectF(coord.x() - (BOUNDINPOINTSIZE / 2.0),
               coord.y() - (BOUNDINPOINTSIZE / 2.0),
               BOUNDINPOINTSIZE,
               BOUNDINPOINTSIZE);

}

ControlPoint::~ControlPoint(){
    delete _rect;
}

void ControlPoint::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget){
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if(!_scene->isSimplifyViewActivated()){
        painter->setPen(_color);
        //painter->setRenderHint(QPainter::Antialiasing);
        painter->drawEllipse(*_rect);
    }else{
        Q_UNUSED(painter);
    }
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

        QRectF zone = _scene->sceneRect();
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
        this->prepareGeometryChange();
        _rect->moveTo(pos.x() - (BOUNDINPOINTSIZE / 2.0),
                      pos.y() - (BOUNDINPOINTSIZE / 2.0));
        _tangent1->setLine(_line->line().p1(),_rect->center());
        _tangent2->setLine(_line->line().p2(),_rect->center());
    }else{
        QGraphicsItem::mousePressEvent(event);
    }
    _scene->update(_scene->sceneRect());
 }

void ControlPoint::mousePressEvent(QGraphicsSceneMouseEvent* event){
    if(event->button() == Qt::LeftButton){
        if(_scene->isRemoveControlPointActivated()){
            _line->setControlPoint(false);
            //si on supprime un point de contrôle, celui-ci ne peut pas rétablir
            //la variable _canCreateSelectionRect à true, il faut donc le faire ici :
            _scene->setCanCreateSelectionRect(true);
        }else{
            _canMove = true;
        }
    }else{
        _canMove = false;
        QGraphicsItem::mousePressEvent(event);
    }
}

void ControlPoint::mouseReleaseEvent(QGraphicsSceneMouseEvent* event){
    if(event->button() == Qt::LeftButton){
        _canMove = false;
    }else{
        _canMove = false;
        QGraphicsItem::mousePressEvent(event);
    }
    _scene->update(_scene->sceneRect());
}
