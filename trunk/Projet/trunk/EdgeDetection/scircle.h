/*
 * SCircle.h
 *
 *  Created on: 3 mars 2009
 *      Author: Nicolas
 */

#ifndef SCIRCLE_H_
#define SCIRCLE_H_

#include <QList>
#include <QPoint>
#include "pixmapitem.h"
#include "spoint.h"

class PixmapItem;

class SCircle{

public:

    /**
     * constructor
     * creates a "Snake circle" with a list of "Snake points"
     * the "Snake algorithm" used this object to detect the edges of a form
     * @param cX : the coordinate X of the circle in the scene
     * @param cY : the coordinate Y of the circle in the scene
     * @param radius : the radius of the circle
     * @param pixItem : the "PixmapItem" that contains the image and so the form
     */
     SCircle(qreal cX, qreal cY, qreal radius = 0, PixmapItem* pixItem = NULL);

     /**
      * destructor
      */
    ~SCircle();

    /**
     * setter
     * @param i : the rank of the point in the circle
     * @return the rank i QPoint (in the scene's coordinates)
     */
    QPointF* getQPoint(int i);

    /**
     * getter
     * @param i : the rank of the point in the circle
     * @return the rank i "SPoint" (Snake Point)
     */
    SPoint* getSPoint(int i);

    /**
     * getter
     * @param i : the rank of the point in the circle
     * @param angle : the angle of rotation of the "PixmapItem"
     * @return the rank i QPoint with the rotation of angle (in the scene coordinate)
     */
    QPointF getQPointRotate(int i, qreal angle, qreal scale);

    /**
     * getter
     * @return the "PixmapItem"
     */
    PixmapItem* pixmapItem();

    /**
     * getter
     * @return the radius of the circle
     */
    qreal radius();

    /**
     * getter
     * @return the number of points of the circle
     */
    int getSPointNb();

    /**
     * setter
     * @param radius : the new radius of the circle
     */
    void setRadius(qreal radius);

    /**
     * setter
     * @param cX : the new coordinate X of the circle in the scene
     * @param cY : the new coordinate Y of the circle in the scene
     * @param radius : the new radius of the circle
     */
    void setParam(qreal cX, qreal cY, qreal radius);

    /**
     * @param angle : the angle of rotation of the circle
     * @param offsetX : the offset X
     * @param offsetY : the offset Y
     * @return the rank of the point after the first point, if there is an error return -1
     */
    int firstPoint(qreal angle, qreal offsetX, qreal offsetY, qreal scale, QPointF* fPoint);

    /**
     * give the coefficient of the tangent of a point
     * @param i : a point of the circle
     * @return the coefficient
     */
    qreal tangentX(int i);

    /**
     * give the coefficient of the tangent of a point
     * @param i : a point of the circle
     * @return the coefficient
     */
    qreal tangentY(int i);

     /**
     * give the tangent of a point
     * @param i : a point of the circle
     * @return the tangent
     */
    QLineF tangent(int i);

    /**
     * give the tangent of a special point
     * @param point : a special point wich is on the detected edge but is not a point of the scircle
     * @param i : a point of the circle
     * @return the tangent
     */
    QLineF specialTangent(QPointF point, int i);

    /**
     * give the variation of the coefficient of the tangent for the point i
     * @param i : a point of the circle
     * @return the variation
     */
    qreal tangentVariation(int i);

    /**
     * translate the center of the circle
     * @param dx : the translation along X
     * @param dy : the translation along Y
     */
    void translateCenter(qreal dx, qreal dy);

    /**
     * @return true if all of the points of the circle are fixed else return false
     */
    bool isAllPointsFixed();

    /**
     * add one point to the circle
     */
    void addSPoint();

    /**
     * add nb point(s) to the circle
     * @param nb : the number of points to add
     */
    void addSPoint(int nb);

    /**
     * remove one point to the circle
     */
    void removeSPoint();

    /**
     * remove nb point to the circle
     * if nb is more than the number of the points of the circle, all the points are removed
     * @param nb : the number of points to remove
     */
    void removeSPoint(int nb);

    /**
     * change the number of points of the circle into nb
     * @param nb : the new number of points
     */
    void changeSPointNb(int nb);

    /**
     * reinitialize the circle
     */
    void reinitialize();

protected:
    QList<SPoint*> _circle; // the list of the points of the circle
    QPointF _center; // the center of the circle
    qreal _initRadius; // the initial radius of the circle
    qreal _radius; // the radius of the circle
    int _spointNb; // the number of points of the circle

    PixmapItem* _pixItem; // the "PixmapItem" that contains the form to detect

    /**
     * @param v real
     * @return the absolute value of a signed real
     */
    qreal valAbs(qreal v);

};

inline qreal SCircle::radius(){
        return _radius;
}

inline int SCircle::getSPointNb(){
        return _spointNb;
}

inline PixmapItem* SCircle::pixmapItem(){
    return _pixItem;
}

#endif /* SCIRCLE_H_ */
