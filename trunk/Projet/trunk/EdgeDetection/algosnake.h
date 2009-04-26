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

private:

    SCircle* _scircle; // the "snake circle" that be used to detect the form
    QImage* _image; // image that contains the form that will be detected

    /**
     * @param v integer
     * @return the absolute value of a signed integer
     */
    int valAbs(int v);

public:

    /**
     * constructor
     */
    AlgoSnake(SCircle* scircle = NULL);

    /**
     * setter
     * @param image
     */
    void setImage(QImage* image){
        _image = image;
    }

    virtual void edgesDetection();
    virtual void edgesExtraction();

};

#endif /* ALGOSNAKE_H_ */
