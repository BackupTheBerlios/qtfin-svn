/*
 * AlgoSnake.cpp
 *
 *  Created on: 5 mars 2009
 *      Author: Nicolas
 */
#include <QColor>
#include "algosnake.h"
#include <QThread>
#include <QHash>
#include <QLineF>

using namespace Data;

AlgoSnake::AlgoSnake(SCircle* scircle):
        AbstractAlgoEdgesExtraction(), _scircle(scircle), _image(NULL){
    _scircle->addSPoint(100);
}

int AlgoSnake::valAbs(int v){
    if (v < 0)
        return - v;
    return v;
}

void AlgoSnake::setImage(QImage* image){
        _image = image;
}

bool AlgoSnake::edgesDetection(qreal offsetX){
    QPointF offsetI = _scircle->pixmapItem()->offset();
    if(_scircle != NULL && _image != NULL){
        for(qreal i = _scircle->radius(); i > 0  && !_scircle->isAllPointsFixed(); i--){
            _scircle->setRadius(i);
            for(int j = 0; j < _scircle->getSPointNb(); j++){
                QPointF* qp = _scircle->getQPoint(j);
                SPoint* sp = _scircle->getSPoint(j);
                if(_image->valid(qp->toPoint())){
                    int gray = qGray(_image->pixel(qp->toPoint()));
                    if(sp->grayValue() != -1){
                        int t = valAbs(sp->grayValue() - gray);
                        if(t > 25)
                        sp->setRadiusFixed(true);
                    }
                    sp->setGrayValue(qGray(_image->pixel(qp->toPoint())));
                }
            }
        }
        for(int i = 0; i < _scircle->getSPointNb(); i++){
            QPointF point = _scircle->getQPointRotate(i, _scircle->pixmapItem()->rotationAngle(),
                                         _scircle->pixmapItem()->getScale()) + offsetI;
            if(point.y() < 0 || point.x() > offsetX)
                return false;
        }
    }
    return true;
}


bool AlgoSnake::edgesExtraction(ProjectFile* monofin, qreal scale, qreal angle, qreal offsetX, qreal offsetY){

    /*
     * Initialisation des variables utiles
     */
    QList<int> pointsToKeepInOrder;
    qreal marge = 1;
    QPointF offsetI = _scircle->pixmapItem()->offset();
    int firstPoint = -1;
    int secondPoint = -2;
    int lastPoint = -3;
    QPointF firstP;
    QPointF secondP;
    QPointF lastP;

    /*
     * Detection du premier point
     */
    QPointF* fp = new QPointF();
    int np = _scircle->firstPoint(angle, offsetX, offsetY, scale, fp);
    qDebug("Premier point : (%f, %f)", fp->x(), fp->y());
    qDebug("Prochain point : %d", np);

    /*
     * si il n'y a pas de premier point, retourne faux
     */
    if(np == -1)
        return false;
    
    /*
     * On regarde si le premier point est sur l'axe de symétrie ou sur le talon
     * Ajout du premier point, du second point si nécessaire et du prochain point dans la liste
     */

    if(fp->y() == offsetY){
        firstP = *fp;
        pointsToKeepInOrder.push_back(firstPoint);
    }else{
        firstP = QPointF(offsetX, offsetY);
        secondP = *fp;
        pointsToKeepInOrder.push_back(firstPoint);
        pointsToKeepInOrder.push_back(secondPoint);
    }
    delete fp;
    /*
     * On parcours la liste des points jusqu'à repasser par l'axe de symétrie ou fait le tour complet
     * On essaye de garder les points caractéristiques
     * Si on ne repasse pas par l'axe de symétrie, ça marche pas
     */
    bool finish = false;
    for(int i = np; i < _scircle->getSPointNb() && !finish; i++){
        QPointF cp = _scircle->getQPointRotate(i, angle, scale);
        if(cp.y() + offsetI.y() >= offsetY){
            QPointF* lp = new QPointF();
            QLineF(0, offsetY, offsetX * 10, offsetY).intersect(QLineF(_scircle->getQPointRotate(i - 1, angle, scale) + offsetI, cp),
                                                        lp);
            lastP = *lp;
            pointsToKeepInOrder.push_back(lastPoint);
            finish = true;
            delete lp;
        }else{
            qreal tv = _scircle->tangentVariation(i);
            if(tv >= marge|| tv <=- marge)
                pointsToKeepInOrder.push_back(i);
        }
    }
    for(int i = 0; i < np && !finish; i++){
        QPointF cp = _scircle->getQPointRotate(i, angle, scale);
        if(cp.y() + offsetI.y() >= offsetY){
            QPointF* lp = new QPointF();
            QLineF(0, offsetY, offsetX * 10, offsetY).intersect(QLineF(_scircle->getQPointRotate(i - 1, angle, scale) + offsetI, cp),
                                                        lp);
            lastP = *lp;
            pointsToKeepInOrder.push_back(lastPoint);
            finish = true;
            delete lp;
        }else{
            qreal tv = _scircle->tangentVariation(i);
            if(tv >= marge|| tv <= -marge)
                pointsToKeepInOrder.push_back(i);
        }
    }
    for(int i = 0; i < pointsToKeepInOrder.size(); i++){
        qDebug("Point gardé : %d", pointsToKeepInOrder.value(i));
    }
    if(!finish){
        qDebug("bug");
        return false;
    }

    /*
     * Ajout des points dans le project File avec les points de contrôle
     */

    /*
     * Ajout du premier point dans monofin
     */
     qDebug("monofin est-il initialisé ?");
     monofin->startHistory(Data::MonofinSurface);
     int fpk = monofin->addIntersectionPoint(firstP.x(), offsetY - firstP.y());
     qDebug("OUIIIIII!!!");
    /*
     * Vérifie si le second point est sur le talon
     */
     if(pointsToKeepInOrder.value(1) == secondPoint){
        /*
         * le second point est sur le talon
         */
         qDebug("passe par talon");
        /*
         * ajout du second point dans monofin
         */
        int spk = monofin->addIntersectionPoint(secondP.x(), offsetY - secondP.y());
        /*
         * ajout du point de controle entre le premier et le second point
         */
        int fcpk = monofin->addControlPoint(firstP.x(), offsetY - (secondP.y() + (firstP.y() - secondP.y()) / 2) );
        /*
         * ajout du premier segment entre le premier et le second point
         */
        monofin->addSegment(fpk, spk, fcpk);

        /*
         * ajout du troisième point
         */
        QPointF np1 = _scircle->getQPointRotate(np, angle, scale) + offsetI; // np1 : next point 1
        int npk1 = monofin->addIntersectionPoint(np1.x(), offsetY - np1.y());// npk1 : next point key 1

        /*
         * calcul de la position du point de controle entre le second et le troisième point
         */
        QPointF* cp = new QPointF();// control point
        _scircle->specialTangent(secondP, np, angle, scale).intersect(_scircle->tangent(np, true, angle, scale), cp);
        /*
         * ajout point de controle entre le second et le troisième point
         */
        fcpk = monofin->addControlPoint(cp->x(), offsetY - cp->y());

        /*
         * ajout du second segment entre le second et troisième point
         */
        monofin->addSegment(spk, npk1, fcpk);

        int p1 = np; // point 1
        int p2, npk2; // point 2

        /*
         * pour le reste des points sauf le dernier
         */
        for(int i = 2; i < pointsToKeepInOrder.size() - 1; i++){
            /*
             * recherche de l'index du point courant
             */
            p2 = pointsToKeepInOrder.value(i);
            /*
             * ajout du point courant
             */
            QPointF np2 = _scircle->getQPointRotate(p2, angle, scale) + offsetI;
            npk2 = monofin->addIntersectionPoint(np2.x(), offsetY - np2.y());
            /*
             * calcul de la position du point de controle entre le point courant et le point précédent
             */
            _scircle->tangent(p1, false, angle, scale).intersect(_scircle->tangent(p2, true, angle, scale), cp);
            /*
             * ajout du point de controle courant dans monofin
             */
            fcpk = monofin->addControlPoint(cp->x(), offsetY - cp->y());
             qDebug("point de controle : (%f, %f)", cp->x(), offsetY - cp->y());
            /*
             * ajout du segment courant dans monofin
             */
            monofin->addSegment(npk1, npk2, fcpk);

            /*
             * point courant devient point précédent et on recommence !!
             */
            p1 = p2;
            npk1 = npk2;
        }

        /*
         * ajout du dernier point dans monofin
         */
        npk2 = monofin->addIntersectionPoint(lastP.x(), offsetY - lastP.y());

        /*
         * calcul de la position du point de contrôle entre l'avant dernier et le dernier point
         */
        _scircle->tangent(p1, false, angle, scale).intersect(_scircle->specialTangent(lastP, p1, angle, scale), cp);

        /*
         * ajout du point de contrôle entre l'avant dernier et le dernier point dans monofin
         */
        fcpk = monofin->addControlPoint(cp->x(), offsetY - cp->y());

        /*
         * ajout du dernier segment
         */
        monofin->addSegment(npk1, npk2, fcpk);
        delete cp;
    }// fin second point sur talon
    else{
        /*
         * le second point n'est pas sur le talon
         */
        qDebug("passe pas par talon");
        /*
         * ajout du second point
         */
        QPointF np1 = _scircle->getQPointRotate(np, angle, scale) + offsetI;
        int npk1 = monofin->addIntersectionPoint(np1.x(), offsetY - np1.y());

        /*
         * calcul de la position du point de controle entre le premier et le second point
         */
        QPointF* cp = new QPointF();
        _scircle->specialTangent(firstP, np, angle, scale).intersect(_scircle->tangent(np, true, angle, scale), cp);
        /*
         * ajout point de controle entre le premier et le second point
         */
        int fcpk = monofin->addControlPoint(cp->x(), offsetY - cp->y());
        /*
         * ajout du premier segment entre le premier et le second point
         */
        monofin->addSegment(fpk, npk1, fcpk);

        int p1 = np; // point 1
        int p2, npk2; // point 2

        /*
         * pour le reste des points sauf le dernier
         */
        for(int i = 2; i < pointsToKeepInOrder.size() - 1; i++){
            /*
             * recherche de l'index du point courant
             */
            p2 = pointsToKeepInOrder.value(i);
            /*
             * ajout du point courant
             */
            QPointF np2 = _scircle->getQPointRotate(p2, angle, scale) + offsetI;
            npk2 = monofin->addIntersectionPoint(np2.x(), offsetY - np2.y());
            /*
             * calcul de la position du point de controle entre le point courant et le point précédent
             */
            _scircle->tangent(p1, false, angle, scale).intersect(_scircle->tangent(p2, true, angle, scale), cp);
            qDebug("point de controle : (%f, %f)", cp->x(), offsetY - cp->y());
            /*
             * ajout du point de controle courant dans monofin
             */
            fcpk = monofin->addControlPoint(cp->x(), offsetY - cp->y());
            /*
             * ajout du segment courant dans monofin
             */
            monofin->addSegment(npk1, npk2, fcpk);

            /*
             * point courant devient point précédent et on recommence !!
             */
            p1 = p2;
            npk1 = npk2;
        }

        /*
         * ajout du dernier point dans monofin
         */
        npk2 = monofin->addIntersectionPoint(lastP.x(), offsetY - lastP.y());

        /*
         * calcul de la position du point de contrôle entre l'avant dernier et le dernier point
         */
        _scircle->tangent(p1, false, angle, scale).intersect(_scircle->specialTangent(lastP, p1, angle, scale), cp);

        /*
         * ajout du point de contrôle entre l'avant dernier et le dernier point dans monofin
         */
        fcpk = monofin->addControlPoint(cp->x(), offsetY - cp->y());

        /*
         * ajout du dernier segment
         */
        monofin->addSegment(npk1, npk2, fcpk);
        delete cp;
    }// fin second point pas sur talon
    monofin->saveProject("MaMonopalme", "test");
    monofin->stopHistory(Data::MonofinSurface);
    return true;
}
