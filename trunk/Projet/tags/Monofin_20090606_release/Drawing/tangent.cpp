#include "tangent.h"
#include "paintingscene.h"

//PUBLIC

Tangent::Tangent(QPointF p1, QPointF p2, PaintingScene* scene)
        :QGraphicsItem(), _scene(scene){
    _line = new QLineF(p1, p2);
}

Tangent::~Tangent(){
    delete _line;
}

QRectF Tangent::boundingRect() const{
    return QRectF(_line->p1(), _line->p2());
}

void Tangent::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget){
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if(!_scene->isSimplifyViewActivated()){
        painter->setPen(Qt::DashLine);
        painter->drawLine(*_line);
    }else{
        Q_UNUSED(painter);
    }
}

void Tangent::setLine(QPointF p1, QPointF p2){
    _line->setPoints(p1,p2);

}
