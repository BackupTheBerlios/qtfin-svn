#include "ghostpoint.h"
#include "paintingscene.h"

//PUBLIC

GhostPoint::GhostPoint(QPointF coord, PaintingScene* scene)
        :QGraphicsItem(), _canBePlaced(false), _scene(scene){
    _p = new BoundingPoint(coord, scene);
}

GhostPoint::~GhostPoint(){
    delete _p;
}

void GhostPoint::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget){
    painter->setOpacity(0.8);
    if(_canBePlaced){painter->setPen(Qt::black);}
    else{painter->setPen(Qt::red);}
    painter->drawRect(_p->rect());

}
