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
    _isCreatingSelectionRect(false), _isMultiSelectionKeyActivated(false),
    _isOneSelection(false), _isRemoveControlPointActivated(false),
    _isSimplifyViewActivated(false), _scaleFactor(1){

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
        //Le point appelle des fonctions de destruction sur chacune
        //de ses lignes
        p->removeLeftLine();
        p->removeRightLine();

        //on enlève le point de la liste, de la scène et on le détruit
        _pointList.removeOne(p);
        this->removeItem(p);
        delete p;
    }//foreach
    qDebug("all points are removed");
}

void PaintingScene::removeControlPoint(ControlPoint* p, bool isInScene){
    //on retire le point de contrôle de la liste, de la scène
    //s'il y est et on le détruit
    _controlPointsList.removeOne(p);
    if(isInScene){this->removeItem(p);}
    delete p;
}

void PaintingScene::removeLine(BrLine* l){
    //on retire la ligne de la liste, ainsi que de la scène
    _lineList.removeOne(l);
    this->removeItem(l);
    //on appelle une fonction détruisant le point de contrôle et les tangentes
    //et on détruit la ligne
    l->deleteBezier();
    delete l;
    //on remet à jour l'indice de toutes les lignes
    this->updateLinesId();
}

void PaintingScene::removePoint(BoundingPoint* p){
    //Le point appelle des fonctions de destruction sur chacune de ses lignes
    p->removeLeftLine();
    p->removeRightLine();

    //on enlève le point de la liste de points, ainsi que de la scène
    int i = _pointList.indexOf(p);
    _pointList.removeAt(i);
    this->removeItem(p);
    //Destruction du point et du GraphicsItem qu'il contient
    delete p;

    //si le point n'est pas aux extémités, on relie les deux points autour
    //de celui supprimé, par une ligne
    if(i != 0 && i != _pointList.size()){
        this->createLine(_pointList.at(i-1), _pointList.at(i));
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
    }else{}
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
            }else if(item->type() == ExtremityPoint::Type){
                qgraphicsitem_cast<ExtremityPoint*>(item)->alignTangents();
            }
        }//foreach
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

void PaintingScene::savePicture(){
    unsigned int x = 800;
    unsigned int y = 600;

    QPixmap pixmap(x,y);
    pixmap.fill(Qt::white);

    QPainter painter;
    painter.begin(&pixmap);
    painter.translate(0,y);
    painter.rotate(0);
    painter.scale(1,-1);
    bool simpl = _isSimplifyViewActivated;
    this->simplifyView(false);
    this->render(&painter/*, QRectF(), QRectF(this->sceneRect().topLeft().x()-5000,
                                            this->sceneRect().topLeft().y()-5000,
                                            this->sceneRect().width()+10000,
                                            this->sceneRect().height()+10000)*/);
    painter.end();
    this->simplifyView(simpl);
    qDebug("save pixmap");
    pixmap.save("test.png","PNG");
}

void PaintingScene::showCoords(){
    BoundingPoint* p;
    foreach(p, _pointList){
        qDebug("(%f, %f)", p->coord().x(), p->coord().y());
    }
}

void PaintingScene::simplifyView(bool a){
    _isSimplifyViewActivated = a;
    this->update(this->sceneRect());
}

void PaintingScene::stopCreateLine(){
    if(_isCreateLineActivated){
        _isCreateLineActivated = false;
        this->removeItem(_ghostPoint);
        this->removeItem(_ghostLine);
        delete _ghostPoint;
        delete _ghostLine;
        this->removeAllPoints();
        emit this->lineInterrupted();
        this->update(this->sceneRect());
    }
}

//PROTECTED


void PaintingScene::keyPressEvent(QKeyEvent* event){
    if(event->key() == Qt::Key_Delete){
        qDebug("suppr");
        this->removeSelectedPoints();
    }else if(event->key() == Qt::Key_Control){
        _isMultiSelectionKeyActivated = true;
    }else if(event->key() == Qt::Key_P){
        this->savePicture();
    }else if(event->key() == Qt::Key_Escape){
        this->stopCreateLine();
    }
}

void PaintingScene::keyReleaseEvent(QKeyEvent* event){
    if(event->key() == Qt::Key_Control){
        _isMultiSelectionKeyActivated = false;
    }
}

void PaintingScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event){
    //qDebug("(%f,%f)", event->scenePos().x(), event->scenePos().y());
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

            //QRectF r(event->scenePos().x(), event->scenePos().y(), 0.1, 0.1);
            foreach(bp, _pointList){
                bp->setMouseOnPoint(false);
                if(bp->boundingRect().contains(event->scenePos()) && !_isOneSelection){
                    bp->setMouseOnPoint(true);
                    _isOneSelection = true;
                    _itemUnderMouse = bp;
                    //break;
                }
            }//foreach
        }//if

        /********************/
        if(_isAddControlActivated || _isAddPointActivated){
            QRectF r(event->scenePos().x() - 3, event->scenePos().y() - 3, 6, 6);
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
        }//if
    }
    this->update(this->sceneRect());
    QGraphicsScene::mouseMoveEvent(event);
}

void PaintingScene::mousePressEvent(QGraphicsSceneMouseEvent* event){
    if(event->button() == Qt::LeftButton){

        qDebug("click");

//1. CREATE LINE
        if(_isCreateLineActivated){
            if(_ghostPoint->canBePlaced()){
                BoundingPoint* rect; //pointeur vers le nouveau point
                                    //qui sera ajouté à la scène et à la liste
                if(_hasPlacedFirstPoint){
                
                    QRectF mouseRect(_ghostPoint->coord().x()- 5,
                           _ghostPoint->coord().y()- 5,
                           10,
                           10);
                    if(mouseRect.intersects(QRectF(_axis->line().x1(),
                                                   _axis->line().y1()-5,
                                                   _axis->line().x2(),
                                                   _axis->line().y2() + 5))){
            //Last point :
                        rect = new ExtremityPoint(QPointF(_ghostPoint->coord().x(), 0), this);
                        this->removeItem(_ghostPoint);
                        this->removeItem(_ghostLine);
                        delete _ghostPoint;
                        delete _ghostLine;
                        emit lineFinished(true);
                        qDebug("last point created");
                    }else{
            //Middle points :
                        //on place le nouveau point aux coordonnées du ghost point
                        rect = new BoundingPoint(_ghostPoint->coord(), this);
                        //on définit le nouveau point de départ de la ghost line
                        _ghostLine->setP1(rect);
                    }
                }else{
             //First Point :
                    rect = new ExtremityPoint(_ghostPoint->coord(), this);
                    _hasPlacedFirstPoint = true;
                    _ghostLine = new GhostLine(rect, _ghostPoint, this);
                    this->addItem(_ghostLine);
                    PaintingScene::mouseMoveEvent(event);
                }
            //All points :
                this->addItem(rect);
                _pointList.append(rect);

                if(_pointList.size() > 1){

                    BoundingPoint* p1;
                    BoundingPoint* p2;
                    p1 = _pointList.at(_pointList.size() - 2);
                    p2 = _pointList.back();
                    this->createLine(p1, p2);
                    qDebug("number of points : %d", _pointList.size());
                }
            }//end if (can be placed)
        }else

//2. ADD CONTROL POINT
        if(_isAddControlActivated){
            BrLine* l;
            foreach(l, _lineList){
                if(l->isMouseOnLine()){
                    l->setControlPoint(true);
                }
            }
        }else

//3. INSERT POINT
        if(_isAddPointActivated){
            BrLine* lf;
            foreach(lf, _lineList){
                if(lf->isMouseOnLine()){
                    int i = lf->lineId();
                    qDebug("line id = %d",i);

                    if(!lf->isControlPointActivated()){
                    //si ce n'est pas une courbe de bézier
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
                    }else{
                        QLineF tangent1 = lf->tangent1();
                        QLineF tangent2 = lf->tangent2();

                        qreal t = 0;
                        qreal dist = 10000;
                        QPointF mousePos(event->scenePos());
                        for(qreal j = 0; j <= 1; j+= 0.01){
                            qreal newDist = QLineF(lf->pointAt(j), mousePos).length();
                            if(newDist < dist){
                                dist = newDist;
                                t = j;
                            }
                        }

                        QPointF pos = lf->pointAt(t);

                        _pointList.at(i)->removeRightLine();

                        BoundingPoint* bp = new BoundingPoint(pos, this);
                        this->addItem(bp);
                        _pointList.insert(i+1, bp);
                        BrLine* l1 = this->createLine(_pointList.at(i), bp);
                        BrLine* l2 = this->createLine(bp, _pointList.at(i+2));
                        l1->moveControlPoint(tangent1.pointAt(t));
                        l1->setControlPoint(true);
                        l2->moveControlPoint(tangent2.pointAt(1-t));
                        l2->setControlPoint(true);
                    }
                    this->updateLinesId();
                    break;
                }
            }//foreach

//4. REMOVE CONTROL POINT
        }else if(_isRemoveControlPointActivated){}

//5. OTHER
//5.1 Something is highlighted
        else if(_isOneSelection){
            if(!_isMultiSelectionKeyActivated){
                QPainterPath path;
                path.addEllipse(event->scenePos(), 1, 1);
                qDebug("set selection area");
                this->setSelectionArea(path, Qt::IntersectsItemBoundingRect);
            }else{
                //this->clearSelection();
                _itemUnderMouse->setSelected(!_itemUnderMouse->isSelected());
            }
        }
//5.2 Nothing is highligthed
    //(selection rectangle)
        else if(_canCreateSelectionRect){
            qDebug("create selection rect");
            _isCreatingSelectionRect = true;
            _selectionRect = new SelectionRect(QRectF(event->scenePos(), event->scenePos()));
            this->addItem(_selectionRect);

        }
//6. ALL THE OTHER CASES
        else{

        }

//ALWAYS
        this->update(this->sceneRect());
        if(!_isCreatingSelectionRect){
            QGraphicsScene::mousePressEvent(event);
        }
        /*if(_isOneSelection){
            _itemUnderMouse->setSelected(true);
        }*/
    }//end if event->button == leftButton
}

void PaintingScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event){

    if(_isCreatingSelectionRect){
        _isCreatingSelectionRect = false;
        QList<QGraphicsItem*> selectedItems;
        if(_isMultiSelectionKeyActivated){
            selectedItems = this->selectedItems();
        }
        QPainterPath path(event->scenePos());
        path.addRect(_selectionRect->rect());
        this->setSelectionArea(path, Qt::IntersectsItemBoundingRect);
        QGraphicsItem* item;
        foreach(item, selectedItems){
            item->setSelected(true);
        }
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






