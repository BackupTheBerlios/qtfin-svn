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
    SelectionRect(QRectF rectangle);

    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
    void setBottomRight(QPointF p);

};

#endif // SELECTIONRECT_H
