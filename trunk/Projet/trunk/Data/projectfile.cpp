#include "configfile.h"
#include "monofinfile.h"
#include "projectfile.h"
#include "xmlsavevisitor.h"

namespace Data{

    ProjectFile::ProjectFile(){
        _monofinPhysicalProperties = new ConfigFile();
        _monofinGeometry = new MonofinFile(_monofinPhysicalProperties);
        addHistoryMaker(_monofinGeometry);
        addHistoryMaker(_monofinPhysicalProperties);
    }

    ProjectFile::~ProjectFile(){
        delete _monofinGeometry;
        delete _monofinPhysicalProperties;
    }

    void ProjectFile::addLayer(int rank, float length, float height){
        _monofinGeometry->addLayer(rank,length,height);
    }

    void ProjectFile::setLayerLength(int rank, float length){
        _monofinGeometry->setLayerLength(rank,length);
    }

    void ProjectFile::setLayerHeight(int rank, float height){
        _monofinGeometry->setLayerHeight(rank,height);
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

    float ProjectFile::getLayerLength(int rank) const{
        return _monofinGeometry->getLayerLength(rank);
    }

    float ProjectFile::getLayerHeight(int rank) const{
        return _monofinGeometry->getLayerHeight(rank);
    }

    int ProjectFile::getHowManyLayers() const{
        return _monofinGeometry->getHowManyLayers();
    }

    float ProjectFile::getLayerConfigRho(int rank) const{
        return _monofinPhysicalProperties->getRho(rank);
    }

    float ProjectFile::getLayerConfigYoung(int rank) const{
        return _monofinPhysicalProperties->getYoung(rank);
    }

    float ProjectFile::getLayerConfigPoisson(int rank) const{
        return _monofinPhysicalProperties->getPoisson(rank);
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

    /**
      * return all IntersectionPoint only linked to one segment
      *@return QList<int>, key's list
      */
    QList<int> ProjectFile::getExtremityPoint(){
        return _monofinGeometry->getExtremityPoint();
    }

    QList<int> ProjectFile::getSegmentKeysLinkedToPoint(int pointKey){
        return _monofinGeometry->getSegmentKeysLinkedToPoint(pointKey);
    }


    void ProjectFile::saveProject(QString path, QString projectName){
        XMLSaveVisitor save;
        _monofinGeometry->accept(&save);
        _monofinPhysicalProperties->accept(&save);
        save.save(path.toStdString(),projectName.toStdString());
    }

    QString loadProject(QString path){
        return (QString("None"));
    }

} // namespace Data
