/*
 * SPoint.h
 *
 *  Created on: 1 mars 2009
 *      Author: Nicolas
 */

#ifndef SPOINT_H
#define SPOINT_H


#include <QPoint>

class SPoint{

public:

    /**
     * constructor
     * create a "Snake point" that is attached to a pixel of the image (thanks to the snake algorithm)
     * @param radPos : the radial position of the point on the "Snake circle"
     * @param N : the number of "SPoint" in the circle
     * @param center : the center of the "SCircle"
     * @param radius : the radius of the "SCircle"
     */
    SPoint(int radPos = 0, int N = 0, const QPointF& center = QPointF(0,0), qreal radius = 0);

    /**
     * getter
     * @return the radial position of the point
     */
    int radPos();

    /**
     * getter
     * @return the point in the scene coordinate
     */
    QPointF& carthPos();

    /**
     * getter
     * @return the level of grey of the pixel that is attached to the point
     */
    int grayValue();

    /**
     * setter
     * @param v : the new level of grey of the pixel that is attached to the point
     */
    void setGrayValue(int v);

    /**
     * setter
     * @param radius :  the new radius of the circle
     */
    void setRadius(qreal radius);

    /**
     * setter
     * @param v : the boolean that says if the point is fixed or not
     */
    void setRadiusFixed(bool v);

    /**
     * @return true if the point if fixed, else return false
     */
    bool isRadiusFixed();

    /**
     * set the coordinate of the point in the scene's coordinates thanks to the radial's coordinates
     * @param N : the number of "SPoint" in the circle
     * @param center : the center of the "SCircle"
     * @param radius : the radius of the "SCircle"
     */
    void radToCarth(int N, const QPointF& center, qreal radius);

    /**
     * @param N : the number of points of the circles
     * @param center : the center of the circle
     * @param angle : the angle of rotation of the "PixmapItem"
     * @return QPoint with the rotation of angle (in the scene coordinate)
     */
    QPointF posRotate(int N, const QPointF& center, qreal angle, qreal scale);

protected:
    int _radPos; // the radial position
    QPointF _carthPos; // the cartesian's coordinates
    qreal _radius; // the radius of the circle that contains the point

    int _grayValue; // the level of grey of the pixel that is attached to the point
    bool _rayonFixe; // true if the point is fixed in the scene , else false
};

inline int SPoint::radPos(){
        return _radPos;
    }

inline QPointF& SPoint::carthPos(){
        return _carthPos;
    }

inline int SPoint::grayValue(){
        return _grayValue;
    }

inline void SPoint::setGrayValue(int v){
        _grayValue = v;
    }

inline void SPoint::setRadius(qreal radius){
    _radius = radius;
}

inline bool SPoint::isRadiusFixed(){
        return _rayonFixe;
    }

inline void SPoint::setRadiusFixed(bool v){
        _rayonFixe = v;
    }

#endif /* SPOINT_H_ */
