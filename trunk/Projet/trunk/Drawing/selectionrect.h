#ifndef SELECTIONRECT_H
#define SELECTIONRECT_H

#include <QGraphicsRectItem>
#include <QRectF>
#include <QPointF>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>

class SelectionRect: public QGraphicsRectItem{

public:
    /**
    * Creates a selection rectangle by calling the constructor of
    * QGraphicsRectItem with the given rectangle.
    *@rectangle the rectangle of the item : passed to the constructor
    * of QGraphicsRectItem
    **/
    SelectionRect(QRectF rectangle);

    /**
    * Draws the item's rectangle with a dash line and a little opacity.
    *@param painter the painter which will be used to draw the rectangle
    *@param option not used here, see the documentation of QGraphicsItem
    *@param widget not used here, see the documentation of QGraphicsItem
    **/
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

    /**
    * Recreates the rectangle of the item by keeping its pevious top left
    * point and with the given bottom right point.
    *@param p the new bottom right point of the item
    **/
    void setBottomRight(QPointF p);

};

#endif // SELECTIONRECT_H
