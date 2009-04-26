#ifndef ROTATECIRCLE_H
#define ROTATECIRCLE_H

#include <QGraphicsRectItem>
#include <QApplication>
#include <QObject>
#include "pixmapitem.h"

class RotateCircle : public QGraphicsRectItem
{
public:
    RotateCircle( PixmapItem * parent = 0);

    void setPosition(qreal angle);
    bool isMoved(){
        return _isMoved;
    }


protected:

    bool _enter;
    bool _isMoved;
    qreal _radius;

    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);

};

#endif // ROTATECIRCLE_H
