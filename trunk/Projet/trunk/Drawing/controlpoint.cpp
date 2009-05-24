#include "controlpoint.h"
#include "brline.h"

//PUBLIC

ControlPoint::ControlPoint(qreal x, qreal y, BrLine* line, PaintingScene* scene)
    : QGraphicsItem(), _canMove(false), _hasFirstTangent(false),
    _hasSecondTangent(false),
    _internalKey(Data::MONOFIN_SURFACE_NO_CONTROL_POINT),
    _isHighlighted(false), _isMoving(false),
    _line(line), _scene(scene){

    _colorWhenNormal = _scene->getColor(PaintingScene::ControlPointColor,
                                        PaintingScene::NormalColor);
    _colorWhenHighlighted = _scene->getColor(PaintingScene::ControlPointColor,
                                        PaintingScene::HighlightingColor);

    this->setAcceptHoverEvents(true);

    _pos = new QPointF(x, y);

}

ControlPoint::ControlPoint(const QPointF& coord, BrLine* line, PaintingScene* scene)
    : QGraphicsItem(), _canMove(false), _hasFirstTangent(false),
    _hasSecondTangent(false),
    _internalKey(Data::MONOFIN_SURFACE_NO_CONTROL_POINT),
    _isHighlighted(false), _isMoving(false),
    _line(line), _scene(scene){

    _colorWhenNormal = _scene->getColor(PaintingScene::ControlPointColor,
                                        PaintingScene::NormalColor);
    _colorWhenHighlighted = _scene->getColor(PaintingScene::ControlPointColor,
                                        PaintingScene::HighlightingColor);

    this->setAcceptHoverEvents(true);

    _pos = new QPointF(coord);

}

ControlPoint::~ControlPoint(){
    delete _pos;
}

bool ControlPoint::isAllowedToMoveAtPosX(qreal posX){
    QRectF zone = _scene->sceneRect();
    return (posX > zone.left() && posX < zone.right());
}

bool ControlPoint::isAllowedToMoveAtPosY(qreal posY){
    QRectF zone = _scene->sceneRect();
    return (posY > 0 && posY < zone.bottom());
}

void ControlPoint::moveTo(const QPointF& p){
    QPointF pos(p);
    QRectF zone = _scene->sceneRect();

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
}

void ControlPoint::paint(QPainter* painter,
                         const QStyleOptionGraphicsItem* option,
                         QWidget* widget){
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if(!_scene->isSimplifyViewActivated()){

        if(!_isHighlighted || _scene->isRenderingPicture()){
            painter->setPen(_colorWhenNormal);
        }else{
            painter->setPen(_colorWhenHighlighted);
        }
        painter->drawEllipse(*_pos, this->rectangleSize()/2.0,
                             this->rectangleSize()/2.0);
    }else{
        Q_UNUSED(painter);
    }
}

void ControlPoint::setColorWhenHighlighted(const QColor& color){
    if (color.isValid()){
        _colorWhenHighlighted = color;
    }
}

void ControlPoint::setColorWhenNormal(const QColor& color){
    if (color.isValid()){
        _colorWhenNormal = color;
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

bool ControlPoint::willMoveToGoCloseToPosX(qreal posX){
    QRectF zone = _scene->sceneRect();
    if(posX < zone.left()){
        return _pos->x() != zone.left();
    }else if(posX > zone.right()){
        return _pos->x() != zone.right();
    }else{
        return _pos->x() != posX;
    }
}

bool ControlPoint::willMoveToGoCloseToPosY(qreal posY){
    QRectF zone = _scene->sceneRect();
    if(posY < 0){
        return _pos->y() != 0;
    }else if(posY > zone.bottom()){
        return _pos->y() != zone.bottom();
    }else{
        return _pos->y() != posY;
    }
}


//PROTECTED

void ControlPoint::hoverEnterEvent(QGraphicsSceneHoverEvent* event){
    qDebug("control hover");
    _scene->setCanCreateSelectionRect(false);
    _isHighlighted = true;
    _scene->update(_scene->sceneRect());
}

void ControlPoint::hoverLeaveEvent(QGraphicsSceneHoverEvent* event){
    _scene->setCanCreateSelectionRect(true);
    _isHighlighted = false;
    _scene->update(_scene->sceneRect());
}

void ControlPoint::mouseMoveEvent(QGraphicsSceneMouseEvent* event){
    if(_canMove){

        _isMoving = true;
        this->moveTo(event->scenePos());

    }else{
        QGraphicsItem::mousePressEvent(event);
    }

    _scene->update(_scene->sceneRect());
 }

void ControlPoint::mousePressEvent(QGraphicsSceneMouseEvent* event){
    if(event->button() == Qt::LeftButton){

//REMOVE CONTROL POINT

        if(_scene->state() == PaintingScene::RemoveControlPointsState){
            _line->setControlPoint(false);

            _scene->controlPointHasBeenHidden(this);

            //si on supprime un point de contrôle, celui-ci ne peut pas rétablir
            //la variable _canCreateSelectionRect à true, il faut donc le faire ici :
            _scene->setCanCreateSelectionRect(true);

            _isHighlighted = false;

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
