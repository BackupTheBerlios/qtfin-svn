#include "paintingscene.h"
#include "symmetryaxis.h"
#include "ghostpoint.h"
#include "ghostline.h"
#include "brline.h"
#include "selectionrect.h"

//using namespace Data;

//PUBLIC

PaintingScene::PaintingScene(qreal width,
                             qreal height,
                             Data::ProjectFile* structure,
                             QObject* parent)
    :QGraphicsScene(parent), _canCreateSelectionRect(true),
    _gridUnit(20), _hasABackgroundPicture(false),
    _hasABoundingPointMoved(false), _hasPlacedFirstPoint(false),
    _isAddControlActivated(false), _isAddPointActivated(false),
    _isAPointHighlighted(false), _isCreateLineActivated(false),
    _isCreatingSelectionRect(false), _isMagnetActivated(false),
    _isModifyingBackgroundPicture(false), _isMultiSelectionKeyActivated(false),
    _isRemoveControlPointActivated(false), _isRenderingPicture(false),
    _isSimplifyViewActivated(false), _keepBezierCurve(true),
    _lastPlacedPoint(0), _pixmap(0), _scaleFactor(1), _showGrid(true),
    _state(NormalState), _structure(structure){

     this->setSceneRect(0,-height/2,width, height);

     //Indispensable pour une scène dont les items bougent souvent
     this->setItemIndexMethod(QGraphicsScene::NoIndex);

    _axis = new SymmetryAxis(this);
    this->addItem(_axis);

    _colors.resize(6);

    _colors.insert(BoundingPointNormalColor, Qt::black);
    _colors.insert(BoundingPointHighlightingColor, Qt::blue);
    _colors.insert(BoundingPointSelectionColor, Qt::red);

    _colors.insert(ControlPointNormalColor, Qt::black);
    _colors.insert(ControlPointHighlightingColor, Qt::green);

    _colors.insert(LineNormalColor, Qt::black);
    _colors.insert(LineHighlightingColor, Qt::blue);

    //to avoid null pointers
    _pixItem = new PixmapItem(QPixmap());
    _rotCircle = new RotateCircle(_pixItem);

    _ghostPoint = new GhostPoint(QPointF(0,0), this);
    _ghostLine = new GhostLine(0, _ghostPoint, this); //WARNING : initialized
                                                      //with a null pointer

}

PaintingScene::~PaintingScene(){
    if(_state == this->CreatePointsState){
        _state = NormalState;
        _isCreateLineActivated = false;
        //this->removeItem(_ghostPoint);
        //delete _ghostPoint;
//        if(_hasPlacedFirstPoint){
//            this->removeItem(_ghostLine);
//            delete _ghostLine;
//        }
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

QColor PaintingScene::getColor(ColorItem item, ColorType type){
    switch(item){
        case BoundingPointColor:
            switch(type){
                case NormalColor:
                    return _colors.at(BoundingPointNormalColor);
                    break;

                case HighlightingColor:
                    return _colors.at(BoundingPointHighlightingColor);
                    break;

                case SelectionColor:
                    return _colors.at(BoundingPointSelectionColor);
                    break;

                default:
                    qDebug("Bad type of color while requesting a color");
                    return QColor();

            }//switch
        break;

        case ControlPointColor:
            switch(type){
                case NormalColor:
                    return _colors.at(ControlPointNormalColor);
                    break;

                case HighlightingColor:
                    return _colors.at(ControlPointHighlightingColor);
                    break;

                default:
                    qDebug("Bad type of color while requesting a color");
                    return QColor();

            }//switch
        break;

        case LineColor:
            switch(type){
                case NormalColor:
                    return _colors.at(LineNormalColor);
                    break;

                case HighlightingColor:
                    return _colors.at(LineHighlightingColor);
                    break;

                default:
                    qDebug("Bad type of color while requesting a color");
                    return QColor();

            }//switch
        break;

        default:
        qDebug("Bad item while requesting color");
        return QColor();

    }

}

QPixmap PaintingScene::getPictureOfTheScene(unsigned int x, unsigned int y){

    if(x <= 0 || y <= 0){return QPixmap();}

    if(_state == ModifyBackgroundPictureState){
        this->removeItem(_pixItem);
    }

    QPixmap pixmap(x,y);
    pixmap.fill(Qt::white);

    QPainter painter;
    painter.begin(&pixmap);
    painter.translate(0,y);
    painter.scale(1,-1);
    bool simpl = _isSimplifyViewActivated;
    this->simplifyView(true);
    _isRenderingPicture = true;
    this->render(&painter, QRectF(), this->itemsBoundingRect().adjusted(-20,-20,20,20));
    _isRenderingPicture = false;
    painter.end();
    this->simplifyView(simpl);

    if(_state == ModifyBackgroundPictureState){
        this->addItem(_pixItem);
    }

    return pixmap;
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
}


//PUBLIC SLOTS

void PaintingScene::activateCreateLine(bool a){
    if(a){
        if(_state != NormalState){
            qDebug("Warning : 'Create line' activated while another function is already activated");
        }
        _state = CreatePointsState;

        //_ghostPoint = new GhostPoint(QPointF(this->pointsBoundingZone().bottomLeft().x(),0), this);
        _ghostPoint->moveTo(QPointF(this->pointsBoundingZone().width()/4.0, 0));
        this->addItem(_ghostPoint);
        _hasPlacedFirstPoint = false;
        this->update();
    }else{
        if(_state != CreatePointsState){
            qDebug("Warning : attempting to deactivate 'create line', but this function is not active : go back to normal mode");
            _state = NormalState;
        }else{
            this->stopCreateLine();
        }
    }
    _isCreateLineActivated = a;
}

void PaintingScene::activateAddControl(bool a){
    if(a){
        if(_state != NormalState){
            qDebug("Warning : 'Add control points' activated while another function is already activated");
        }
        _state = AddControlPointsState;

    }else{
        if(_state != AddControlPointsState){
            qDebug("Warning : attempting to deactivate 'add control points', but this function is not active : go back to normal mode");
        }
        _state = NormalState;
    }
    _isAddControlActivated = a;
}

void PaintingScene::activateAddPoint(bool a){
    if(a){
        if(_state != NormalState){
            qDebug("Warning : 'Insert points' activated while another function is already activated");
        }
        _state = InsertPointsState;

    }else{
        if(_state != InsertPointsState){
            qDebug("Warning : attempting to deactivate 'insert points', but this function is not active : go back to normal mode");
        }
        _state = NormalState;
    }
    _isAddPointActivated = a;
}

void PaintingScene::activateRemoveControlPoint(bool a){
    if(a){
        if(_state != NormalState){
            qDebug("Warning : 'Remove control points' activated while another function is already activated");
        }
        _state = RemoveControlPointsState;

    }else{
        if(_state != RemoveControlPointsState){
            qDebug("Warning : attempting to deactivate 'remove control points', but this function is not active : go back to normal mode");
        }
        _state = NormalState;
    }
    _isRemoveControlPointActivated = a;
}

void PaintingScene::adjustSceneRect(qreal dx, qreal dy){
    qreal MINSIZE = 1;
    qreal MAXSIZE = 10000;


    QRectF newRect = this->sceneRect().adjusted(0, -dy, dx, dy);
    QRectF itemRect = this->itemsBoundingRect();

    ControlPoint c(0,0,0,this);
    BoundingPoint b(0,0,this);
    qreal max = c.boundingRect().width();
    if (max < b.boundingRect().width()){
        max = b.boundingRect().width();
    }
    max/=2.0;

    //if the new rectangle contains the item rectangle or if they no
    //items on the scene, we verify the scene rectangle size, and if it is
    //correct (between MAXSIZE and MINSIZE), we adjust the scene rectangle
    if((newRect.adjusted(-max, -max, max, max).contains(itemRect) ||
        _pointList.size() == 0)
        && newRect.height() > MINSIZE
                && newRect.width() > MINSIZE
                && newRect.height() < MAXSIZE
                && newRect.width() < MAXSIZE){

        this->setSceneRect(newRect);
    }
    _axis->setLine(0,0,this->width(), 0);
    this->update();

}

void PaintingScene::alignTangents(){
    //we get the list of the selected items
    QList<QGraphicsItem*> l = this->selectedItems();

    //we begin the modifications on the internal structure
    _structure->startHistory(Data::MonofinSurface);

    //we call the function alignTangent on each item selected, depebding on its type
    foreach(QGraphicsItem* item, l){
        if(item->type() == BoundingPoint::Type){
            qgraphicsitem_cast<BoundingPoint*>(item)->alignTangents();
        }else if(item->type() == ExtremityPoint::Type){
            qgraphicsitem_cast<ExtremityPoint*>(item)->alignTangents();
        }
    }//foreach

    //we end the modifications on the internal structure
    _structure->stopHistory(Data::MonofinSurface);

    //we set the attribute hasABoundingPointMoved to false, to allow the
    //user to select correctly a point when others are selected
    //(see mouseReleaseEvent)
    this->_hasABoundingPointMoved = false;

    //we emit a signal because an action has been done
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

void PaintingScene::changeColor(int item, int type, const QColor& color){
    switch(item){

        case BoundingPointColor:
            foreach(BoundingPoint* bp, _pointList){
                switch(type){
                    case NormalColor:
                        bp->setColorWhenNormal(color);
                        _colors[BoundingPointNormalColor] = color;
                        break;
                    case SelectionColor:
                        bp->setColorWhenSelected(color);
                        _colors[BoundingPointSelectionColor] = color;
                        break;
                    case HighlightingColor:
                        bp->setColorWhenHighlighted(color);
                        _colors[BoundingPointHighlightingColor] = color;
                        break;
                    default:
                        qDebug("Cannot change color of bounding points : bad type");
                        return;
                }//switch
            }//foreach
            break;


        case ControlPointColor:
            foreach(ControlPoint* cp, _controlPointsList){
                switch(type){
                    case NormalColor:
                        cp->setColorWhenNormal(color);
                        _colors[ControlPointNormalColor] = color;
                        break;
                    case HighlightingColor:
                        cp->setColorWhenHighlighted(color);
                        _colors[ControlPointHighlightingColor] = color;
                        break;
                    default:
                        qDebug("Cannot change color of control points : bad type");
                        return;
                }//switch
            }//foreach
        break;

        case LineColor:
            foreach(BrLine* l, _lineList){
                switch(type){
                    case NormalColor:
                        l->setColorWhenNormal(color);
                        _colors[LineNormalColor] = color;
                        break;
                    case HighlightingColor:
                        l->setColorWhenHighlighted(color);
                        _colors[LineHighlightingColor] = color;
                        break;
                    default:
                        qDebug("Cannot change color of lines : bad type");
                        return;
                }//switch
            }//foreach
        break;

        default:
            qDebug("Cannot change color : bad argument for the item");
            return;
    }//switch

    this->update(this->sceneRect());

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

void PaintingScene::keepBezierCurve(bool k){
    _keepBezierCurve = k;
}

void PaintingScene::modifyBackgroundPicture(bool on){
    if(_hasABackgroundPicture){

        if(on){
            if(_state != NormalState){
                qDebug("Warning : 'Modify background picture' activated while another function is already activated");
            }
            _state = ModifyBackgroundPictureState;
            _isModifyingBackgroundPicture = true;
            this->addItem(_pixItem);
        }else if(!on){
            if(_state != ModifyBackgroundPictureState){
                qDebug("Warning : attempting to deactivate 'modify background picture', but this function is not active : go back to normal mode");
            }
            _state = NormalState;
            this->removeItem(_pixItem);
            _isModifyingBackgroundPicture = false;
        }

        this->update();
    }else{
        if(on){
            _state = ModifyBackgroundPictureState;
        }else{
            _state = NormalState;
        }

        qDebug("no background picture to modify, but changing state");
    }
}

void PaintingScene::redo(){
    _structure->redo(Data::MonofinSurface);

    this->getMonofinFromStructure();
    emit this->somethingChanged(ActionRedo);
}

void PaintingScene::removeBackgroundPicture(){
    if(_hasABackgroundPicture){
        _hasABackgroundPicture = false;
        if(_isModifyingBackgroundPicture){
            this->removeItem(_pixItem);
        }
        delete _pixItem;
        _pixItem = 0;

        this->update();

    }else{
        qDebug("No background picture to remove");
    }
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

void PaintingScene::setBackGroundPicture(QPixmap pixmap){

    if(pixmap.isNull()){return;}

    if(_hasABackgroundPicture){
        this->removeBackgroundPicture();
    }

    _pixItem = new PixmapItem(pixmap);
    _pixItem->setZValue(-2);

    _rotCircle = new RotateCircle(_pixItem);
    _rotCircle->setZValue(-1);

    _pixItem->setOffset(0, 0);
    _pixItem->translate2((this->width() - _pixItem->boundingRect().width())/ 2 , - _pixItem->boundingRect().height() / 2);
    _pixItem->rotate2(180);
    _rotCircle->setPosition(180);

    _pixmap = new QPixmap(_pixItem->pixmap());

    if(_state == ModifyBackgroundPictureState){
        this->addItem(_pixItem);
    }

    _hasABackgroundPicture = true;

    this->adjustSceneRectangleToItems();

    //this->update(this->sceneRect());

}

void PaintingScene::simplifyView(bool a){
    _isSimplifyViewActivated = a;
    this->update(this->sceneRect());
}

void PaintingScene::stopCreateLine(){
    if(_state == CreatePointsState){
        _isCreateLineActivated = false;

        QList<QGraphicsItem*> items = this->items();
        if(items.contains(_ghostPoint)){
            this->removeItem(_ghostPoint);
        }
        if(items.contains(_ghostLine)){
            this->removeItem(_ghostLine);
        }
        //delete _ghostPoint;
        //delete _ghostLine;

        this->removeAllPoints();

        _structure->clearSurface();
        _structure->stopHistory(Data::MonofinSurface);
        _structure->dropLastHistory(Data::MonofinSurface);

        emit this->somethingChanged(this->ActionNoAction);

        _state = NormalState;
        emit this->lineInterrupted();
        this->update(this->sceneRect());
        
    }else{
        qDebug("Warning : attempting to stop the creation of the line, but no line is being created");
    }
}

void PaintingScene::undo(){
    _structure->undo(Data::MonofinSurface);

    this->getMonofinFromStructure();
    emit this->somethingChanged(this->ActionUndo);

}

void PaintingScene::updateMonofinDrawing(){
    this->getMonofinFromStructure();
    emit this->somethingChanged(ActionLoadStructure);
}

void PaintingScene::zoomOnBackgroundPicture(qreal factor){
    if(_state == ModifyBackgroundPictureState &&
       _pixItem->getScale()*factor > 0.01 &&
       _pixItem->getScale()*factor < 10){
        _pixItem->scaled(_pixItem->getScale()*factor);
        this->update();
    }

}

//PROTECTED

void PaintingScene::adjustSceneRectangleToItems(){

    //on vérifie que le rectangle de la scène est bien autour de
    //tous les objets de cette scène SAUF AUTOUR DE L'IMAGE DE FOND,
    //sinon on l'ajuste :

    if(_state == ModifyBackgroundPictureState){
        this->removeItem(_pixItem);
    }

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

    //si le rectangle de la scène (légèrement agrandi) ne contient pas
    //tous les items, on l'ajuste
    if(!sceneRect.adjusted(-max, -max, max, max).contains(itemRect)){
        sceneRect = sceneRect.united(itemRect);
        sceneRect.moveLeft(0);
        //on s'arrange pour que le rectangle soit symétrique
        qreal top = sceneRect.top();
        qreal bottom = sceneRect.bottom();
        if(sceneRect.bottom() < 0){
            sceneRect.setBottom(-qMax(qAbs(top), qAbs(bottom)));
        }else{
            sceneRect.setBottom(qMax(qAbs(top), qAbs(bottom)));
        }
        if(sceneRect.top() < 0){
            sceneRect.setTop(-qMax(qAbs(top), qAbs(bottom)));
        }else{
            sceneRect.setTop(qMax(qAbs(top), qAbs(bottom)));
        }
        QRect newSceneRect = sceneRect.toRect();
        this->setSceneRect(newSceneRect);
    }

    //on remet l'item de l'image de fond si on l'avait enlevé
    if(_state == ModifyBackgroundPictureState){
        this->addItem(_pixItem);
    }

    //on réajuste l'axe de symétrie au cas où la taille de la scène
    //aurait changé et on rafraichit la vue
    _axis->setLine(0,0,this->width(), 0);
    this->update(this->sceneRect());
}

void PaintingScene::drawBackground(QPainter* painter, const QRectF& rect){
    Q_UNUSED(rect);

    if(!_isRenderingPicture){

        QTransform matrix(painter->transform());

        if(_hasABackgroundPicture && !_isModifyingBackgroundPicture){

            QPointF offset = _pixItem->offset();
            qreal x = offset.x() + _pixmap->width() / 2;
            qreal y = offset.y() + _pixmap->height() / 2;
            painter->translate(x, y);
            painter->rotate(_pixItem->rotationAngle() + 180);
            painter->scale(_pixItem->getScale(), -_pixItem->getScale());
            painter->translate(-x, -y);

            painter->drawPixmap(_pixItem->offset(), *_pixmap);
        }

        painter->setTransform(matrix, false);

        painter->setPen(Qt::DashDotLine);
        painter->drawRect(this->sceneRect());
        painter->drawRect(this->pointsBoundingZone());

        if(_showGrid){
            painter->setPen(Qt::DotLine);
            painter->setOpacity(0.2);
            QRect r = this->sceneRect().toRect();
            qreal gridUnit = this->gridUnit();
            for(qreal i = 0; i < r.bottomRight().x(); i+=gridUnit){
                painter->drawLine(QLineF(i, r.bottomLeft().y(), i, r.topLeft().y()));
            }
            for(qreal i = 0; i > r.topLeft().y(); i-=gridUnit){
                painter->drawLine(QLineF(r.bottomLeft().x(), i, r.bottomRight().x(), i));
            }
            for(qreal i = 0; i < r.bottomLeft().y(); i+=gridUnit){
                painter->drawLine(QLineF(r.bottomLeft().x(), i, r.bottomRight().x(), i));
            }
        }//if showGrid

        /*QPen pen;
        pen.setWidthF(1.5);
        pen.setColor(Qt::red);

        painter->setOpacity(1);
        painter->setPen(pen);*/

        painter->setPen(QPen());
        painter->setBrush(QBrush("red"));
        painter->setOpacity(0.5);

        QPainterPath path1;
        path1.addRect(QRectF(QPointF(-10,this->sceneRect().bottom()), QPointF(0,this->sceneRect().top())));
        painter->drawPath(path1);

        //painter->drawLine(QPointF(0,this->sceneRect().bottom()), QPointF(0,this->sceneRect().top()));
        //painter->drawRect(QRectF(QPointF(-10,this->sceneRect().bottom()), QPointF(0,this->sceneRect().top())));

        /*painter->setBrush(QBrush("black"));
        painter->setOpacity(0.75);

        QPainterPath path1;
        path1.addRect(rect);
        QPainterPath path2;
        path2.addRect(this->sceneRect());
        path1 -= path2;
        painter->drawPath(path1);*/

        if(_state == CreatePointsState){
            painter->setBrush(QBrush("gray"));
            painter->setOpacity(0.2);

            QPainterPath path2;
            path2.addRect(QRectF(QPointF(0,0), QPointF(this->sceneRect().width(),this->sceneRect().top())));
            painter->drawPath(path2);
        }


    }//if !isRenderingPicture


}

void PaintingScene::getMonofinFromStructure(){

    if(_state == CreatePointsState){
        this->stopCreateLine();
    }

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
                //on n'oublie pas de dire que le point de contrôle peut bouger
                //en dehors de la scène (au cas où la scène actuelle serait trop
                //petite)
                newLine->moveControlPoint(QPointF(x, y), false);
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


        this->adjustSceneRectangleToItems();



    }//end if size > 0

    else{
        //s'il n'y a aucun point à placer, on émet un signal indiquant que
        //la scène est vide
        emit this->pointsOnScene(false);
    }


}

void PaintingScene::keyPressEvent(QKeyEvent* event){

//SUPPR
    /*if(event->key() == Qt::Key_Delete){
        qDebug("suppr");
        this->removeSelectedPoints();

//CTRL
    }else */if(event->key() == Qt::Key_Control){
        _isMultiSelectionKeyActivated = true;

//P
//    }else if(event->key() == Qt::Key_P){
//        this->getPictureOfTheScene(-1,-1);

//ESC
    /*}else if(event->key() == Qt::Key_Escape){
        this->stopCreateLine();*/

//+
    /*}else if(event->key() == Qt::Key_Plus){
        this->adjustSceneRect(20,20);


//-
    }else if(event->key() == Qt::Key_Minus){
        this->adjustSceneRect(-20,-20);
    }

//UP
    else if(event->key() == Qt::Key_Up){
        this->setGridUnit(this->gridUnit() + 1);
    }

//DOWN
    else if(event->key() == Qt::Key_Down){
        this->setGridUnit(this->gridUnit() - 1);
    */}

//PAGE UP
    else if(event->key() == Qt::Key_PageUp){
        this->zoomOnBackgroundPicture(1.2);
    }

//PAGE DOWN
    else if(event->key() == Qt::Key_PageDown){
        this->zoomOnBackgroundPicture(0.8);
    }

}

void PaintingScene::keyReleaseEvent(QKeyEvent* event){
    if(event->key() == Qt::Key_Control){
        _isMultiSelectionKeyActivated = false;
    }
}

void PaintingScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event){
    //qDebug("(%f,%f)", event->scenePos().x(), event->scenePos().y());


    bool intersect = false; //useful for 'create line'
    QPointF pos; //useful for 'create line'
    QRectF r; //useful for 'highlight lines'
    bool isOneSelection = false; //useful for 'highlight lines'

    switch(_state){

//1. CREATE LINE
        case CreatePointsState:

        if(_hasPlacedFirstPoint){
            if(_pointList.isEmpty()){}
            else{
                QLineF templine(_pointList.last()->coord(), _ghostPoint->coord());
                BrLine* bl;
                for(int i = 0; i < _lineList.size()-1; i++){
                    bl = _lineList.at(i);
                    if(bl->intersect(templine)){
                        intersect = true;
                        break;
                    }
                }
            }

        _ghostLine->setCanBePlaced(!intersect);
        }

        _ghostPoint->setCanBePlaced(!intersect);

        pos = event->scenePos();
        if(!_hasPlacedFirstPoint){
            pos.setY(0);
        }else{}
        _ghostPoint->moveTo(pos);

        break;

//2. CREATE SELECTION RECTANGLE
        case NormalState:
        if(_isCreatingSelectionRect){
            _selectionRect->setBottomRight(event->scenePos());
        }

        break;

//3. HIGHLIGHT ITEMS
   // Highlight lines
        case AddControlPointsState:
        case InsertPointsState:

        r = QRectF(event->scenePos().x() - 3, event->scenePos().y() - 3, 6, 6);
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

        break;

        default:
        ;

    }//switch

//ALWAYS
    this->update();
    QGraphicsScene::mouseMoveEvent(event);
}

void PaintingScene::mousePressEvent(QGraphicsSceneMouseEvent* event){
    if(event->button() == Qt::LeftButton){

        //these two booleans are only useful for the case 'create line',
        //but must be initialized here
        bool isFirstPoint = false;
        bool isLastPoint = false;

        switch(_state){

//1. CREATE LINE
            case CreatePointsState:

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
                        QList<QGraphicsItem*> items = this->items();
                        if(items.contains(_ghostPoint)){
                            this->removeItem(_ghostPoint);
                        }
                        if(items.contains(_ghostLine)){
                            this->removeItem(_ghostLine);
                        }
                        //delete _ghostPoint;
                        //delete _ghostLine;
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
                    //_ghostLine = new GhostLine(rect, _ghostPoint, this);
                    _ghostLine->setP1(rect);
                    this->addItem(_ghostLine);
                    isFirstPoint = true;
                }
            //All points :
                this->addItem(rect);
                _pointList.append(rect);

                int key = Data::MONOFIN_SURFACE_NOT_CREATED_POINT;
                key = _structure->addIntersectionPoint(rect->coord().x(), rect->coord().y());

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

                    newLine->setInternalKey(segmentKey);

                }

                _lastPlacedPoint = rect;

            //First and last points :
                if(isFirstPoint){
                    isFirstPoint = false;
                    _hasPlacedFirstPoint = true;
                    PaintingScene::mouseMoveEvent(event);
                }else if(isLastPoint){

                    //WARNING
                    //turn the state to normal state NOW, otherwise problems
                    //with pointers will appear for _ghostPoint and _ghostLine
                    _state = NormalState;

                    foreach(BoundingPoint* bp, _pointList){
                        bp->setFlag(QGraphicsItem::ItemIsSelectable, true);
                    }

                    isLastPoint = false;
                    emit lineFinished(true);
                    _isCreateLineActivated = false;

                    _structure->stopHistory(Data::MonofinSurface);
                    emit this->somethingChanged(this->ActionCreateLine);
                }

            }//end if (can be placed)
            break;


//2. ADD CONTROL POINT
        case AddControlPointsState:

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
            }//foreach

            break;


//3. INSERT POINT
            case InsertPointsState:

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

            break;


//4. REMOVE CONTROL POINT
            case RemoveControlPointsState:

            break;

//5. MODIFY BACKGROUND PICTURE
            case ModifyBackgroundPictureState:

            break;


//6. OTHER
            case NormalState:

    //6.1 Something is highlighted
            if(_isAPointHighlighted){
                if(!_isMultiSelectionKeyActivated){
                    if(!this->selectedItems().contains(_itemUnderMouse)){
                        this->clearSelection();
                        _itemUnderMouse->setSelected(true);
                    }
                }else{
                    _itemUnderMouse->setSelected(!_itemUnderMouse->isSelected());
                }
            }

    //6.2 Nothing is highligthed
        //(selection rectangle)
            else if(_canCreateSelectionRect){
                _isCreatingSelectionRect = true;
                _selectionRect = new SelectionRect(QRectF(event->scenePos(), event->scenePos()));
                this->addItem(_selectionRect);

            }

            break;

//7. ALL THE OTHER CASES
            default:



        ;}//switch

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






