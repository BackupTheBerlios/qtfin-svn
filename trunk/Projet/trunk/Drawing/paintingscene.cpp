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
    _hasABoundingPointMoved(false), _hasPlacedFirstPoint(false),
    _isAddControlActivated(false), _isAddPointActivated(false),
    _isAPointHighlighted(false), _isCreateLineActivated(false),
    _isCreatingSelectionRect(false), _isMultiSelectionKeyActivated(false),
    _isRemoveControlPointActivated(false),
    _isSimplifyViewActivated(false), _keepBezierCurve(true),
    _lastPlacedPoint(0), _scaleFactor(1), _structure(structure){

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

void PaintingScene::addControlPoint(ControlPoint* p){
    _controlPointsList.append(p);
}

void PaintingScene::boundingPointHasMoved(BoundingPoint* p, bool isUniqueModification){
    if(isUniqueModification){
        _structure->startHistory(Data::MonofinSurface);
    }

    _structure->setIntersectionPoint(p->internalKey(), p->coord().x(), p->coord().y());
    _hasABoundingPointMoved = true;


    if(isUniqueModification){
        _structure->stopHistory(Data::MonofinSurface);
        emit this->somethingChanged(this->ActionMoveBoundingPoint);
    }
}

void PaintingScene::boundingPointIsHighlighted(BoundingPoint* p, bool isHighlighted){
    _isAPointHighlighted = isHighlighted;
    _itemUnderMouse = p;
}

void PaintingScene::controlPointHasMoved(ControlPoint* p, bool isUniqueModification){
    if(isUniqueModification){
        _structure->startHistory(Data::MonofinSurface);
    }

    _structure->setControlPoint(p->internalKey(), p->coord().x(), p->coord().y());

    if(isUniqueModification){
        _structure->stopHistory(Data::MonofinSurface);
        emit this->somethingChanged(this->ActionMoveControlPoint);
    }
}

void PaintingScene::controlPointHasBeenHidden(ControlPoint* p, bool isUniqueModification){
    if(isUniqueModification){
        _structure->startHistory(Data::MonofinSurface);
    }

    _structure->removeControlPoint(p->internalKey());

    if(isUniqueModification){
        _structure->stopHistory(Data::MonofinSurface);
        emit this->somethingChanged(this->ActionDeleteControlPoint);
    }
}

QList<ControlPoint*> PaintingScene::controlPointsSurroundedBySelectedPoints(){
    QList<ControlPoint*> l;
    foreach(BrLine* line, _lineList){
        if(line->isControlPointActivated()){
            if(line->boundingPointP1()->isSelected() &&
               line->boundingPointP2()->isSelected()){
                l.append(line->controlPoint());
            }
        }
    }//foreach
    return l;
}

BrLine* PaintingScene::createLine(BoundingPoint* p1, BoundingPoint* p2){
    BrLine* l = new BrLine(p1,p2,this);

    if(!p1->hasRightLine()){
        p1->setRightLine(l);
    }else if(!p1->hasLeftLine()){
        p1->setLeftLine(l);
    }else{
        qDebug("Warning : attempting to add a line to a point already linked with two lines.");
    }

    if(!p2->hasLeftLine()){
        p2->setLeftLine(l);
    }else if(!p2->hasRightLine()){
        p2->setRightLine(l);
    }else{
        qDebug("Warning : attempting to add a line to a point already linked with two lines.");
    }

    _lineList.append(l);
    this->addItem(l);
    this->update(this->sceneRect());
    return l;
}

bool PaintingScene::existIntersectionsBetweenLines(){
    bool intersect = false;
    for(int i =0; i < _lineList.size()-1; i++){
        for(int j = i+1; j < _lineList.size(); j++){
            intersect = intersect || _lineList.at(i)->intersects(*(_lineList.at(j)));
        }
    }
    return intersect;

}

QRectF PaintingScene::pointsBoundingZone(){
    return this->sceneRect()/*.adjusted(20,20,-20,-20)*/;
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
}

void PaintingScene::removePoint(BoundingPoint* p){    
    //on sauvegarde temporairement les coordonnées au cas où
    QPointF oldCoord = p->coord();

    //on récupère les références des points autour de p, tout en faisant les
    //tests nécessaires (pointeurs non nuls) pour éviter les bugs
    BrLine* left = p->leftLine();
    BrLine* right = p->rightLine();

    //on récupère au passage les clés internes des deux anciennes lignes
    int leftKey = left->internalKey();
    int rightKey = right->internalKey();


    BoundingPoint* p1 = 0;
    BoundingPoint* p2 = 0;

    if(left != 0){
        p1 = left->boundingPointP1();
        if(p1 == p){
            p1 = left->boundingPointP2();
        }
    }
    if(right != 0){
        p2 = right->boundingPointP2();
        if(p2 == p){
            p2 = right->boundingPointP1();
        }
    }

    //Le point appelle des fonctions de destruction sur chacune de ses lignes
    p->removeLeftLine();
    p->removeRightLine();

    //on enlève le point de la liste de points, ainsi que de la scène
    _pointList.removeOne(p);
    this->removeItem(p);

    //on l'enlève de la structure interne et on récupère la clé supprimée
    int removedKey = _structure->removeIntersectionPoint(p->internalKey());
    if(removedKey == Data::MONOFIN_SURFACE_NO_SEGMENT_ERASED){
        qDebug("Big problem while suppressing the point from the structure.");
        return;
    }

    //si le point n'est pas aux extémités, on relie les deux points autour
    //de celui supprimé, par une ligne
    if(p->type() != ExtremityPoint::Type){
        BrLine* newLine;
        if(p1 !=0 && p2 !=0){

///////////////////////////

//PREVOIR UN TEST AVEC CREATELINE(p2,p1)

///////////////////////////

            newLine = this->createLine(p2, p1);
        }else{
            qDebug("Problem with pointers while removing a point.");
            return;
        }

        //Entre les deux clés des anciennes lignes, on vérifie laquelle a été
        //supprimée par la structure et laquelle est toujours valide. Celle
        //toujours valide sert de clé pour la nouvelle ligne.
        int newSegmentKey = Data::MONOFIN_SURFACE_NOT_CREATED_SEGMENT;
        if(leftKey == removedKey){
            newSegmentKey = rightKey;
        }else if(rightKey == removedKey){
            newSegmentKey = leftKey;
        }else{
            qDebug("Big problem while suppressing a point.");
        }

        newLine->setInternalKey(newSegmentKey);


        //newLine->setControlPoint(true);
        //newLine->moveControlPoint(oldCoord);
    }

    //Destruction du point et du GraphicsItem qu'il contient
    delete p;
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

    ControlPoint c(0,0,0,this);
    BoundingPoint b(0,0,this);
    qreal max = c.boundingRect().width();
    if (max < b.boundingRect().width()){
        max = b.boundingRect().width();
    }
    max/=2.0;

    if(newRect.adjusted(-max, -max, max, max).contains(itemRect)){
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
        emit this->somethingChanged(this->ActionAlignTangents);
}

void PaintingScene::cleanPoints(){
    this->removeAllPoints();
    _structure->startHistory(Data::MonofinSurface);
    _structure->clearSurface();
    _structure->stopHistory(Data::MonofinSurface);
    emit this->somethingChanged(this->ActionRemoveSomePoints);
    this->update(this->sceneRect());
    _hasPlacedFirstPoint = false;
}

void PaintingScene::keepBezierCurve(bool k){
    _keepBezierCurve = k;
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
    emit this->somethingChanged(this->ActionRemoveSomePoints);
}

void PaintingScene::savePicture(){
    unsigned int x = 800;
    unsigned int y = 600;

    QPixmap pixmap(x,y);
    pixmap.fill(Qt::white);

    QPainter painter;
    painter.begin(&pixmap);
    painter.translate(0,y);
    painter.scale(1,-1);
    bool simpl = _isSimplifyViewActivated;
    this->simplifyView(false);
    this->render(&painter, QRectF(), this->itemsBoundingRect().adjusted(-20,-20,20,20));
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
        emit this->somethingChanged(this->ActionNoAction);

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
    int size = l.size();

    if(size > 0){

        //on récupère la liste des deux clés des points extrémité
        QList<int> extremityPoints = _structure->getExtremityPoint();
        if(extremityPoints.size() != 2){
            qDebug("There is less or more than 2 extremity points in the structure.");
            return;
        }

        int extremity1 = extremityPoints.first();
        int extremity2 = extremityPoints.back();

        float x = 0; //valeurs arbitraires pour l'initialisation
        float y = 0;

        BoundingPoint* p;

        //on place le premier point extremité (pas forcément le plus à gauche)
        _structure->getIntersectionPoint(extremity1, x, y);
        p = new ExtremityPoint(QPointF(x,y),this);
        this->addItem(p);
        _pointList.append(p);
        p->setInternalKey(extremity1);

        //on démarre une boucle qui travaille sur tous les points en
        //initialisant le point courant au premier point extrémité, et le
        //segment courant au segment relié au premier point extrémité
        int currentPointKey = extremity1;
        QList<int> segmentKeyList = _structure->getSegmentKeysLinkedToPoint(extremity1);
        if(segmentKeyList.size() != 1){
            qDebug("An extremity point is linked to less or more than one segment.");
            return;
        }
        int currentSegmentKey = segmentKeyList.first();

        //on initialise des variables utiles à la boucle
        int p1Key = Data::MONOFIN_SURFACE_NOT_CREATED_POINT;
        int p2Key = Data::MONOFIN_SURFACE_NOT_CREATED_POINT;
        int controlKey = Data::MONOFIN_SURFACE_NO_CONTROL_POINT;
        BoundingPoint* lastPlacedPoint = p;

        bool allPointsPlaced = false;

        //la boucle en question, avec un compteur pour éviter de boucler à
        //l'infini en cas de problème, mais en principe, la réelle condition
        //d'arrêt est le booléen allPointsPlaced :
        //équivalent à " while(!allPointsPlaced){ "
        for(int i = 0; i < size && !allPointsPlaced; i++){

            //on récupère les informations sur le segment courant :
            //clés de ses deux points, et clé de son éventuel point de contrôle
            _structure->getSegment(currentSegmentKey, p1Key, p2Key, controlKey);

            //entre les deux points du segment, on cherche sur lequel on n'a
            //pas encore travaillé, et on passe ce dernier en point courant
            if(p1Key == currentPointKey){
                currentPointKey = p2Key;
            }else if(p2Key == currentPointKey){
                currentPointKey = p1Key;
            }else{
                qDebug("Problem with point key while getting the monofin structure.");
                return;
            }

            //on récupère les coordonnées du point courant
            _structure->getIntersectionPoint(currentPointKey, x, y);

            //si le point courant est le deuxième point extrémité de la palme,
            //on le construit en tant que tel, et on fixe un booléen indiquant
            //que la boucle doit se terminer
            if(currentPointKey == extremity2){
                p = new ExtremityPoint(QPointF(x, y), this);
                allPointsPlaced = true;
            }else{
                p = new BoundingPoint(x, y, this);
            }

            //on insère le point dans la scène
            this->addItem(p);
            _pointList.append(p);

            //on fixe sa clé interne
            p->setInternalKey(currentPointKey);

            //on relie le point courant et le dernier point placé par une ligne
            //et on fixe la clé interne de la ligne
            BrLine* newLine = this->createLine(lastPlacedPoint, p);
            newLine->setInternalKey(currentSegmentKey);

            //on actualise le dernier point placé au point courant
            lastPlacedPoint = p;

            //si dans la structure, le segment contient un point de contrôle,
            //on l'active dans la nouvelle ligne, on fixe sa clé interne et on
            //le place aux coordonnées récupérées
            if(controlKey != Data::MONOFIN_SURFACE_NO_CONTROL_POINT){
                _structure->getControlPoint(controlKey, x, y);
                ControlPoint* cp = newLine->controlPoint();
                cp->setInternalKey(controlKey);
                newLine->setControlPoint(true);
                newLine->moveControlPoint(QPointF(x, y));
            }

            //Si l'on est pas à la fin de la boucle, on actualise le segment
            //courant, en prenant celui relié au point courant qui n'est pas
            //l'ancien segment courant
            if(!allPointsPlaced){
                segmentKeyList = _structure->getSegmentKeysLinkedToPoint(currentPointKey);
                if(segmentKeyList.size() != 2){
                qDebug("An intersection point is linked to less or more than two segments.");
                return;
                }
                int segmentKey1 = segmentKeyList.first();
                int segmentKey2 = segmentKeyList.back();
                if(segmentKey1 == currentSegmentKey){
                    currentSegmentKey = segmentKey2;
                }else if(segmentKey2 == currentSegmentKey){
                    currentSegmentKey = segmentKey1;
                }else{
                    qDebug("Problem with the segment keys while getting the monofin structure");
                    return;
                }

            }//end if !allPointsPlaced


        }//while

        //par sécurité, on ne rend les points sélectionnables
        //(et donc déplaçables) qu'à la fin
        foreach(BoundingPoint* bp, _pointList){
            bp->setFlag(QGraphicsItem::ItemIsSelectable, true);
        }

        //on émet un signal indiquant que la scène contient des points
        emit this->pointsOnScene(true);


        //on vérifie que le rectangle de la scène est bien autour de
        //tous les objets de cette scène, sinon on l'ajuste :

        QRectF itemRect = this->itemsBoundingRect();
        QRectF sceneRect = this->sceneRect();
        //un point d'intersection ou un point de contrôle peut avoir un
        //rectangle englobant dépassant du rectangle de la scène, donc on le
        //prend en compte en agrandissant un peu le rectangle de la scène
        //avant de vérifier qu'il entoure bien tous les objets
        ControlPoint c(0,0,0,this); //TRES DANGEREUX / VERY DANGEROUS
        BoundingPoint b(0,0,this);
        qreal max = c.boundingRect().width();
        if (max < b.boundingRect().width()){
            max = b.boundingRect().width();
        }
        max/=2.0;

        if(!sceneRect.adjusted(-max, -max, max, max).contains(itemRect)){
            sceneRect = sceneRect.united(itemRect);
            sceneRect.moveLeft(0);
            QRect newSceneRect = sceneRect.toRect();
            this->setSceneRect(newSceneRect);
        }
        //on réajuste l'axe de symétrie au cas où la taille de la scène
        //aurait changé et on rafraichit la vue
        _axis->setLine(0,0,this->width(), 0);
        this->update(this->sceneRect());



    }//end if size > 0

    else{
        //s'il n'y a aucun point à placer, on émet un signal indiquant que
        //la scène est vide
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
        this->ajustSceneRect(20,20);


//-
    }else if(event->key() == Qt::Key_Minus){
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
    /*if(this->existIntersectionsBetweenLines()){
        qDebug("intersect !!");
    }else{
        qDebug("intersect not...");
    }*/

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
                        break;
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

        /*if(!(_isAddControlActivated ||
             _isAddPointActivated ||
             _isRemoveControlPointActivated)){

            //QRectF r(event->scenePos().x(), event->scenePos().y(), 0.1, 0.1);
            foreach(bp, _pointList){
                bp->setMouseOnPoint(false);
                if(!__isAPointHighlighted && bp->isUnderMouse()){
                    bp->setMouseOnPoint(true);
                    __isAPointHighlighted = true;
                    _itemUnderMouse = bp;





                }//if
            }//foreach
        }//if
*/
   //3.2 Highlight lines
        bool isOneSelection = false;
        if(_isAddControlActivated || _isAddPointActivated){
            isOneSelection = false;
            QRectF r(event->scenePos().x() - 3, event->scenePos().y() - 3, 6, 6);
            BrLine* lf;
            foreach(lf, _lineList){
                lf->setMouseOnLine(false);
                if(!isOneSelection){
                    if(lf->intersects(r)){
                        lf->setMouseOnLine(true);
                        isOneSelection = true;
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

                int key = Data::MONOFIN_SURFACE_NOT_CREATED_POINT;
                key = _structure->addIntersectionPoint(rect->coord().x(), rect->coord().y());
                qDebug("point's key : %d", key);

                rect->setInternalKey(key);

                if(_pointList.size() > 1 && _lastPlacedPoint != 0){

                    BrLine* newLine;
                    newLine = this->createLine(_lastPlacedPoint, rect);


                    int segmentKey = Data::MONOFIN_SURFACE_NOT_CREATED_SEGMENT;


                    /*int controlKey = Data::MONOFIN_SURFACE_NO_CONTROL_POINT;
                    ControlPoint* cp = _controlPointsList.at(_lineList.indexOf(newLine));
                    controlKey = _structure->addControlPoint(cp->coord().x(),
                                                             cp->coord().y());
                    */


                    segmentKey = _structure->addSegment(_lastPlacedPoint->internalKey(),
                                                        rect->internalKey(),
                                                        Data::MONOFIN_SURFACE_NO_CONTROL_POINT);
                    qDebug("line's key : %d", segmentKey);

                    newLine->setInternalKey(segmentKey);

                }

                _lastPlacedPoint = rect;

            //First and last points :
                if(isFirstPoint){
                    isFirstPoint = false;
                    _hasPlacedFirstPoint = true;
                    PaintingScene::mouseMoveEvent(event);
                }else if(isLastPoint){

                    foreach(BoundingPoint* bp, _pointList){
                        bp->setFlag(QGraphicsItem::ItemIsSelectable, true);
                    }

                    isLastPoint = false;
                    emit lineFinished(true);
                    _isCreateLineActivated = false;
                    qDebug("last point created");

                    _structure->stopHistory(Data::MonofinSurface);
                    emit this->somethingChanged(this->ActionCreateLine);
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
                        ControlPoint* cp = l->controlPoint();
                        int controlKey = _structure->
                                         addControlPoint(cp->coord().x(),
                                                         cp->coord().y());
                        cp->setInternalKey(controlKey);
                        _structure->addControlPointToSegment(l->internalKey(), controlKey);
                        _structure->stopHistory(Data::MonofinSurface);
                        emit this->somethingChanged(this->ActionInsertControlPoint);

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

                    int oldLineKey = lf->internalKey();
                    BoundingPoint* p1 = lf->boundingPointP1();
                    BoundingPoint* p2 = lf->boundingPointP2();

        // ! duplication de code, à revoir pour optimisation

                    if(!lf->isControlPointActivated() || !_keepBezierCurve){
                    //si ce n'est pas une courbe de bézier
                        if(p1->rightLine() == lf){
                            p1->removeRightLine();
                        }else if(p1->leftLine() == lf){
                            p1->removeLeftLine();
                        }else{
                            qDebug("Big problem while inserting a point");
                        }

                        //_pointList.at(i)->removeRightLine();

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
                        _pointList.append(bp);

                        //on crée deux nouvelles lignes

                        int newLineKey = _structure->subdivideSegment(oldLineKey, pointKey);

                        qDebug("new line's key : %d ; old line's key : %d", newLineKey, oldLineKey);


                        BrLine* l1 = this->createLine(p1, bp);
                        //celle-ci a la clef interne de l'ancienne ligne
                        l1->setInternalKey(oldLineKey);

                        BrLine* l2 = this->createLine(bp, p2);
                        l2->setInternalKey(newLineKey);

                        bp->setFlag(QGraphicsItem::ItemIsSelectable, true);

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

                        if(p1->rightLine() == lf){
                            p1->removeRightLine();
                        }else if(p1->leftLine() == lf){
                            p1->removeLeftLine();
                        }else{
                            qDebug("Big problem while inserting a point");
                        }

                        //on crée un nouveau point
                        BoundingPoint* bp = new BoundingPoint(pos, this);
                        int pointKey = _structure->addIntersectionPoint(pos.x(), pos.y());
                        bp->setInternalKey(pointKey);
                        this->addItem(bp);
                        _pointList.append(bp);

                        //on crée deux nouvelles lignes

                        int newLineKey = _structure->subdivideSegment(oldLineKey, pointKey);

                        BrLine* l1 = this->createLine(p1, bp);
                        //celle-ci a la clef interne de l'ancienne ligne
                        l1->setInternalKey(oldLineKey);

                        BrLine* l2 = this->createLine(bp, p2);
                        l2->setInternalKey(newLineKey);

                        //on place les points de contrôle aux emplacements calculés
                        QPointF contr1(tangent1.pointAt(t));
                        l1->setControlPoint(true);
                        l1->moveControlPoint(contr1);
                        int controlKey1 = _structure->addControlPoint(contr1.x(), contr1.y());
                        _structure->addControlPointToSegment(l1->internalKey(), controlKey1);
                        //ControlPoint* cp1 = _controlPointsList.at(i);
                        l1->controlPoint()->setInternalKey(controlKey1);

                        QPointF contr2(tangent2.pointAt(1-t));
                        l2->setControlPoint(true);
                        l2->moveControlPoint(tangent2.pointAt(1-t));
                        int controlKey2 = _structure->addControlPoint(contr2.x(), contr2.y());
                        _structure->addControlPointToSegment(l2->internalKey(), controlKey2);
                        //ControlPoint* cp2 = _controlPointsList.at(i+1);
                        l2->controlPoint()->setInternalKey(controlKey2);

                        bp->setFlag(QGraphicsItem::ItemIsSelectable, true);

                    }
                    //this->updateLinesId();

                    _structure->stopHistory(Data::MonofinSurface);
                    emit this->somethingChanged(this->ActionInsertBoundingPoint);

                    break;
                }
            }//foreach


//4. REMOVE CONTROL POINT
        }else if(_isRemoveControlPointActivated){}

//5. OTHER
//5.1 Something is highlighted
        else if(_isAPointHighlighted){
            if(!_isMultiSelectionKeyActivated){
                if(!this->selectedItems().contains(_itemUnderMouse)){
                    this->clearSelection();
                    _itemUnderMouse->setSelected(true);
                }
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

        if(_hasABoundingPointMoved){
            _hasABoundingPointMoved = false;
        }else{

            if(_isAPointHighlighted){
                if(!_isMultiSelectionKeyActivated){
                    this->clearSelection();
                    _itemUnderMouse->setSelected(true);
                }
            }
        }


    }
}






