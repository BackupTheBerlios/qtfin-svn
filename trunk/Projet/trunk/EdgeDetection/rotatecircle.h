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
    RotateCircle( PixmapItem * parent);

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

    /**
     * see the Qt Reference Documentation
     */
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);


protected:

    bool _enter; // boolean that is true if the mouse is on the rotateCirle
    bool _isMoved; // boolean that is true if the rotateCircle is moving
    qreal _radiusInit; // the initial radius of circle around wich the rotateCircle revolves
    qreal _radius; // the actual radius of the circle
    qreal _scale; //the scale of the item

    /**
     * different overloaded functions that handle the mouse events
     * see the Qt Reference Documentation
     */
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);

};

inline bool RotateCircle::isMoved(){
    return _isMoved;
}

#endif // ROTATECIRCLE_H
