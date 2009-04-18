#include "paintingscene.h"
#include "symmetryaxis.h"
#include "ghostpoint.h"
#include "ghostline.h"
#include "brline.h"
#include "selectionrect.h"

using namespace Data;

//PUBLIC

PaintingScene::PaintingScene(qreal width,
                             qreal height,
                             ProjectFile* structure,
                             QObject* parent)
    :QGraphicsScene(parent), _canCreateSelectionRect(true),
    _firstPointKey(Data::MONOFIN_SURFACE_NOT_CREATED_POINT),
    _hasPlacedFirstPoint(false), _isAddControlActivated(false),
    _isAddPointActivated(false), _isCreateLineActivated(false),
    _isCreatingSelectionRect(false), _isMultiSelectionKeyActivated(false),
    _isOneSelection(false), _isRemoveControlPointActivated(false),
    _isSimplifyViewActivated(false), _lastPointKey(Data::MONOFIN_SURFACE_NOT_CREATED_POINT),
    _scaleFactor(1), _structure(structure){

     this->setSceneRect(0,-height/2,width, height);

     //Indispensable pour une scène dont les items bougent souvent
     this->setItemIndexMethod(QGraphicsScene::NoIndex);

    _axis = new SymmetryAxis(this);
    this->addItem(_axis);

}

PaintingScene::~PaintingScene(){
    if(_isCreateLineActivated){
        _isCreateLineActivated = false;
        this->removeItem(_ghostPoint);
        delete _ghostPoint;
        if(_hasPlacedFirstPoint){
            this->removeItem(_ghostLine);
            delete _ghostLine;
        }
    }
    this->removeAllPoints();
}

void PaintingScene::addControlPoint(ControlPoint* p, int i){
    _controlPointsList.insert(i, p);
}

void PaintingScene::boundingPointHasMoved(BoundingPoint* p, bool isUniqueModification){
    if(isUniqueModification){
        _structure->startHistory(Data::MonofinSurface);
    }

    _structure->setIntersectionPoint(p->internalKey(), p->coord().x(), p->coord().y());

    if(isUniqueModification){
        _structure->stopHistory(Data::MonofinSurface);
    }
}

void PaintingScene::controlPointHasMoved(ControlPoint* p, bool isUniqueModification){
    if(isUniqueModification){
        _structure->startHistory(Data::MonofinSurface);
    }

    _structure->setControlPoint(p->internalKey(), p->coord().x(), p->coord().y());

    if(isUniqueModification){
        _structure->stopHistory(Data::MonofinSurface);
    }
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
    //l->deleteBezier();
    delete l;
    //on remet à jour l'indice de toutes les lignes
    this->updateLinesId();
}

void PaintingScene::removePoint(BoundingPoint* p){    
    //on sauvegarde temporairement les coordonnées au cas où
    QPointF oldCoord = p->coord();

    //on récupère la clé interne de sa ligne de gauche
    int segmentKey = p->internalKeyLeftLine();

    //Le point appelle des fonctions de destruction sur chacune de ses lignes
    p->removeLeftLine();
    p->removeRightLine();

    //on enlève le point de la liste de points, ainsi que de la scène
    int i = _pointList.indexOf(p);
    _pointList.removeAt(i);
    this->removeItem(p);

    _structure->removeIntersectionPoint(p->internalKey());

    //Destruction du point et du GraphicsItem qu'il contient
    delete p;

    //si le point n'est pas aux extémités, on relie les deux points autour
    //de celui supprimé, par une ligne
    BrLine* newLine;
    if(i != 0 && i != _pointList.size()){
        newLine = this->createLine(_pointList.at(i-1), _pointList.at(i));
        newLine->setInternalKey(segmentKey);
        //newLine->setControlPoint(true);
        //newLine->moveControlPoint(oldCoord);
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

void PaintingScene::ajustSceneRect(qreal dx, qreal dy){
    QRectF newRect = this->sceneRect().adjusted(0, -dy, dx, dy);
    QRectF itemRect = this->itemsBoundingRect();

    if(newRect.contains(itemRect)){
        this->setSceneRect(newRect);
    }
    _axis->setLine(0,0,this->width(), 0);
    this->update(this->sceneRect());

}

void PaintingScene::alignTangents(){
    QList<QGraphicsItem*> l = this->selectedItems();
        QGraphicsItem* item;
        _structure->startHistory(Data::MonofinSurface);
        foreach(item, l){
            if(item->type() == BoundingPoint::Type){
                qgraphicsitem_cast<BoundingPoint*>(item)->alignTangents();
            }else if(item->type() == ExtremityPoint::Type){
                qgraphicsitem_cast<ExtremityPoint*>(item)->alignTangents();
            }
        }//foreach
        _structure->stopHistory(Data::MonofinSurface);
}

void PaintingScene::cleanPoints(){
    this->removeAllPoints();
    _structure->startHistory(Data::MonofinSurface);
    _structure->clearSurface();
    _structure->stopHistory(Data::MonofinSurface);
    this->update(this->sceneRect());
    _hasPlacedFirstPoint = false;
}

void PaintingScene::removeSelectedPoints(){
    QList<QGraphicsItem*> l = this->selectedItems();
    QGraphicsItem* item;
    _structure->startHistory(Data::MonofinSurface);
    foreach(item, l){
        if(item->type() == BoundingPoint::Type){
            this->removePoint(qgraphicsitem_cast<BoundingPoint*>(item));
        }//if
    }//foreach
    _structure->stopHistory(Data::MonofinSurface);
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

/*void PaintingScene::showCoords(){
    BoundingPoint* p;
    foreach(p, _pointList){
        qDebug("(%f, %f)", p->coord().x(), p->coord().y());
    }
}*/

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

        _structure->clearSurface();
        _structure->stopHistory(Data::MonofinSurface);

        emit this->lineInterrupted();
        this->update(this->sceneRect());
        
    }
}

void PaintingScene::redo(){
    qDebug("redo");
    _structure->redo(Data::MonofinSurface);

    this->getMonofinFromStructure();
}

void PaintingScene::undo(){
    qDebug("undo");
    _structure->undo(Data::MonofinSurface);

    this->getMonofinFromStructure();

}

//PROTECTED


void PaintingScene::getMonofinFromStructure(){
    //on enlève tous les points sans affecter la structure interne
    this->removeAllPoints();

    QList<int> l = _structure->getAllSegmentKeys();

/*
    float x;
    float y;
    int p1Key;
    int p2Key;
    int controlKey;


//on récupère le premier point
    if(l.size() > 0){

        _structure->getSegment(l.first(), p1Key, p2Key, controlKey);

        _structure->getIntersectionPoint(p1Key, x, y);
        ExtremityPoint* p = new ExtremityPoint(QPointF(x, y), this);
        this->addItem(p);
        _pointList.append(p);
        p->setInternalKey(p1Key);

    }



    for(int i = 0; i < l.size(); i++){
        _structure->getSegment(l.at(i), p1Key, p2Key, controlKey);

        _structure->getIntersectionPoint(p2Key, x, y);

        BoundingPoint* p;
        if(i == l.size()-1){
            p = new ExtremityPoint(QPointF(x, y), this);
        }else{
            p = new BoundingPoint(x, y, this);
        }
        this->addItem(p);
        _pointList.append(p);
        p->setInternalKey(p2Key);

        BoundingPoint* p1;
        p1 = _pointList.at(_pointList.size() - 2);
        BrLine* newLine = this->createLine(p1, p);
        newLine->setInternalKey(l.at(i));

        if(controlKey != Data::MONOFIN_SURFACE_NO_CONTROL_POINT){
            _structure->getControlPoint(controlKey, x, y);
            ControlPoint* cp = _controlPointsList.at(newLine->lineId());
            cp->setInternalKey(controlKey);
            newLine->setControlPoint(true);
            newLine->moveControlPoint(QPointF(x, y));
        }

    }//for
*/

    /*
    float x;
    float y;
    int p1Key;
    int p2Key;
    int controlKey;

    QList<int> placed;

    for(int i = 0; i < l.size(); i++){
        _structure->getSegment(l.at(i), p1Key, p2Key, controlKey);

        if(!placed.contains(p1Key)){
            placed.append(p1Key);

            _structure->getIntersectionPoint(p1Key, x, y);

            BoundingPoint* p;
            //if(i == l.size()-1){
            //    p = new ExtremityPoint(QPointF(x, y), this);
            //}else{
                p = new BoundingPoint(x, y, this);
            //}
            this->addItem(p);
            _pointList.append(p);
            p->setInternalKey(p1Key);
        }

        if(!placed.contains(p2Key)){
            placed.append(p2Key);

            _structure->getIntersectionPoint(p2Key, x, y);

            BoundingPoint* p;
            //if(i == l.size()-1){
            //    p = new ExtremityPoint(QPointF(x, y), this);
            //}else{
                p = new BoundingPoint(x, y, this);
            //}
            this->addItem(p);
            _pointList.append(p);
            p->setInternalKey(p1Key);
        }

    }//for
*/

    float x;
    float y;
    int p1Key;
    int p2Key;
    int controlKey;
    int lastPlacedKey;
    BoundingPoint* p;
    int size = l.size();

    if(size > 0){
        _structure->getIntersectionPoint(_firstPointKey, x, y);
        p = new ExtremityPoint(QPointF(x, y), this);
        this->addItem(p);
        _pointList.append(p);
        p->setInternalKey(_firstPointKey);

        lastPlacedKey = _firstPointKey;
    }

    //pour éviter une boucle infinie avec un while,
    //on fait un for sur une valeur constante
    for(int j = 0; j < size; j++){
        foreach(int i, l){
            _structure->getSegment(i, p1Key, p2Key, controlKey);

            if(p1Key == lastPlacedKey){
                _structure->getIntersectionPoint(p2Key, x, y);
                if(p2Key == _lastPointKey){
                    p = new ExtremityPoint(QPointF(x, y), this);
                }else{
                    p = new BoundingPoint(x, y, this);
                }
                this->addItem(p);
                _pointList.append(p);
                p->setInternalKey(p2Key);

                BoundingPoint* p1;
                p1 = _pointList.at(_pointList.size() - 2);
                BrLine* newLine = this->createLine(p1, p);
                newLine->setInternalKey(i);

                if(controlKey != Data::MONOFIN_SURFACE_NO_CONTROL_POINT){
                    _structure->getControlPoint(controlKey, x, y);
                    ControlPoint* cp = _controlPointsList.at(newLine->lineId());
                    cp->setInternalKey(controlKey);
                    newLine->setControlPoint(true);
                    newLine->moveControlPoint(QPointF(x, y));
                }

                l.removeOne(i);
                lastPlacedKey = p2Key;
                break;

            }else if(p2Key == lastPlacedKey){
                _structure->getIntersectionPoint(p1Key, x, y);
                if(p1Key == _lastPointKey){
                    p = new ExtremityPoint(QPointF(x, y), this);
                }else{
                    p = new BoundingPoint(x, y, this);
                }
                this->addItem(p);
                _pointList.append(p);
                p->setInternalKey(p1Key);

                BoundingPoint* p1;
                p1 = _pointList.at(_pointList.size() - 2);
                BrLine* newLine = this->createLine(p1, p);
                newLine->setInternalKey(i);

                if(controlKey != Data::MONOFIN_SURFACE_NO_CONTROL_POINT){
                    _structure->getControlPoint(controlKey, x, y);
                    ControlPoint* cp = _controlPointsList.at(newLine->lineId());
                    cp->setInternalKey(controlKey);
                    newLine->setControlPoint(true);
                    newLine->moveControlPoint(QPointF(x, y));
                }

                l.removeOne(i);
                lastPlacedKey = p1Key;
                break;
            }
        }//foreach
    }//while

    if(_pointList.size() > 0){
        emit this->pointsOnScene(true);
    }else{
        emit this->pointsOnScene(false);
    }

}

void PaintingScene::keyPressEvent(QKeyEvent* event){

//SUPPR
    if(event->key() == Qt::Key_Delete){
        qDebug("suppr");
        this->removeSelectedPoints();

//CTRL
    }else if(event->key() == Qt::Key_Control){
        _isMultiSelectionKeyActivated = true;

//P
    }else if(event->key() == Qt::Key_P){
        this->savePicture();

//ESC
    }else if(event->key() == Qt::Key_Escape){
        this->stopCreateLine();

//+
    }else if(event->key() == Qt::Key_Plus){
        qDebug("plus");
        this->ajustSceneRect(20,20);


//-
    }else if(event->key() == Qt::Key_Minus){
        qDebug("minus");
        this->ajustSceneRect(-20,-20);

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

//1. CREATE LINE
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

//2. CREATE SELECTION RECTANGLE
    else if(_isCreatingSelectionRect){
        _selectionRect->setBottomRight(event->scenePos());
    }


//3. HIGHLIGHT ITEMS
    else{
  //3.1 Highlight points

        if(!(_isAddControlActivated ||
             _isAddPointActivated ||
             _isRemoveControlPointActivated)){

            //QRectF r(event->scenePos().x(), event->scenePos().y(), 0.1, 0.1);
            foreach(bp, _pointList){
                bp->setMouseOnPoint(false);
                if(!_isOneSelection && bp->isUnderMouse()){
                    bp->setMouseOnPoint(true);
                    _isOneSelection = true;
                    _itemUnderMouse = bp;


                //Décommenter le bloc suivant rendra
                //les points plus durs à attraper...
                    /*
                    QLineF l(bp->coord(), event->scenePos());
                    if(l.dx() > 0){
                        bp->moveTo(QPointF(bp->coord().x()-2, bp->coord().y()));
                    }else{
                        bp->moveTo(QPointF(bp->coord().x()+2, bp->coord().y()));
                    }
                    if(l.dy() > 0){
                        bp->moveTo(QPointF(bp->coord().x(), bp->coord().y()-2));
                    }else{
                        bp->moveTo(QPointF(bp->coord().x(), bp->coord().y()+2));
                    }
                    */



                }//if
            }//foreach
        }//if

   //3.2 Highlight lines
        if(_isAddControlActivated || _isAddPointActivated){
            QRectF r(event->scenePos().x() - 3, event->scenePos().y() - 3, 6, 6);
            BrLine* lf;
            foreach(lf, _lineList){
                lf->setMouseOnLine(false);
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
        bool isFirstPoint = false;
        bool isLastPoint = false;
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
                        isLastPoint = true;
                    }else{
            //Middle points :
                        //on place le nouveau point aux coordonnées du ghost point
                        rect = new BoundingPoint(_ghostPoint->coord(), this);
                        //on définit le nouveau point de départ de la ghost line
                        _ghostLine->setP1(rect);
                    }
                }else{
            //First point :

                    _structure->startHistory(Data::MonofinSurface);

                    rect = new ExtremityPoint(_ghostPoint->coord(), this);
                    _ghostLine = new GhostLine(rect, _ghostPoint, this);
                    this->addItem(_ghostLine);
                    isFirstPoint = true;
                }
            //All points :
                this->addItem(rect);
                _pointList.append(rect);

                int key = BADKEY;
                key = _structure->addIntersectionPoint(rect->coord().x(), rect->coord().y());
                qDebug("point's key : %d", key);

                rect->setInternalKey(key);

                if(_pointList.size() > 1){

                    BoundingPoint* p1;
                    BoundingPoint* p2;
                    p1 = _pointList.at(_pointList.size() - 2);
                    p2 = _pointList.back();
                    BrLine* newLine = this->createLine(p1, p2);

                    int segmentKey = BADKEY;


                    /*int controlKey = BADKEY;
                    ControlPoint* cp = _controlPointsList.at(_lineList.indexOf(newLine));
                    controlKey = _structure->addControlPoint(cp->coord().x(),
                                                             cp->coord().y());
                    */


                    segmentKey = _structure->addSegment(p1->internalKey(),
                                                        p2->internalKey(),
                                                        Data::MONOFIN_SURFACE_NO_CONTROL_POINT);
                    qDebug("line's key : %d", segmentKey);

                    newLine->setInternalKey(segmentKey);

                }

            //First and last points :
                if(isFirstPoint){
                    isFirstPoint = false;
                    _hasPlacedFirstPoint = true;
                    PaintingScene::mouseMoveEvent(event);
                }else if(isLastPoint){

                    //à ce moment là, les points sont présents dans
                    //la liste DANS L'ORDRE
                    _firstPointKey = _pointList.at(0)->internalKey();
                    _lastPointKey = _pointList.back()->internalKey();

                    isLastPoint = false;
                    emit lineFinished(true);
                    _isCreateLineActivated = false;
                    qDebug("last point created");

                    _structure->stopHistory(Data::MonofinSurface);
                }

            }//end if (can be placed)
        }else

//2. ADD CONTROL POINT
        if(_isAddControlActivated){
            BrLine* l;
            foreach(l, _lineList){
                if(l->isMouseOnLine()){
                    if(!l->isControlPointActivated()){
                        l->setControlPoint(true);

                        _structure->startHistory(Data::MonofinSurface);
                        ControlPoint* cp = _controlPointsList.at(l->lineId());
                        int controlKey = _structure->
                                         addControlPoint(cp->coord().x(),
                                                         cp->coord().y());
                        cp->setInternalKey(controlKey);
                        _structure->addControlPointToSegment(l->internalKey(), controlKey);
                        _structure->stopHistory(Data::MonofinSurface);

                    }
                }
            }
        }else

//3. INSERT POINT
        if(_isAddPointActivated){
            BrLine* lf;
            foreach(lf, _lineList){
                if(lf->isMouseOnLine()){

                    _structure->startHistory(Data::MonofinSurface);

                    int i = lf->lineId();
                    int oldLineKey = lf->internalKey();

        // ! duplication de code, à revoir pour optimisation

                    if(!lf->isControlPointActivated()){
                    //si ce n'est pas une courbe de bézier
                        _pointList.at(i)->removeRightLine();

                        //on vérifie que le point est inséré dans la
                        //zone de déplacement des points
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

                        //on crée un nouveau point
                        BoundingPoint* bp = new BoundingPoint(pos, this);
                        int pointKey = _structure->addIntersectionPoint(pos.x(), pos.y());
                        bp->setInternalKey(pointKey);
                        this->addItem(bp);
                        _pointList.insert(i+1, bp);

                        //on crée deux nouvelles lignes

                        int newLineKey = _structure->subdivideSegment(oldLineKey, pointKey);

                        BrLine* l1 = this->createLine(_pointList.at(i), bp);
                        //celle-ci a la clef interne de l'ancienne ligne
                        l1->setInternalKey(oldLineKey);

                        BrLine* l2 = this->createLine(bp, _pointList.at(i+2));
                        l2->setInternalKey(newLineKey);

                    }else{
                    //si c'est une courbe de bézier
                        QLineF tangent1 = lf->tangent1();
                        QLineF tangent2 = lf->tangent2();

                        qreal t = 0;
                        qreal dist = this->height() * this->width(); //infini
                        QPointF mousePos(event->scenePos());
                     //on cherche le point de la courbe le plus proche
                     //du curseur de la souris
                        for(qreal j = 0; j <= 1; j+= 0.01){
                            qreal newDist = QLineF(lf->pointAt(j), mousePos).length();
                            if(newDist < dist){
                                dist = newDist;
                                t = j;
                            }
                        }

                        QPointF pos = lf->pointAt(t);

                        _pointList.at(i)->removeRightLine();

                        //on crée un nouveau point
                        BoundingPoint* bp = new BoundingPoint(pos, this);
                        int pointKey = _structure->addIntersectionPoint(pos.x(), pos.y());
                        bp->setInternalKey(pointKey);
                        this->addItem(bp);
                        _pointList.insert(i+1, bp);

                        //on crée deux nouvelles lignes

                        int newLineKey = _structure->subdivideSegment(oldLineKey, pointKey);

                        BrLine* l1 = this->createLine(_pointList.at(i), bp);
                        //celle-ci a la clef interne de l'ancienne ligne
                        l1->setInternalKey(oldLineKey);

                        BrLine* l2 = this->createLine(bp, _pointList.at(i+2));
                        l2->setInternalKey(newLineKey);

                        //on place les points de contrôle aux emplacements calculés
                        QPointF contr1(tangent1.pointAt(t));
                        l1->setControlPoint(true);
                        l1->moveControlPoint(contr1);
                        int controlKey1 = _structure->addControlPoint(contr1.x(), contr1.y());
                        _structure->addControlPointToSegment(l1->internalKey(), controlKey1);
                        ControlPoint* cp1 = _controlPointsList.at(i);
                        cp1->setInternalKey(controlKey1);

                        QPointF contr2(tangent2.pointAt(1-t));
                        l2->setControlPoint(true);
                        l2->moveControlPoint(tangent2.pointAt(1-t));
                        int controlKey2 = _structure->addControlPoint(contr2.x(), contr2.y());
                        _structure->addControlPointToSegment(l2->internalKey(), controlKey2);
                        ControlPoint* cp2 = _controlPointsList.at(i+1);
                        cp2->setInternalKey(controlKey2);
                    }
                    this->updateLinesId();

                    _structure->stopHistory(Data::MonofinSurface);

                    break;
                }
            }//foreach

//4. REMOVE CONTROL POINT
        }else if(_isRemoveControlPointActivated){}

//5. OTHER
//5.1 Something is highlighted
        else if(_isOneSelection){
            if(!_isMultiSelectionKeyActivated){
                this->clearSelection();
                _itemUnderMouse->setSelected(true);
            }else{
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
    }else{
        QGraphicsScene::mouseReleaseEvent(event);
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






