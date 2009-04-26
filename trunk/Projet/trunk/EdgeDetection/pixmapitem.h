#ifndef PIXMAPITEM_H
#define PIXMAPITEM_H

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QWidget>
#include <QPainter>
#include <QRectF>
#include <QGraphicsSceneMouseEvent>
#include <QWheelEvent>
#include <QPointF>
#include "scircle.h"

class SCircle;

class PixmapItem : public QGraphicsPixmapItem
{
public:
    PixmapItem(QPixmap pix = QPixmap());
    ~PixmapItem();

    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

    qreal getRotateAngle(){
        return _rotateAngl;
    }

    SCircle* scircle(){
        return _scircle;
    }

    void rotate2(qreal angl);
    void translate2(qreal x, qreal y);
    void scaled(qreal);
    void center();

    virtual void mousePressEvent ( QGraphicsSceneMouseEvent * event );
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);

protected:
    SCircle* _scircle;

    qreal X;
    qreal Y;

    QPointF _posMousePress;
    QPointF _posMouseMove;

    qreal _rotateAngl;
    qreal _scale;

    bool enter;
};

#endif // PIXMAPITEM_H
