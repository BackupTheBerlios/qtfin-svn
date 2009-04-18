#include "ghostline.h"
#include "boundingpoint.h"
#include "ghostpoint.h"
#include "paintingscene.h"

//PUBLIC

GhostLine::GhostLine(BoundingPoint* p1, GhostPoint* p2, PaintingScene* scene)
        :QGraphicsItem(), _canBePlaced(false), _p1(p1), _p2(p2)
        {}

QRectF GhostLine::boundingRect() const{
    return QRectF(_p1->coord(), _p2->coord());
}

void GhostLine::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget){
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setOpacity(0.8);
    painter->setRenderHint(QPainter::Antialiasing, true);
    if(_canBePlaced){painter->setPen(Qt::black);}
    else{painter->setPen(Qt::red);}
    painter->drawLine(_p1->coord(), _p2->coord());

    painter->setOpacity(0.5);
    painter->drawLine(QLineF(_p1->coord().x(), -_p1->coord().y(), _p2->coord().x(), -_p2->coord().y()));
}
