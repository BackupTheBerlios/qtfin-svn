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

#define PI 3.14159265358979323846

using namespace Data;

AlgoSnake::AlgoSnake(SCircle* scircle):
        AbstractAlgoEdgesExtraction(), _scircle(scircle), _image(NULL){
    qreal nbPoints = _scircle->radius();
    qDebug("nombre points snake : %f", nbPoints);
    _scircle->addSPoint((int)nbPoints);
}

void AlgoSnake::setImage(QImage* image){
        _image = image;
}

bool AlgoSnake::edgesDetection(qreal offsetX, qreal offsetY){
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
                        int t = qAbs(sp->grayValue() - gray);
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
            if((point.y() < 0 && point.x() > offsetX /10) || (point.x() > offsetX && point.y() < offsetY / 2))
                return false;
        }
    }
    return true;
}

bool AlgoSnake::edgesExtraction(ProjectFile* monofin, qreal offsetX, qreal offsetY){
    /**
     * Initialisation des variables utiles
     */
    QPointF offsetI = _scircle->pixmapItem()->offset();
    qreal scale = _scircle->pixmapItem()->getScale();
    qreal angle = _scircle->pixmapItem()->rotationAngle();
    int firstPoint = -1;
    int secondPoint = -2;
    int lastPoint = -3;
    QPointF firstP;
    QPointF secondP;
    QPointF lastP;

    for(int i = 0; i < _scircle->getSPointNb(); i++){
        QPointF point = _scircle->getQPointRotate(i, angle, scale);
        qDebug("point : (%f, %f)" , point.x(), point.y());
    }

    /**
     * nouvel algo
     */
    /**
     * initialisation liste des points de controle
     */
    QList<int> controlPointsToKeep;

    /**
     * détection du premier point
     */
    QPointF* fp = new QPointF();
    int np = _scircle->firstPoint(angle, offsetX, offsetY, scale, fp);

    /**
     * si il n'y a pas de premier point, retourne faux
     */
    if(np == -1)
        return false;

    /**
     * On regarde si le premier point est sur l'axe de symétrie ou sur le talon
     * Ajout du premier point, du second point si nécessaire et du prochain point dans la liste
     */

    if(fp->y() == offsetY){
        firstP = *fp;
        controlPointsToKeep.push_back(firstPoint);
    }else{
        firstP = QPointF(offsetX, offsetY);
        secondP = *fp;
        controlPointsToKeep.push_back(firstPoint);
        controlPointsToKeep.push_back(secondPoint);
    }

    /**
     * On parcours la liste des points jusqu'à repasser par l'axe de symétrie ou fait le tour complet
     * On essaye de détecter la liste des points de controle à garder
     * Si on ne repasse pas par l'axe de symétrie, ça marche pas !!
     */
    bool finish = false; // dit si on est repassé par l'axe de symétrie
    /**
     * création du path
     */
    QList<QPointF> path;
    /**
     * on met le premier point ou le deuxième point dans le path
     */
    path.push_back(*fp);
    delete fp;

    QPointF* nothingP = new QPointF();
    controlPointsToKeep.push_back(np);
    for(int i = np; i < _scircle->getSPointNb() && !finish; i++){
        qDebug("test1");
        QPointF newPoint = _scircle->getQPointRotate(i, angle, scale) + offsetI;

        if(newPoint.y() >= offsetY){
            if(newPoint.x() <= offsetX)
                return false;
            QPointF* lp = new QPointF();
            QLineF(0, offsetY, offsetX * 10, offsetY).intersect(
                    QLineF(_scircle->getQPointRotate(i - 1, angle, scale) + offsetI, newPoint),
                    lp);
            lastP = *lp;
            controlPointsToKeep.removeLast();
            controlPointsToKeep.push_back(lastPoint);
            finish = true;
            delete lp;
        }
        else
        {
            QLineF newSegment(path.first(), newPoint);
            bool isCP = false;
            for(int j = 1; j < path.size() && !isCP; j++){
                QLineF lz1(path.value(j) + QPointF(-1.5, 0), path.value(j) + QPointF(1.5, 0));
                QLineF lz2(path.value(j) + QPointF(0, -1.5), path.value(j) + QPointF(0, 1.5));
                int intersect1 = newSegment.intersect(lz1, nothingP);
                int intersect2 = newSegment.intersect(lz2, nothingP);
                if(intersect1 != QLineF::BoundedIntersection && intersect2 != QLineF::BoundedIntersection){
                    isCP = true;
                    QPointF temp = path.last();
                    path = QList<QPointF>();
                    path.push_back(temp);
                    path.push_back(newPoint);
                    controlPointsToKeep.push_back(i);
                    qDebug("test1.5");
                    qDebug("taille de la liste : %d", controlPointsToKeep.size());
                }
            }
            if(!isCP){
                path.push_back(newPoint);
                controlPointsToKeep.removeLast();
                controlPointsToKeep.push_back(i);
                qDebug("test1.75");
                qDebug("taille de la liste : %d", controlPointsToKeep.size());
            }
        }
    }


    for(int i = 0; i < np && !finish; i++){
        qDebug("test2");
        QPointF newPoint = _scircle->getQPointRotate(i, angle, scale) + offsetI;

        if(newPoint.y() >= offsetY){
            if(newPoint.x() <= offsetX)
                return false;
            QPointF* lp = new QPointF();
            QLineF(0, offsetY, offsetX * 10, offsetY).intersect(
                    QLineF(_scircle->getQPointRotate(i - 1, angle, scale) + offsetI, newPoint),
                    lp);
            lastP = *lp;
            controlPointsToKeep.push_back(lastPoint);
            controlPointsToKeep.removeLast();
            finish = true;
            delete lp;
        }
        else
        {
            QLineF newSegment(path.first(), newPoint);
            bool isCP = false;
            for(int j = 1; j < path.size() && !isCP; j++){
                QLineF lz1(path.value(j) + QPointF(-1.5, 0), path.value(j) + QPointF(1.5, 0));
                QLineF lz2(path.value(j) + QPointF(0, -1.5), path.value(j) + QPointF(0, 1.5));
                int intersect1 = newSegment.intersect(lz1, nothingP);
                int intersect2 = newSegment.intersect(lz2, nothingP);
                if(intersect1 != QLineF::BoundedIntersection && intersect2 != QLineF::BoundedIntersection){
                    isCP = true;
                    QPointF temp = path.last();
                    path = QList<QPointF>();
                    path.push_back(temp);
                    path.push_back(newPoint);
                    controlPointsToKeep.push_back(i);
                    qDebug("test2.5");
                }
            }
            if(!isCP){
                path.push_back(newPoint);
                controlPointsToKeep.removeLast();
                controlPointsToKeep.push_back(i);
                qDebug("test2.75");
            }
        }
    }

    if(!finish){
        return false;
    }
    qDebug("test3");

    /**
     * Ajout des points dans le project File avec les points de contrôle
     */

    /**
     * Ajout du premier point dans monofin
     */
     monofin->startHistory(Data::MonofinSurface);
     int fpk = monofin->addIntersectionPoint(firstP.x(), offsetY - firstP.y());
    /**
     * Vérifie si le second point est sur le talon
     */
     qDebug("taille de la liste : %d", controlPointsToKeep.size());
     if(controlPointsToKeep.value(1) == secondPoint){
         qDebug("test4");
        /**
         * le second point est sur le talon
         */
        /**
         * ajout du second point dans monofin
         */
        int spk = monofin->addIntersectionPoint(secondP.x(), offsetY - secondP.y());
        /**
         * ajout du point de controle entre le premier et le second point
         */
        int fcpk = monofin->addControlPoint(firstP.x(), offsetY - (secondP.y() + (firstP.y() - secondP.y()) / 2) );
        /**
         * ajout du premier segment entre le premier et le second point
         */
        monofin->addSegment(fpk, spk, fcpk);

        /**
         * ajout du troisième point
         */
        QPointF np1 = (9 * (_scircle->getQPointRotate(controlPointsToKeep.value(2), angle, scale) + offsetI)
                      + secondP) / 10; // np1 : next point 1
        int npk1 = monofin->addIntersectionPoint(np1.x(), offsetY - np1.y());// npk1 : next point key 1

        /**
         * calcul de la position du point de controle entre le second et le troisième point
         */
        QPointF cp = (np1 + secondP) / 2;// control point
        /**
         * ajout point de controle entre le second et le troisième point
         */
        fcpk = monofin->addControlPoint(cp.x(), offsetY - cp.y());

        /**
         * ajout du second segment entre le second et troisième point
         */
        monofin->addSegment(spk, npk1, fcpk);

        int p1 = controlPointsToKeep.value(2); // point 1
        int p2, npk2; // point 2

        /**
         * pour le reste des points sauf le dernier
         */
        for(int i = 3; i < controlPointsToKeep.size() - 1; i++){
            qDebug("test4");
            /**
             * recherche de l'index du point de controle courant
             */
            p2 = controlPointsToKeep.value(i);
            /**
             * ajout du point courant
             */
            QPointF np2 = (_scircle->getQPointRotate(p2, angle, scale) +
                           _scircle->getQPointRotate(p1, angle, scale)) / 2 + offsetI;
            npk2 = monofin->addIntersectionPoint(np2.x(), offsetY - np2.y());
            /**
             * calcul de la position du point de controle entre le point courant et le point précédent
             */
            cp = _scircle->getQPointRotate(p1, angle, scale) + offsetI;
            /**
             * ajout du point de controle courant dans monofin
             */
            fcpk = monofin->addControlPoint(cp.x(), offsetY - cp.y());
            /**
             * ajout du segment courant dans monofin
             */
            monofin->addSegment(npk1, npk2, fcpk);

            /**
             * point courant devient point précédent et on recommence !!
             */
            p1 = p2;
            npk1 = npk2;
        }

        /**
         * ajout de l'avant dernier point dans monofin
         */
        controlPointsToKeep.removeLast();
        QPointF npll = (9 * (_scircle->getQPointRotate(controlPointsToKeep.last(), angle, scale) + offsetI)
                      + lastP) / 10; // np1 : next point 1
        int npkll = monofin->addIntersectionPoint(npll.x(), offsetY - npll.y());
         /**
         * calcul de la position du point de controle
         */
        cp = _scircle->getQPointRotate(controlPointsToKeep.last(), angle, scale) + offsetI;
        /**
         * ajout point de controle
         */
        fcpk = monofin->addControlPoint(cp.x(), offsetY - cp.y());

        /**
         * ajout du second segment entre le second et troisième point
         */
        monofin->addSegment(npk1, npkll, fcpk);


        /**
         * ajout du dernier point dans monofin
         */
        npk2 = monofin->addIntersectionPoint(lastP.x(), offsetY - lastP.y());

        /**
         * calcul de la position du point de contrôle entre l'avant dernier et le dernier point
         */
        cp = (npll + lastP) / 2;

        /**
         * ajout du point de contrôle entre l'avant dernier et le dernier point dans monofin
         */
        fcpk = monofin->addControlPoint(cp.x(), offsetY - cp.y());

        /**
         * ajout du dernier segment
         */
        monofin->addSegment(npkll, npk2, fcpk);
    }// fin second point sur talon
    else{
        /**
         * le second point n'est pas sur le talon
         */
        /**
         * ajout du second point
         */
        QPointF np1 = (9 * (_scircle->getQPointRotate(controlPointsToKeep.value(2), angle, scale) + offsetI)
                      + firstP) / 10; // np1 : next point 1
        int npk1 = monofin->addIntersectionPoint(np1.x(), offsetY - np1.y());// npk1 : next point key 1

        /**
         * calcul de la position du point de controle entre le premier et le second point
         */
         QPointF cp = (np1 + firstP) / 2;// control point
        /**
         * ajout point de controle entre le premier et le second point
         */
        int fcpk = monofin->addControlPoint(cp.x(), offsetY - cp.y());
        /**
         * ajout du premier segment entre le premier et le second point
         */
        monofin->addSegment(fpk, npk1, fcpk);

        int p1 = controlPointsToKeep.value(1); // point 1
        int p2, npk2; // point 2

        /**
         * pour le reste des points sauf le dernier
         */
        for(int i = 2; i < controlPointsToKeep.size() - 1; i++){
            /**
             * recherche de l'index du point de controle courant
             */
            p2 = controlPointsToKeep.value(i);
            /**
             * ajout du point courant
             */
            QPointF np2 = (_scircle->getQPointRotate(p2, angle, scale) +
                           _scircle->getQPointRotate(p1, angle, scale)) / 2 + offsetI;
            npk2 = monofin->addIntersectionPoint(np2.x(), offsetY - np2.y());
            /**
             * calcul de la position du point de controle entre le point courant et le point précédent
             */
            cp = _scircle->getQPointRotate(p1, angle, scale) + offsetI;
            /**
             * ajout du point de controle courant dans monofin
             */
            fcpk = monofin->addControlPoint(cp.x(), offsetY - cp.y());
            /**
             * ajout du segment courant dans monofin
             */
            monofin->addSegment(npk1, npk2, fcpk);

            /**
             * point courant devient point précédent et on recommence !!
             */
            p1 = p2;
            npk1 = npk2;
        }

        /**
         * ajout de l'avant dernier point dans monofin
         */
        controlPointsToKeep.removeLast();
        QPointF npll = (9 * (_scircle->getQPointRotate(controlPointsToKeep.last(), angle, scale) + offsetI)
                      + lastP) / 10; // np1 : next point 1
        int npkll = monofin->addIntersectionPoint(npll.x(), offsetY - npll.y());
         /**
         * calcul de la position du point de controle
         */
        cp = _scircle->getQPointRotate(controlPointsToKeep.last(), angle, scale) + offsetI;
        /**
         * ajout point de controle
         */
        fcpk = monofin->addControlPoint(cp.x(), offsetY - cp.y());

        /**
         * ajout du second segment entre le second et troisième point
         */
        monofin->addSegment(npk1, npkll, fcpk);


        /**
         * ajout du dernier point dans monofin
         */
        npk2 = monofin->addIntersectionPoint(lastP.x(), offsetY - lastP.y());

        /**
         * calcul de la position du point de contrôle entre l'avant dernier et le dernier point
         */
        cp = (npll + lastP) / 2;

        /**
         * ajout du point de contrôle entre l'avant dernier et le dernier point dans monofin
         */
        fcpk = monofin->addControlPoint(cp.x(), offsetY - cp.y());

        /**
         * ajout du dernier segment
         */
        monofin->addSegment(npkll, npk2, fcpk);
    }
    monofin->stopHistory(Data::MonofinSurface);
    return true;
}
