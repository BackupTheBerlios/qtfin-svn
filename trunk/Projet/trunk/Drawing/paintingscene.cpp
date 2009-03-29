#include "paintingscene.h"
#include "symmetryaxis.h"
#include "ghostpoint.h"
#include "ghostline.h"
#include "brline.h"
#include "selectionrect.h"

//PUBLIC

PaintingScene::PaintingScene(qreal width, qreal height, QObject* parent)
    :QGraphicsScene(parent), _canCreateSelectionRect(true),
    _hasPlacedFirstPoint(false), _isAddControlActivated(false),
    _isAddPointActivated(false), _isCreateLineActivated(false),
    _isCreatingSelectionRect(false), _isOneSelection(false),
    _isRemoveControlPointActivated(false), _scaleFactor(1){

     this->setSceneRect(0,-height/2,width, height);

    _axis = new SymmetryAxis(this);
    this->addItem(_axis);
}

void PaintingScene::addControlPoint(ControlPoint* p, int i){
    _controlPointsList.insert(i, p);
}

BrLine* PaintingScene::createLine(BoundingPoint* p1, BoundingPoint* p2){
    int i = _pointList.indexOf(p1);
    BrLine* l;
    l = new BrLine(p1,p2,this, i);
    p1->setRightLine(l);
    p2->setLeftLine(l);
    _lineList.insert(i, l);
    this->addItem(l);
    this->update(this->sceneRect());
    return l;
}

QRectF PaintingScene::pointsBoundingZone(){
    return this->sceneRect().adjusted(20,20,-20,-20);
}

void PaintingScene::removeAllPoints(){
    BoundingPoint* p;
    foreach(p, _pointList){
        p->removeLeftLine();
        p->removeRightLine();
        _pointList.removeOne(p);
        this->removeItem(p);
        delete p;
    }

}

void PaintingScene::removeControlPoint(ControlPoint* p){
    _controlPointsList.removeOne(p);
    this->removeItem(p);
}

void PaintingScene::removeLine(BrLine* l){
    _lineList.removeOne(l);
    this->removeItem(l);
    delete l;
}

void PaintingScene::removePoint(BoundingPoint* p){
    p->removeLeftLine();
    p->removeRightLine();
    int i = _pointList.indexOf(p);
    this->updateLinesId();
    _pointList.removeAt(i);
    this->removeItem(p);
    delete p;
    if(i != 0 && i != _pointList.size()){
        this->createLine(_pointList.at(i-1), _pointList.at(i));
        qDebug("createLine");
    }
}

void PaintingScene::scale(qreal factor){
    _scaleFactor *= factor;
    qDebug("New scale factor : %f", _scaleFactor);
}


//PUBLIC SLOTS

void PaintingScene::activateCreateLine(bool a){
    if(a){
        _ghostPoint = new GhostPoint(QPointF(this->pointsBoundingZone().bottomLeft().x(),0), this);
        this->addItem(_ghostPoint);
        _hasPlacedFirstPoint = false;
    }else{
        this->removeItem(_ghostPoint);
        delete _ghostPoint;
    }
    _isCreateLineActivated = a;
}

void PaintingScene::activateAddControl(bool a){
    _isAddControlActivated = a;
}

void PaintingScene::activateAddPoint(bool a){
    _isAddPointActivated = a;
}

void PaintingScene::activateRemoveControlPoint(bool a){
    _isRemoveControlPointActivated = a;
}

void PaintingScene::alignTangents(){
    QList<QGraphicsItem*> l = this->selectedItems();
        QGraphicsItem* item;
        foreach(item, l){
            if(item->type() == BoundingPoint::Type){
                qgraphicsitem_cast<BoundingPoint*>(item)->alignTangents();
            }
        }
}

void PaintingScene::cleanPoints(){
    this->removeAllPoints();
    this->update(this->sceneRect());
    _hasPlacedFirstPoint = false;
}

void PaintingScene::removeSelectedPoints(){
    QList<QGraphicsItem*> l = this->selectedItems();
    QGraphicsItem* item;
    foreach(item, l){
        if(item->type() == BoundingPoint::Type){
            this->removePoint(qgraphicsitem_cast<BoundingPoint*>(item));
        }//if
    }//foreach

}

void PaintingScene::showCoords(){
    BoundingPoint* p;
    foreach(p, _pointList){
        qDebug("(%f, %f)", p->coord().x(), p->coord().y());
    }
}


//PROTECTED


void PaintingScene::keyPressEvent(QKeyEvent* event){
    if(event->key() == Qt::Key_Delete){
        this->removeSelectedPoints();
    }
}

void PaintingScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event){
    QRectF r(event->scenePos().x() - 3, event->scenePos().y() - 3, 6,6);
    BoundingPoint* bp;
    _isOneSelection = false;

    if(_isCreateLineActivated){
        bool intersect = false;
        if(_hasPlacedFirstPoint){
            if(_pointList.isEmpty()){qDebug("empty list");}
            else{
                QLineF templine(_pointList.last()->coord(), _ghostPoint->coord());
                BrLine* bl;
                for(int i = 0; i < _lineList.size()-1; i++){
                    bl = _lineList.at(i);
                    if(bl->intersect(templine)){
                        intersect = true;
                        qDebug("intersect");
                    }
                }
            }

        _ghostLine->setCanBePlaced(!intersect);
        }

        _ghostPoint->setCanBePlaced(!intersect);

        QRectF zone = this->pointsBoundingZone();
        QPointF pos(event->scenePos());
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
        if(!_hasPlacedFirstPoint){
            pos.setY(0);
        }else{}
        _ghostPoint->moveTo(pos);
    }
    else if(_isCreatingSelectionRect){
        _selectionRect->setBottomRight(event->scenePos());
    }



    else{

        if(!(_isAddControlActivated ||
             _isAddPointActivated ||
             _isRemoveControlPointActivated)){

            foreach(bp, _pointList){
                bp->setMouseOnPoint(false);
                if(r.intersects(bp->boundingRect()) && !_isOneSelection){
                    bp->setMouseOnPoint(true);
                    _isOneSelection = true;
                    break;
                }
            }//foreach
        }//if

        /********************/
        if(_isAddControlActivated || _isAddPointActivated){
            BrLine* lf;
            foreach(lf, _lineList){
                lf->setMouseOnLine(false);
                /*for(qreal i = BEZIERSTEP; i <= 1 && !_isOneSelection; i += BEZIERSTEP){
                    if(r.intersects(QRectF(lf->cubicBezierPoint(i-BEZIERSTEP), lf->cubicBezierPoint(i)))){
                        lf->setMouseOnLine(true);
                        _isOneSelection = true;
                        break;
                    }
                }*/
                if(!_isOneSelection){
                    if(lf->intersects(r)){
                        lf->setMouseOnLine(true);
                        _isOneSelection = true;
                    }
                }
            }//foreach
        }
    }
    this->update(this->sceneRect());
    QGraphicsScene::mouseMoveEvent(event);
}

void PaintingScene::mousePressEvent(QGraphicsSceneMouseEvent* event){
    if(event->button() == Qt::LeftButton){

        qDebug("click");

        if(_isCreateLineActivated){
            if(_ghostPoint->canBePlaced()){
                BoundingPoint* rect;
                if(_hasPlacedFirstPoint){
                
                    QRectF mouseRect(_ghostPoint->coord().x()- 5,
                           _ghostPoint->coord().y()- 5,
                           10,
                           10);
                    if(mouseRect.intersects(QRectF(_axis->line().x1(),
                                                   _axis->line().y1()-5,
                                                   _axis->line().x2(),
                                                   _axis->line().y2() + 5))){
                        rect = new ExtremityPoint(QPointF(_ghostPoint->coord().x(), 0), this);
                        this->removeItem(_ghostPoint);
                        this->removeItem(_ghostLine);
                        emit lineFinished(true);
                    }else{
                        rect = new BoundingPoint(_ghostPoint->coord(), this);
                        _ghostLine->setP1(rect);
                    }
                }else{
                    rect = new ExtremityPoint(_ghostPoint->coord(), this);
                    _hasPlacedFirstPoint = true;
                    _ghostLine = new GhostLine(rect, _ghostPoint, this);
                    this->addItem(_ghostLine);
                    PaintingScene::mouseMoveEvent(event);
                    /*_ghostPoint->moveTo(event->scenePos());*/
                }

                addItem(rect);
                _pointList.append(rect);

                if(_pointList.size() > 1){

                    BoundingPoint* p1;
                    BoundingPoint* p2;
                    p1 = _pointList.at(_pointList.size() - 2);
                    p2 = _pointList.back();
                    this->createLine(p1, p2);

                    //QObject::connect(p1,SIGNAL(moving()),l,SLOT(move()));
                    //QObject::connect(p2,SIGNAL(moving()),l,SLOT(move()));
                }
            }
        }else


        if(_isAddControlActivated){
            BrLine* l;
            foreach(l, _lineList){
                if(l->isMouseOnLine()){
                    l->setControlPoint(true);
                }
            }
        }else

        if(_isAddPointActivated){
            BrLine* lf;
            foreach(lf, _lineList){
                if(lf->isMouseOnLine()){
                    int i = lf->lineId();
                    qDebug("line id = %d",i);
                    _pointList.at(i)->removeRightLine();
                    QRectF zone = this->pointsBoundingZone();
                    QPointF pos(event->scenePos());
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

                    BoundingPoint* bp = new BoundingPoint(pos, this);
                    this->addItem(bp);
                    _pointList.insert(i+1, bp);
                    this->createLine(_pointList.at(i), bp);
                    this->createLine(bp, _pointList.at(i+2));
                    this->updateLinesId();
                }
            }
        }else if(_isRemoveControlPointActivated){}
        else if(_isOneSelection){
            QPainterPath path(event->scenePos());
            path.addEllipse(event->scenePos(), 5, 5);
            this->setSelectionArea(path, Qt::IntersectsItemBoundingRect);
        }
        else if(_canCreateSelectionRect){
            qDebug("create selection rect");
            _isCreatingSelectionRect = true;
            _selectionRect = new SelectionRect(QRectF(event->scenePos(), event->scenePos()));
            this->addItem(_selectionRect);

        }
        else{

        }
        this->update(this->sceneRect());

        QGraphicsScene::mousePressEvent(event);
    }
}

void PaintingScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event){

    if(_isCreatingSelectionRect){
        _isCreatingSelectionRect = false;
        QPainterPath path(event->scenePos());
        //path.addEllipse(event->scenePos(), 2, 2);
        path.addRect(_selectionRect->rect());
        this->setSelectionArea(path, Qt::IntersectsItemBoundingRect);
        this->removeItem(_selectionRect);
        delete _selectionRect;
    }
}

void PaintingScene::updateLinesId(){
    BrLine* bl;
    int j = 0;
    foreach(bl, _lineList){
        bl->setLineId(j);
        j++;
    }
}






