#include "boundingpoint.h"
#include "brline.h"
#include "paintingscene.h"

//PUBLIC

BoundingPoint::BoundingPoint(qreal x, qreal y, PaintingScene* scene):
    QGraphicsItem(), _atLeastOneClick(false), _canMove(false),
    _hasLeftLine(false), _hasRightLine(false),
    _internalKey(Data::MONOFIN_SURFACE_NOT_CREATED_POINT),
    _isMouseOnPoint(false),  _isMoving(false),
    _scene(scene){

    _colorWhenNormal = _scene->getColor(PaintingScene::BoundingPointColor,
                                        PaintingScene::NormalColor);
    _colorWhenHighlighted = _scene->getColor(PaintingScene::BoundingPointColor,
                                        PaintingScene::HighlightingColor);
    _colorWhenSelected = _scene->getColor(PaintingScene::BoundingPointColor,
                                        PaintingScene::SelectionColor);

    this->setAcceptHoverEvents(true);
    //this->setFlag(QGraphicsItem::ItemIsSelectable, true);

    _pos = new QPointF(x, y);

}

BoundingPoint::BoundingPoint(const QPointF& coord, PaintingScene* scene):
        QGraphicsItem(), _atLeastOneClick(false), _canMove(false),
    _hasLeftLine(false), _hasRightLine(false),
    _internalKey(Data::MONOFIN_SURFACE_NOT_CREATED_POINT),
    _isMouseOnPoint(false),  _isMoving(false),
    _scene(scene){

    _colorWhenNormal = _scene->getColor(PaintingScene::BoundingPointColor,
                                        PaintingScene::NormalColor);
    _colorWhenHighlighted = _scene->getColor(PaintingScene::BoundingPointColor,
                                        PaintingScene::HighlightingColor);
    _colorWhenSelected = _scene->getColor(PaintingScene::BoundingPointColor,
                                        PaintingScene::SelectionColor);

    this->setAcceptHoverEvents(true);
    //this->setFlag(QGraphicsItem::ItemIsSelectable, true);

    _pos = new QPointF(coord);
}

BoundingPoint::~BoundingPoint(){
    delete _pos;
    //les objets BrLine* sont détruits dans la PaintingScene
}

void BoundingPoint::alignTangents(){
    //we get the old position of the point
    QPointF oldPos = this->coord();


    //the first step is to initialize the two points around this one,
    //with which this point will be aligned

    //the left one : we try to get the first point at the left of this one,
    //which is a control point, or a non-selected point. If a such point
    //doesn't exist, we leave the loop with a state indicating that there
    //is no point found on the left
    QPointF leftContr;

    //at the same time, we calculate the length of all the lines between this
    //point and the chosen one on the left (for later)
    qreal leftLength = 0;
    qreal totalLength = 0;//the total length is the length on the left and
                          //the length on the right



    //some variables are initialized for the loop
    BoundingPoint* currentPoint = this;
    bool finished = false;
    bool hasFoundALeftPoint = false;

    //if the point has a left line, we try to find a left point to align
    //this one (as explained before)
    if(_hasLeftLine){
        BrLine* currentLine = this->_leftLine;

        //the loop to find a left point
        while(!finished){

            //if the line has a control point, it is chosen to be the left point
            if(currentLine->isControlPointActivated()){
                leftContr = currentLine->coordControlPoint();

                //we add to the left length the length between the last
                //current point and the control point
                leftLength += QLineF(leftContr, currentPoint->coord()).length();

                //we state that a left point has been found and that
                //the loop is finished
                hasFoundALeftPoint = true;
                finished = true;


            }else{
                //if the line has no control point

                //we get the other point of the line as the current point
                currentPoint = currentLine->otherPointAs(currentPoint);
                if(currentPoint == 0){
                    qDebug("Big problem with the links between line and point while we align tangents (left line, problem with point)");
                    return;
                }

                //we add to the left length the entire length of the line
                leftLength += currentLine->length();

                //if the other point of the line is not selected, it is
                //chosen to be the left point
                if(!currentPoint->isSelected()){
                    leftContr = currentPoint->coord();

                    //we state that a left point has been found and that
                    //the loop is finished
                    hasFoundALeftPoint = true;
                    finished = true;
                }else{
                    //if the other point is selected, we have two cases :

                    //First case :
                    //if the selected point is a bounding point, we continue
                    //the loop from this point
                    if(currentPoint->type() == BoundingPoint::Type){
                        currentLine = currentPoint->otherLineAs(currentLine);
                        if(currentLine == 0){
                            qDebug("Big problem with the links between line and point while we align tangents (left line, problem with line)");
                            return;
                        }

                    //Second case :
                    //if the selected point is an extremity point, we can not
                    //go further, then the loop is finished and we indicate
                    //that no left point has been found
                    }else if(currentPoint->type() == ExtremityPoint::Type){
                        hasFoundALeftPoint = false;
                        finished = true;
                    }else{
                        qDebug("Big problem : a point is not a bounding point or an extremity point (while we align tangents)");
                        return;
                    }
                }
            }

        }//while

        //to finish, we set the total length to the left one
        //the right length will be added next
        totalLength = leftLength;
    }//if




    //we do the same loop to find the right point
    QPointF rightContr;

    currentPoint = this;
    finished = false;
    bool hasFoundARightPoint = false;

    //the loop is the same with 'right' instead of 'left', and we
    //add the length of the lines to the total length
    if(_hasRightLine){
        BrLine* currentLine = this->_rightLine;
        while(!finished){

            if(currentLine->isControlPointActivated()){
                rightContr = currentLine->coordControlPoint();
                totalLength += QLineF(rightContr, currentPoint->coord()).length();
                hasFoundARightPoint = true;
                finished = true;


            }else{
                currentPoint = currentLine->otherPointAs(currentPoint);
                if(currentPoint == 0){
                    qDebug("Big problem with the links between line and point while we align tangents (right line, problem with point)");
                    return;
                }
                totalLength += currentLine->length();


                if(!currentPoint->isSelected()){
                    rightContr = currentPoint->coord();
                    hasFoundARightPoint = true;
                    finished = true;


                }else{
                    if(currentPoint->type() == BoundingPoint::Type){
                        currentLine = currentPoint->otherLineAs(currentLine);
                        if(currentLine == 0){
                            qDebug("Big problem with the links between line and point while we align tangents (right line, problem with line)");
                            return;
                        }

                    }else if(currentPoint->type() == ExtremityPoint::Type){
                        hasFoundARightPoint = false;
                        finished = true;

                    }else{
                        qDebug("Big problem : a point is not a bounding point or an extremity point (while we align tangents)");
                        return;
                    }
                }
            }

        }//while
    }//if

    //if no right point has been found, we set it to a point on the same
    //x-coordinate as the left point, but in y-coordinate 0 (it will be
    //aligned on the symmetric of the left point)
    if(hasFoundALeftPoint && !hasFoundARightPoint){
        rightContr = QPointF(leftContr.x(), 0);

    //if no left point has been found, we do the same kind of operation
    }else if(hasFoundARightPoint && !hasFoundALeftPoint){
        leftContr = QPointF(rightContr.x(), 0);

    //if no points have been found, we choose the left point and the right
    //point as the same as the current point. The result is that when all
    //points are selected, if there are no control points, nothing will happen
    }else if(!hasFoundARightPoint && !hasFoundALeftPoint){
        rightContr = this->coord();
        leftContr = this->coord();
    }

    //The second step of the algorithm is to correctly place the point

    //two cases :
    //first case, if we are on an extremity point :
    if(!_hasLeftLine || !_hasRightLine){
        qreal x = 0;

        //if it has a left line, it will be placed under the found left point
        if(_hasLeftLine){
            x = leftContr.x();

        //if it has a right line, it will be placed under the found right point
        }else if(_hasRightLine){
            x = rightContr.x();
        }

        //it is placed on the calculated x and 0 for y, and the magnet
        //is ignored
        this->moveTo(QPointF(x, this->coord().y()), false);


    //second case : if we are on a bounding point :
    }else{

        //with the calculated lengths, we make a division to know at which
        //percentage of the line between the left point and the right point,
        //this one will be placed
        qreal t;
        if(totalLength == 0){ //to avoid division by 0
            t = 0;
        }else{
            t = leftLength / (totalLength);
        }

        //and we set the new position of this point to the calculated
        //percentage of the line between the left point and the right one
        //(the magnet is ignored)
        QLineF l(leftContr, rightContr);
        QPointF pos = l.pointAt(t);
        this->moveTo(pos, false);
    }


    //if the result of the operation is that the point has moved,
    //we notify the scene of this action
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

void BoundingPoint::moveTo(const QPointF& p, bool useMagnetIfActivated){
    this->prepareGeometryChange();
    QPointF pos = p;

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
        pos.setY((int)((pos.y()+ gu/2.0) / gu) * gu);

    }

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

BrLine* BoundingPoint::otherLineAs(BrLine* l){
    BrLine* res = 0;
    if(_hasLeftLine && _hasRightLine){

        if(l == _leftLine){
            res = _rightLine;
        }else if(l == _rightLine){
            res = _leftLine;
        }

    }//if
    return res;
}

void BoundingPoint::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget){
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if(!_scene->isSimplifyViewActivated()){
        QPen pen;

        if(_scene->isRenderingPicture()){
            pen.setColor(_colorWhenNormal);
        }else if(this->isSelected()){
            pen.setColor(_colorWhenSelected);
        }else{
            if(_isMouseOnPoint){
                pen.setColor(_colorWhenHighlighted);
                pen.setWidthF(2.2);
            }else{
                pen.setColor(_colorWhenNormal);
            }
        }
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

void BoundingPoint::setColorWhenHighlighted(const QColor& color){
    if (color.isValid()){
        _colorWhenHighlighted = color;
    }
}

void BoundingPoint::setColorWhenNormal(const QColor& color){
    if (color.isValid()){
        _colorWhenNormal = color;
    }
}

void BoundingPoint::setColorWhenSelected(const QColor& color){
    if (color.isValid()){
        _colorWhenSelected = color;
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

/*bool BoundingPoint::willMoveToGoCloseToPosX(qreal posX){
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
}*/

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
    Q_UNUSED(event);
    if(_scene->state() == PaintingScene::NormalState){

        _scene->setCanCreateSelectionRect(false);
        this->setMouseOnPoint(true);
        _scene->boundingPointIsHighlighted(this, true);

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
    Q_UNUSED(event);
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







