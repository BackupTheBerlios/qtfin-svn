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
    virtual bool edgesDetection(qreal offserX, qreal offsetY);

    /**
     * implementation of the edges extraction algorithm
     */
    virtual bool edgesExtraction(Data::ProjectFile* monofin, qreal offsetX, qreal offsetY);

    /**
     * reinitialize the algorithms
     */
    virtual void reinitialize();

    void setParameters(int numberPointsSnake, int coefficientDetectionSnake, qreal precisionPotrace);

    int numberPointsSnake(){return _numberPointsSnake;}

    int coefficientDetectionSnake(){return _coefficientDetectionSnake;}

    qreal precisionPotrace(){return _precisionPotrace;}

protected:

    SCircle* _scircle; // the "snake circle" that be used to detect the form
    QImage* _image; // image that contains the form that will be detected

    int _numberPointsSnake;
    int _coefficientDetectionSnake;
    qreal _precisionPotrace;

};

inline void AlgoSnake::reinitialize(){
    _scircle->reinitialize();
}

#endif /* ALGOSNAKE_H_ */
