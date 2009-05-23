#ifndef ROTATECIRCLE_H
#define ROTATECIRCLE_H

#include <QGraphicsRectItem>
#include <QApplication>
#include <QObject>
#include "pixmapitem.h"

class RotateCircle : public QGraphicsRectItem
{
public:
    /**
     * constructor
     * creates an object allowing to rotate a "pixmatItem"
     * @param parent : the "pixmapItem" that is rotated by the "rotateCircle"
     */
    RotateCircle( PixmapItem * parent = 0);

    /**
     * setter
     * @param angle : the angle of the item (same angle than the "pixmapItem")
     */
    void setPosition(qreal angle);

    /**
     * setter
     * @param scale : the scale of the item
     */
    void setScale(qreal scale);

    /**
     * @return true if the "rotateCircle" is moving else return false
     */
    bool isMoved();

    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);


protected:

    bool _enter; // boolean ...
    bool _isMoved;
    qreal _radiusInit;
    qreal _radius; // the radius
    qreal _scale;

     /**
     * different overloaded functions that handle the mouse events
     */

    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);

};

inline bool RotateCircle::isMoved(){
    return _isMoved;
}

inline void RotateCircle::setScale(qreal scale){
    _scale = scale;
    //_radius = _radiusInit * scale;
}

#endif // ROTATECIRCLE_H
