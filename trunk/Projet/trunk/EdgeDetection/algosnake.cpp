/*
 * AlgoSnake.cpp
 *
 *  Created on: 5 mars 2009
 *      Author: Nicolas
 */
#include <QColor>
#include "algosnake.h"
#include <QThread>

AlgoSnake::AlgoSnake(SCircle* scircle):
        AbstractAlgoEdgesExtraction(), _scircle(scircle), _image(NULL){}

int AlgoSnake::valAbs(int v){
    if (v < 0)
        return - v;
    return v;
}

void AlgoSnake::edgesDetection(){
    if(_scircle != NULL && _image != NULL){
        for(int i = _scircle->getRadius(); i > 0  && !_scircle->isAllPointsFixed(); i--){
            _scircle->setRadius(i);
            for(int j = 0; j < _scircle->getSPointNb(); j++){
                QPoint* qp = _scircle->getQPoint(j);
                SPoint* sp = _scircle->getSPoint(j);
                if(_image->valid(*qp)){
                    int gray = qGray(_image->pixel(*qp));
                    if(sp->getGrayValue() != -1){
                        int t = valAbs(sp->getGrayValue() - gray);
                        if(t > 25)
                        sp->setRadiusFixed(true);
                    }
                    sp->setGrayValue(qGray(_image->pixel(*qp)));
                }
            }
        }
    }
}


void AlgoSnake::edgesExtraction(){

}
