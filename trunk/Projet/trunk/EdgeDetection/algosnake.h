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
     * getter
     * @return the number of points of the snake
     */
    int numberPointsSnake(){return _numberPointsSnake;}

    /**
     * getter
     * @return the coefficient of detection of the snake
     */
    int coefficientDetectionSnake(){return _coefficientDetectionSnake;}

    /**
     * getter
     * @return the precision of the algorithm Potrace used by the edges extracion
     */
    qreal precisionPotrace(){return _precisionPotrace;}

    /**
     * setter
     * @param image : the edges are detected on this image
     */
    void setImage(QImage* image);

    /**
     * set the differents paramaters of the algorithms
     * @param numberPointsSnake an integer
     * @param coefficientDetectionSnake an integer
     * @param precisionPotrace a float
     */
    void setParameters(int numberPointsSnake, int coefficientDetectionSnake, qreal precisionPotrace);

    /**
     * implementation of the edges detection algorithm
     */
    virtual bool edgesDetection(qreal offserX, qreal offsetY);

    /**
     * implementation of the edges extraction algorithm
     */
    virtual bool edgesExtraction(Data::ProjectFile* monofin, qreal offsetX, qreal offsetY);

    /**
     * reinitialize the algorithms
     */
    virtual void reinitialize();

protected:

    SCircle* _scircle; // the "snake circle" that be used to detect the form
    QImage* _image; // image that contains the form that will be detected
    int _numberPointsSnake;//the number of points of the snake
    int _coefficientDetectionSnake; //the coefficient of detection of the snake
    qreal _precisionPotrace;//the precision of the algorithm Potrace used by the edges extracion

};

inline void AlgoSnake::reinitialize(){
    _scircle->reinitialize();
}

#endif /* ALGOSNAKE_H_ */
