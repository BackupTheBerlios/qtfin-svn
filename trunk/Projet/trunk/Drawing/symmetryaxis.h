#ifndef SYMMETRYAXIS_H
#define SYMMETRYAXIS_H

#include <QGraphicsLineItem>
#include <QPainter>
#include <QColor>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include "paintingscene.h"

class PaintingScene;

class SymmetryAxis: public QGraphicsLineItem{

public:
    /**
    * Constructs a QGraphicsLineItem object and sets the line to a straight
    * line from the scene coordinates (0,0) to (scene rectangle's width,0).
    *@param scene a pointer to the main PaintingScene
    **/
    SymmetryAxis(PaintingScene* scene);


    /**
    * Draws the axis as a red/orange line.
    *@param painter the painter which will be used to draw the line
    *@param option not used here, see the documentation of QGraphicsItem
    *@param widget not used here, see the documentation of QGraphicsItem
    **/
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);


protected:
    PaintingScene* _scene;

};

#endif // SYMMETRYAXIS_H
