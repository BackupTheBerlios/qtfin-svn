#ifndef TANGENT_H
#define TANGENT_H

#include <QWidget>
#include <QGraphicsItem>
#include <QLineF>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QPen>

class Tangent: public QGraphicsItem{

public:
    Tangent(QPointF p1, QPointF p2);

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
    void setLine(QPointF p1, QPointF p2);

protected:
    QLineF* _line;

};

#endif // TANGENT_H
