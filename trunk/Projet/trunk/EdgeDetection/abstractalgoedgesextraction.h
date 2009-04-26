/*
 * AbstractAlgoEdgeExtraction.h
 *
 *  Created on: 3 mars 2009
 *      Author: Nicolas
 */


#ifndef ABSTRACTALGOEDGESEXTRACTION_H_
#define ABSTRACTALGOEDGESEXTRACTION_H_

/**
 * Abstract class that contains signatures of the edges detection and edges extration algorithmes
 */

class AbstractAlgoEdgesExtraction {
public:

    /**
     * Detect the form's edges on a picture
     * This form had to be unique
     */
    virtual void edgesDetection() = 0;

    /**
     * Extract the form previously detected and transform this form into a Monofin struture
     */
    virtual void edgesExtraction() = 0;

};


#endif /* ABSTRACTALGOEDGEEXTRACTION_H_ */
