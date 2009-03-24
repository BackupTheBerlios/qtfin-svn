#include "brline.h"
#include <QGraphicsLineItem>

//PUBLIC

BrLine::BrLine(BoundingPoint* p1, BoundingPoint* p2, PaintingScene* scene, int id)
    : QGraphicsItem(), _color(Qt::black), _isControlPointActivated(false),
    _isMouseOnLine(false), _lineId(id),
    _p1(p1), _p2(p2), _scene(scene){

    _line = new QLineF(_p1->coord(), _p2->coord());
    QPointF contr((_p2->coord().x()+_p1->coord().x())/2.0,
                  (_p2->coord().y()+_p1->coord().y())/2.0);
    _contr = new ControlPoint(contr, this, _scene);

    _scene->addControlPoint(_contr,_lineId);

    _tangent1 = new Tangent(_line->p1(), _contr->coord());
    _tangent2 = new Tangent(_line->p2(), _contr->coord());
    _contr->setTangent1(_tangent1);
    _contr->setTangent2(_tangent2);

    _path = new QPainterPath(_p1->coord());
    _path->cubicTo(_contr->coord(), _contr->coord(), _p2->coord());

    this->setAcceptHoverEvents(true);
}

QRectF BrLine::boundingRect() const{
    return QRectF(_line->p1(), _line->p2());
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

void BrLine::move(){
    qDebug("move line");
    this->prepareGeometryChange();
    if(!_isControlPointActivated){
        QPointF contr((_p2->coord().x()+_p1->coord().x())/2.0,
                      (_p2->coord().y()+_p1->coord().y())/2.0);
        _contr->moveTo(contr);
    }
    _line->setPoints(_p1->coord(),_p2->coord());
    _tangent1->setLine(_line->p1(), _contr->coord());
    _tangent2->setLine(_line->p2(), _contr->coord());
    this->updatePath();
    //_scene->update(QRectF(_scene->sceneRect()));
}

void BrLine::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget){
    Q_UNUSED(option);
    Q_UNUSED(widget);

    this->updatePath();
    painter->setRenderHint(QPainter::Antialiasing, true);
    QPen pen;
    int pw = 2;
    //qDebug("Pen Width : %d", pw);
    pen.setWidth(pw);
    if(_isMouseOnLine){
        _color = Qt::blue;
    }else{
        _color = Qt::black;
    }
    pen.setColor(_color);

    painter->setPen(pen);

    if(_isControlPointActivated){
        QPainterPath path;
        path.moveTo(_line->p1());
        path.cubicTo(_contr->coord(),_contr->coord(), _line->p2());
        painter->drawPath(path);
    }else{
        painter->drawLine(*_line);
    }

    /*painter->setPen(Qt::blue);

    QPointF p1 = _scene->cubicBezierPoint(_line->p1(), _contr->coord(), _contr->coord(), _line->p2(), 0);
    QPointF p2;

    for(qreal i = 0.1; i <= 1; i += 0.1){
        p2 = _scene->cubicBezierPoint(_line->p1(), _contr->coord(), _contr->coord(), _line->p2(), i);
        painter->drawLine(p1,p2);
        p1 = p2;
    }*/

    //affichage du symétrique
    painter->setPen(Qt::black);
    painter->setOpacity(0.5);
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

void BrLine::removeFromScene(){
    _p1->hideRightLine();
    _p2->hideLeftLine();
    _scene->removeControlPoint(_contr);
    _scene->removeItem(_tangent1);
    _scene->removeItem(_tangent2);
    delete _contr;
    delete _tangent1;
    delete _tangent2;
    _scene->removeLine(this);
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
}

void BrLine::setLineId(int i){
    if(i >= 0){ _lineId = i;}
}

void BrLine::updatePath(){
    *_path = QPainterPath(_p1->coord());
    _path->cubicTo(_contr->coord(), _contr->coord(), _p2->coord());
    _path->cubicTo(_contr->coord(), _contr->coord(), _p1->coord());
}


