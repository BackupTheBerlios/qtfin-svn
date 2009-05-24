#include "profil.h"
#include "surface.h"
#include "layer.h"
#include "monofinfile.h"
#include <iostream>

namespace Data{

    MonofinFile::MonofinFile(ConfigFile * link) {
        _monofinProfil = new Profil(link);
        _monofinSurface = new Surface();
    }

    MonofinFile::~MonofinFile(){
        delete _monofinProfil;
        delete _monofinSurface;
    }

    /***********
     * Setters *
     ***********/

    /**********
     * Layers *
     **********/

    /**
     * insert a layer at the given rank (displace if necessary other layers), if the rank is outside the current number of layer it will be placed at the last rank
     *@param rank an integer, where we want to insert the new Layer, a negative rank will insert the layer at the begining, a bigger rank than that exist actually will place the layer at the end
     *@param length a float, the  (between 0 and 1) between the length of the layer and the length of the fin
     *@param height a float, the  (between 0 and 1) between the height if the layer and the height of the fin
     */
    void MonofinFile::addLayer(int rank, float length, float height){
        _monofinProfil->addLayer(rank,new Layer(length,height));
    }

    /**
     * a setter on the length  of the layer at the given rank
     *@param rank an integer, the rank of the layer to modify, if it's an incorrect rank, nothing will be done
     *@param length a float, the new length
     */
    void MonofinFile::setLayerLength(int rank, float length){
        _monofinProfil->setLayerLength(rank, length);
    }

    /**
     * a setter on the heigth  of the layer at the given rank
     *@param height a float, the new heigtht
     */
    void MonofinFile::setLayerHeight(int rank, float height){
        _monofinProfil->setLayerHeight(rank, height);
    }

    /**
     * destroy the layer at the given rank, if no there are no layer at this rank, nothing will be done
     *@param rank an integer, the rank of the layer we want to remove
     */
    void MonofinFile::removeLayer(int rank){
        _monofinProfil->remLayer(rank);
    }

    /***********
     * Surface *
     ***********/

    /**
     * add a segment to the segment table
     *@param intersectionPointKey1 an integer, the key of the point representing the first edge of the segment, must be a valid key, otherwise the segment will not be created
     *@param intersectionPointKey2 an integer, the key of the point representing the second edge of the segment, must be a valid key, otherwise the segment will not be created
     *@param controlPointKey an integer, the, must be a valid key or MONOFIN_SURFACE_NO_CONTROL_POINT, otherwise the segment will not be created
     *@return an integer, the key of the created segment or MONOFIN_SURFACE_NOT_CREATED_SEGMENT if an error occured.
     */
    int MonofinFile::addSegment(int intersectionPointKey1, int intersectionPointKey2, int controlPointKey){
        return _monofinSurface->addSegment(intersectionPointKey1,intersectionPointKey2,controlPointKey);
    }

    /**
     * add a point to the control point table
     *@param x a float, x-axis coordinate
     *@param y a float, y-axis coordinate
     *@return an integer, the key of the created control point, or  MONOFIN_SURFACE_NOT_CREATED_POINT
     */
    int MonofinFile::addControlPoint(float x, float y){
        return _monofinSurface->addControlPoint(x,y);
    }

    /**
     * add a point to the intersection point table
     *@param x a float, x-axis coordinate
     *@param y a float, y-axis coordinate
     *@return an integer, the key of the created intersection point, or  MONOFIN_SURFACE_NOT_CREATED_POINT
     */
    int MonofinFile::addIntersectionPoint(float x, float y){
        return _monofinSurface->addIntersectionPoint(x,y);
    }

    /**
     * subdivide a segment by the right
     *@param segmentKey an integer, the segment to subdivide, must be a valid segment key otherwise, the function will have no effect
     *@param intersectionPointKey, the intersection point that will subdivide the original segment into two segments, must be a valid key otherwise the function will have no effect
     *@return an integer, the key of the created segment or MONOFIN_SURFACE_NOT_CREATED_SEGMENT if a key is not valid or an error occured
     */
    int MonofinFile::subdivideSegment(int segmentKey, int intersectionPointKey){
        return _monofinSurface->subdivideSegment(segmentKey,intersectionPointKey);
    }

    /**
     * remove the control point which key is given
     *@param controlPointKey an integer, the key of the control point to remove, must be a valid key otherwise nothing will be done
     */
    void MonofinFile::removeControlPoint(int controlPointKey){
        _monofinSurface->removeControlPoint(controlPointKey);
    }

    /**
     * remove the intersection point which key is given and merge the two segments using this point
     *@param intersectionPointKey an integer, the key of the intersection point to remove, must be a valid key otherwise nothing will be done
     *@return an integer, the key of the erased segement, or MONOFIN_SURFACE_NO_SEGMENT_ERASED if the key weren't valid or an error occured
     */
    int MonofinFile::removeIntersectionPoint(int intersectionPointKey){
        return _monofinSurface->removeIntersectionPoint(intersectionPointKey);
    }

    /**
     * set new coordinates for the intersection point which key is given
     *@param intersectionPointKey an integer, the key of the intersection point to modify
     *@param x a float, the new x-axis coordinate of the point
     *@param y a float, the new y-axis coordinate of the point
     */
    void MonofinFile::setIntersectionPoint(int intersectionPointKey, float x, float y){
        _monofinSurface->setIntersectionPoint(intersectionPointKey,x,y);
    }

    /**
     * set new coordinates for the control point which key is given
     *@param intersectionPointKey an integer, the key of the control point to modify
     *@param x a float, the new x-axis coordinate of the point
     *@param y a float, the new y-axis coordinate of the point
     */
    void MonofinFile::setControlPoint(int controlPointKey, float x, float y){
        _monofinSurface->setControlPoint(controlPointKey,x,y);
    }

    /**
     * set an existing control point to an existing segment, if keys are incorrect nothing will be done
     *@param segmentKey an integer, the key of the segment we want to add a control point
     *@param controlPointKey an integer, the key of the control point to add
     */
    void MonofinFile::addControlPointToSegment(int segmentKey, int controlPointKey){
        _monofinSurface->addControlPointToSegment(segmentKey,controlPointKey);
    }

    /**
     * clear all the surface's structure(delete everything), undo/redo works on it
     */
    void MonofinFile::clearSurface(){
        _monofinSurface->clearSurface();
    }

    /***********
     * Getters *
     ***********/

    /**********
     * Layers *
     **********/

    /**
     * a getter on the length  of the ranked layer
     *@param rank an integer, the rank of the layer we want the length
     *@return a float, the length  of the layer, MONOFIN_PROFIL_BAD_RANK if rank don't exist
     */
    float MonofinFile::getLayerLength(int rank){
        return _monofinProfil->getLayerLength(rank);
    }

    /**
     * a getter on the height  of the ranked layer
     *@param rank an integer, the rank of the layer we want the height
     *@return a float, the height  of the layer, MONOFIN_PROFIL_BAD_RANK if rank don't exist
     */
    float MonofinFile::getLayerHeight(int rank){
        return _monofinProfil->getLayerHeight(rank);
    }

    /**
     * a getter on the number of layer we have
     *@return an integer, the number of layer created
     */
    int MonofinFile::getHowManyLayers(){
        return _monofinProfil->getHowManyLayers();
    }

    /***********
     * Surface *
     ***********/


    /**
     * get the coordinates of the intersection point which key is given
     *@param intersectionPointKey an integer, the key of the point we want the coordinates
     *@param x a float, return the x-axis coordinate
     *@param y a float, return the y-axis coordinate
     */
    void MonofinFile::getIntersectionPoint(int intersectionPointKey, float &x, float &y){
        _monofinSurface->getIntersectionPoint(intersectionPointKey,x,y);
    }

    /**
     * get the coordinates of the control point which key is given
     *@param controlPointKey an integer, the key of the point we want the coordinates
     *@param x a float, return the x-axis coordinate
     *@param y a float, return the y-axis coordinate
     */
    void MonofinFile::getControlPoint(int controlPointKey, float &x, float &y){
        _monofinSurface->getControlPoint(controlPointKey,x,y);
    }

    /**
     * get all the informations over a segment
     *@param segmentKey an integer, the key of the segment we want informations over
     *@param intersectionPointKey1 an integer, return the key of the first intersection point of the segment
     *@param intersectionPointKey2 an integer, return the key of the second intersection point of the segment
     *@param controlPointKey an integer, return the key of the control point (bezier curve), may return MONOFIN_SURFACE_NO_SEGMENT_ERASED if the point don't exist
     */
    void MonofinFile::getSegment(int segmentKey, int &intersectionPointKey1, int &intersectionPointKey2, int &controlPointKey){
        _monofinSurface->getSegment(segmentKey,intersectionPointKey1,intersectionPointKey2,controlPointKey);
    }

    /**
     * a function called at the initialisation of the structure to linkthe profil and the configfile (which also manage layers)
     *@param toLink a pointer to ConfigFile, the address of the current ConfigFile
     */
    /*void MonofinFile::linkProfilWithConfigFile(ConfigFile * toLink){
        _monofinProfil->link(toLink);
    }*/

    void MonofinFile::startHistory(Modification t){
        _monofinSurface->startHistory(t);
        _monofinProfil->startHistory(t);
    }

    QList<HistoryHolder<Modification> *> MonofinFile::retrieveHistory(Modification t){
        return _monofinSurface->retrieveHistory(t)+_monofinProfil->retrieveHistory(t);;
    }

    void MonofinFile::undo(QList<HistoryHolder<Modification> *> history){
        _monofinSurface->undo(history);
        _monofinProfil->undo(history);
    }

    QList<int> MonofinFile::getAllSegmentKeys(){
        return _monofinSurface->getAllSegmentKeys();
    }

    QList<int> MonofinFile::getAllIntersectionPointKeys(){
        return _monofinSurface->getAllIntersectionPointKeys();
    }

    QList<int> MonofinFile::getAllControlPointKeys(){
        return _monofinSurface->getAllControlPointKeys();
    }

    /**
      * return all IntersectionPoint only linked to one segment
      *@return QList<int>, key's list
      */
    QList<int> MonofinFile::getExtremityPoint(){
        return _monofinSurface->getExtremityPoint();
    }

    QList<int> MonofinFile::getSegmentKeysLinkedToPoint(int pointKey){
        return _monofinSurface->getSegmentKeysLinkedToPoint(pointKey);
    }

    void MonofinFile::accept(SaveVisitor *sv){
        _monofinProfil->accept(sv);
        _monofinSurface->accept(sv);
    }

    void MonofinFile::accept(LoadVisitor *lv){
        _monofinProfil->accept(lv);
        _monofinSurface->accept(lv);
    }

    float MonofinFile::getLayerMaxLength(){
        return _monofinProfil->getMaxLength();
    }

    float MonofinFile::getLayerMaxHeight(){
        return _monofinProfil->getMaxHeight();
    }

}// namespace Data

