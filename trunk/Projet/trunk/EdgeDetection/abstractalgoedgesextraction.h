/*
 * AbstractAlgoEdgeExtraction.h
 *
 *  Created on: 3 mars 2009
 *      Author: Nicolas
 */


#ifndef ABSTRACTALGOEDGESEXTRACTION_H_
#define ABSTRACTALGOEDGESEXTRACTION_H_

#include "../Data/projectfile.h"

/**
 * Abstract class that contains signatures of the edges detection and edges extration algorithmes
 */

class AbstractAlgoEdgesExtraction {
public:

    /**
     * Detect the form's edges on a picture
     * This form has to be unique
     */
    virtual bool edgesDetection(qreal offsetX) = 0;

    /**
     * Extract the form detected previously and transform this form into a Monofin struture
     */
    virtual bool edgesExtraction(Data::ProjectFile* monofin, qreal offsetX, qreal offsetY) = 0;

    virtual void reinitialize() = 0;
};


#endif /* ABSTRACTALGOEDGEEXTRACTION_H_ */
