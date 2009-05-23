/*
 * AlgoSnake.h
 *
 *  Created on: 4 mars 2009
 *      Author: Nicolas
 */

#ifndef ALGOSNAKE_H_
#define ALGOSNAKE_H_

#include "abstractalgoedgesextraction.h"
#include "scircle.h"
#include <QImage>

class AlgoSnake : public AbstractAlgoEdgesExtraction{

public:

    /**
     * constructor
     * @param scircle : the algorithm works on this SCircle directly
     */
    AlgoSnake(SCircle* scircle = NULL);

    /**
     * setter
     * @param image : the edges are detected on this image
     */
    void setImage(QImage* image);

    /**
     * implementation of the edges detection algorithm
     */
    virtual void edgesDetection();

    /**
     * implementation of the edges extraction algorithm
     */
    virtual bool edgesExtraction(Data::ProjectFile* monofin, qreal scale, qreal angle, qreal offsetX, qreal offsetY);

protected:

    SCircle* _scircle; // the "snake circle" that be used to detect the form
    QImage* _image; // image that contains the form that will be detected


    /**
     * @param v integer
     * @return the absolute value of a signed integer
     */
    int valAbs(int v);

};

#endif /* ALGOSNAKE_H_ */
