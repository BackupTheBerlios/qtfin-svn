#include "symmetryaxis.h"

//PUBLIC

SymmetryAxis::SymmetryAxis(PaintingScene* scene)
        :_scene(scene){
    this->setLine(0, 0, _scene->width(), 0);
    //_line = new QLineF(0, 0, _scene->width(), 0);
}

void SymmetryAxis::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget){
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setPen(QColor(200,100,50));
    painter->drawLine(this->line());
}
