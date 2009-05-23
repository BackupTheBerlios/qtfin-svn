#include "brline.h"
#include <QGraphicsLineItem>

//PUBLIC

BrLine::BrLine(BoundingPoint* p1, BoundingPoint* p2, PaintingScene* scene)
    : QGraphicsItem(),
    _internalKey(Data::MONOFIN_SURFACE_NOT_CREATED_SEGMENT),
    _isControlPointActivated(false), _isMouseOnLine(false),
    _p1(p1), _p2(p2), _scene(scene){

    _colorWhenNormal = _scene->getColor(PaintingScene::LineColor,
                                        PaintingScene::NormalColor);
    _colorWhenHighlighted = _scene->getColor(PaintingScene::LineColor,
                                        PaintingScene::HighlightingColor);
    
    _line = new QLineF(_p1->coord(), _p2->coord());
    QPointF contr((_p2->coord().x()+_p1->coord().x())/2.0,
                  (_p2->coord().y()+_p1->coord().y())/2.0);
    _contr = new ControlPoint(contr, this, _scene);

    _scene->addControlPoint(_contr);

    _tangent1 = new Tangent(_line->p1(), _contr->coord(), _scene);
    _tangent2 = new Tangent(_line->p2(), _contr->coord(), _scene);
    _contr->setTangent1(_tangent1);
    _contr->setTangent2(_tangent2);

    _path = new QPainterPath(_p1->coord());
    _path->cubicTo(_contr->coord(), _contr->coord(), _p2->coord());

    //this->setAcceptHoverEvents(true);
}

BrLine::~BrLine(){
    this->deleteBezier();
    delete _line;
    delete _path;

    //on cache la ligne à ses points d'extrémité avant de la supprimer

    //on vérifie laquelle des lignes de p1 est celle que l'on supprime
    if(_p1 != 0){
        if(_p1->hasRightLine()){
            if(_p1->rightLine() == this){
                _p1->hideRightLine();
            }
        }
        if(_p1->hasLeftLine()){
            if(_p1->leftLine() == this){
                _p1->hideLeftLine();
            }
        }
    }//end if p1 != 0

    //on fait la même chose pour p2
    if(_p2 != 0){
        if(_p2->hasLeftLine()){
            if(_p2->leftLine() == this){
                _p2->hideLeftLine();
            }
        }
        if(_p2->hasRightLine()){
            if(_p2->rightLine() == this){
                _p2->hideRightLine();
            }
        }
    }//end if p2 != 0
}

QPainterPath BrLine::bezierCurve(){
    this->updatePath();
    return *_path;
}

QRectF BrLine::boundingRect() const{

    QRectF r = _path->boundingRect();

    //ici on crée un rectangle immense qui englobe la courbe et son symétrique
    return QRectF(r.bottomLeft(), QPointF(r.bottomRight().x(), -r.bottomRight().y()));

    //return QRectF(_line->p1(), _line->p2());
}

QPointF BrLine::cubicBezierPoint(QPointF p0, QPointF p1, QPointF p2, QPointF p3, qreal t){
    if(t < 0 || t > 1){
        return QPointF();
    }
    qreal x = p0.x()*(1-t)*(1-t)*(1-t)
              + 3 * p1.x() * t * (1-t) * (1-t)
              + 3 * p2.x() * t * t * (1-t)
              + p3.x() * t * t * t;
    qreal y = p0.y()*(1-t)*(1-t)*(1-t)
              + 3 * p1.y() * t * (1-t) * (1-t)
              + 3 * p2.y() * t * t * (1-t)
              + p3.y() * t * t * t;
    return QPointF(x,y);
    //P(t) = P0(1-t)^3 + 3P1t(1-t)² + 3P2t²(1-t) + P3t^3
}

QPointF BrLine::cubicBezierPoint(qreal t){
    return cubicBezierPoint(_line->p1(), _contr->coord(), _contr->coord(), _line->p2(), t);
}

bool BrLine::intersect(BrLine& line){
    QPointF* p = new QPointF();
    bool res = (_line->intersect(line.line(), p) == QLineF::BoundedIntersection);
    delete p;
    return res;
}

bool BrLine::intersect(QLineF& line){
    QPointF* p = new QPointF();
    bool res = (_line->intersect(line, p) == QLineF::BoundedIntersection);
    delete p;
    return res;
}

bool BrLine::intersects(BrLine& line) const{
    //il y a une intersection entre deux lignes si elles sont reliées au même
    //point, du coup, on teste l'intersection avec une courbe de bezier sans
    //les deux points extrêmes
    QPainterPath* withoutPoints = new QPainterPath(_line->pointAt(0.000000001));
    withoutPoints->cubicTo(_contr->coord(), _contr->coord(), _line->pointAt(0.999999999));
    return withoutPoints->intersects(line.bezierCurve());
}

void BrLine::move(){
    this->prepareGeometryChange();

    //si le point de contrôle n'est pas activé, on le déplace en
    //même temps que la ligne en le laissant au milieu
    if(!_isControlPointActivated){
        QPointF contr((_p2->coord().x()+_p1->coord().x())/2.0,
                      (_p2->coord().y()+_p1->coord().y())/2.0);
        _contr->moveTo(contr);
    }


    _line->setPoints(_p1->coord(),_p2->coord());

    _tangent1->setLine(_line->p1(), _contr->coord());
    _tangent2->setLine(_line->p2(), _contr->coord());

    this->updatePath();
}

void BrLine::moveControlPoint(QPointF pos){
    _contr->moveTo(pos);
    _tangent1->setLine(_line->p1(), _contr->coord());
    _tangent2->setLine(_line->p2(), _contr->coord());
}

BoundingPoint* BrLine::otherPointAs(BoundingPoint* p){
    if(p == this->_p1){
        return _p2;
    }else if(p == this->_p2){
        return _p1;
    }else{
        return 0;
    }
}

void BrLine::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget){
    Q_UNUSED(option);
    Q_UNUSED(widget);

    this->updatePath();
    painter->setRenderHint(QPainter::Antialiasing, true);
    QPen pen;
    int pw = 2;// / _scene->scaleFactor();
    //qDebug("Pen Width : %d", pw);
    pen.setWidth(pw);
    if(_isMouseOnLine){
        pen.setColor(_colorWhenHighlighted);
    }else{
        pen.setColor(_colorWhenNormal);
    }

    painter->setPen(pen);

    if(_isControlPointActivated){
        QPainterPath path;
        path.moveTo(_line->p1());
        path.cubicTo(_contr->coord(),_contr->coord(), _line->p2());
        painter->drawPath(path);


    }else{
        painter->drawLine(*_line);
    }


  //affichage du symétrique
    if(!_scene->isSimplifyViewActivated()){
        painter->setPen(_colorWhenNormal);
        painter->setOpacity(0.5);
    }else{
        pen.setColor(_colorWhenNormal);
        painter->setPen(pen);
    }
    if(_isControlPointActivated){
        QPainterPath path;
        path.moveTo(QPointF(_line->p1().x(), -_line->p1().y()));
        path.cubicTo(QPointF(_contr->coord().x(), -_contr->coord().y()),
                     QPointF(_contr->coord().x(), -_contr->coord().y()),
                     QPointF(_line->p2().x(), -_line->p2().y()));
        painter->drawPath(path);
    }else{
        painter->drawLine(QLineF(_line->x1(), -_line->y1(), _line->x2(), -_line->y2()));

    }

}

void BrLine::setColorWhenHighlighted(const QColor& color){
    if (color.isValid()){
        _colorWhenHighlighted = color;
    }
}

void BrLine::setColorWhenNormal(const QColor& color){
    if (color.isValid()){
        _colorWhenNormal = color;
    }
}

void BrLine::setControlPoint(bool on){
    if (on){
        if(!_isControlPointActivated){
            _scene->addItem(_contr);
            _scene->addItem(_tangent1);
            _scene->addItem(_tangent2);
            _scene->update(_scene->sceneRect());
            _isControlPointActivated = true;
        }
    }else{
        _isControlPointActivated = false;
        _scene->removeItem(_contr);
        _scene->removeItem(_tangent1);
        _scene->removeItem(_tangent2);
        _contr->moveTo(_line->pointAt(0.5));
    }
    _scene->update(_scene->sceneRect());
}

//PROTECTED

void BrLine::deleteBezier(){
    //on appelle une fonction pour supprimer le point de contrôle
    //puisque celui-ci est stocké dans une liste dans la PaintingScene.
    //le deuxième paramètre indique s'il est ou non présent dans la scène
    _scene->removeControlPoint(_contr, _isControlPointActivated);

    //si les tangentes appartiennet à la scène, on les supprime de celle-ci
    if(_isControlPointActivated){
        _scene->removeItem(_tangent1);
        _scene->removeItem(_tangent2);
    }

    //Ensuite, on supprime directement les tangentes
    if(_tangent1 != 0){delete _tangent1;}
    if(_tangent2 != 0){delete _tangent2;}

}


void BrLine::updatePath(){
    *_path = QPainterPath(_p1->coord());
    _path->cubicTo(_contr->coord(), _contr->coord(), _p2->coord());
    _path->cubicTo(_contr->coord(), _contr->coord(), _p1->coord());
}
