#include "configfile.h"
#include "monofinfile.h"
#include "projectfile.h"

namespace Data{

    ProjectFile::ProjectFile(){
        _monofinPhysicalProperties = new ConfigFile();
        _monofinGeometry = new MonofinFile(_monofinPhysicalProperties);
        addHistoryMaker(_monofinGeometry);
        addHistoryMaker(_monofinPhysicalProperties);
    }

    void ProjectFile::addLayer(int rank, float lengthRatio, float heightRatio){
        _monofinGeometry->addLayer(rank,lengthRatio,heightRatio);
    }

    void ProjectFile::setLayerLengthRatio(int rank, float lengthRatio){
        _monofinGeometry->setLayerLengthRatio(rank,lengthRatio);
    }

    void ProjectFile::setLayerHeightRatio(int rank, float heightRatio){
        _monofinGeometry->setLayerHeightRatio(rank,heightRatio);
    }

    void ProjectFile::removeLayer(int rank){
        _monofinGeometry->removeLayer(rank);
    }

    void ProjectFile::setLayerConfigRho(int rank, float rho){
        _monofinPhysicalProperties->setRho(rank,rho);
    }

    void ProjectFile::setLayerConfigYoung(int rank, float young){
        _monofinPhysicalProperties->setYoung(rank,young);
    }

    void ProjectFile::setLayerConfigPoisson(int rank, float poisson){
        _monofinPhysicalProperties->setPoisson(rank,poisson);
    }

    int ProjectFile::addSegment(int intersectionPoint1Key, int intersectionPoint2Key, int controlPointKey){
        return _monofinGeometry->addSegment(intersectionPoint1Key,intersectionPoint2Key,controlPointKey);
    }

    int ProjectFile::addControlPoint(float x, float y){
        return _monofinGeometry->addControlPoint(x,y);
    }

    int ProjectFile::addIntersectionPoint(float x, float y){
        return _monofinGeometry->addIntersectionPoint(x,y);
    }

    int ProjectFile::subdivideSegment(int segmentKey, int intersectionPointKey){
        return _monofinGeometry->subdivideSegment(segmentKey,intersectionPointKey);
    }

    void ProjectFile::removeControlPoint(int controlPointKey) {
        _monofinGeometry->removeControlPoint(controlPointKey);
    }

    int ProjectFile::removeIntersectionPoint(int intersectionPointKey){
        return _monofinGeometry->removeIntersectionPoint(intersectionPointKey);
    }

    void ProjectFile::setIntersectionPoint(int intersectionPointKey, float x, float y) {
        _monofinGeometry->setIntersectionPoint(intersectionPointKey,x,y);
    }

    void ProjectFile::setControlPoint(int controlPointKey, float x, float y) {
        _monofinGeometry->setControlPoint(controlPointKey,x,y);
    }

    /**
     * set an existing control point to an existing segment, if keys are incorrect nothing will be done
     *@param segmentKey an integer, the key of the segment we want to add a control point
     *@param controlPointKey an integer, the key of the control point to add
     */
    void ProjectFile::addControlPointToSegment(int segmentKey, int controlPointKey){
        _monofinGeometry->addControlPointToSegment(segmentKey,controlPointKey);
    }

    /**
     * clear all the surface's structure(delete everything), undo/redo works on it
     */
    void ProjectFile::clearSurface(){
        _monofinGeometry->clearSurface();
    }

    float ProjectFile::getLayerLengthRatio(int rank) const{
        return 0;
    }

    float ProjectFile::getLayerHeightRatio(int rank) const{
        return 0;
    }

    int ProjectFile::getHowManyLayers() const{
        return 0;
    }

    float ProjectFile::getLayerConfigRho(int rank) const{
        return 0;
    }

    float ProjectFile::getLayerConfigYoung(int rank) const{
        return 0;
    }

    float ProjectFile::getLayerConfigPoisson(int rank) const{
        return 0;
    }

    void ProjectFile::getIntersectionPoint(int intersectionPointKey, float &x, float &y){
        _monofinGeometry->getIntersectionPoint(intersectionPointKey,x,y);
    }

    void ProjectFile::getControlPoint(int controlPointKey, float &x, float &y){
        _monofinGeometry->getControlPoint(controlPointKey,x,y);
    }

    void ProjectFile::getSegment(int segmentKey, int &intersectionPointKey1, int &intersectionPointKey2, int &controlPointKey){
        _monofinGeometry->getSegment(segmentKey,intersectionPointKey1,intersectionPointKey2,controlPointKey);
    }

    QList<int> ProjectFile::getAllSegmentKeys(){
        return _monofinGeometry->getAllSegmentKeys();
    }

    QList<int> ProjectFile::getAllIntersectionPointKeys(){
        return _monofinGeometry->getAllIntersectionPointKeys();
    }

    QList<int> ProjectFile::getAllControlPointKeys(){
        return _monofinGeometry->getAllControlPointKeys();
    }

} // namespace Data
