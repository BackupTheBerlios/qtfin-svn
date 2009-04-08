#include "symmetryaxis.h"

//PUBLIC

SymmetryAxis::SymmetryAxis(PaintingScene* scene)
        :_scene(scene){
    _line = new QLineF(0, 0, _scene->width(), 0);
}

SymmetryAxis::~SymmetryAxis(){
    delete _line;
}

QRectF SymmetryAxis::boundingRect() const{
    return QRectF(_line->p1(), _line->p2());
}

void SymmetryAxis::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget){
    painter->setPen(QColor(200,100,50));
    painter->drawLine(*_line);
}
