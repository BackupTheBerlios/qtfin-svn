#ifndef SYMMETRYAXIS_H
#define SYMMETRYAXIS_H

#include <QGraphicsItem>
#include <QRectF>
#include <QLineF>
#include <QPainter>
#include <QColor>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include "paintingscene.h"

class PaintingScene;

class SymmetryAxis: public QGraphicsItem{

public:
    SymmetryAxis(PaintingScene* scene);

    virtual QRectF boundingRect() const;
    QLineF line(){return *_line;}
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);


protected:
    QLineF* _line;
    PaintingScene* _scene;

};

#endif // SYMMETRYAXIS_H
