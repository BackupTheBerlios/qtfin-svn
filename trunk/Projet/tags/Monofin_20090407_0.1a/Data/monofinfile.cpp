#include "monofinfile.h"
using namespace Data;

MonofinFile::MonofinFile()
{
}

void MonofinFile::beginModification(Modification)
{
}

void MonofinFile::endModification()
{
}

void MonofinFile::addLayer(int rank, float lengthRatio)
{
}

void MonofinFile::setLayerLengthRatio(int rank, float lengthRatio)
{
}

void MonofinFile::removeLayer(int rank)
{
}

void MonofinFile::setLayerConfigRho(int rank, float rho)
{
}

void MonofinFile::setLayerConfigYoung(int rank, float young)
{
}

void MonofinFile::setLayerConfigPoisson(int rank, float poisson)
{
}

int MonofinFile::addSegment(int intersectionPoint1Key, int intersectionPoint2Key, int controlPointKey)
{
    return 0;
}

int MonofinFile::addControlPoint(float x, float y)
{
    return 0;
}

int MonofinFile::addIntersectionPoint(float x, float y)
{
    return 0;
}

int MonofinFile::subdivideSegment(int segmentKey, int intersectionPointKey)
{
    return 0;
}

int MonofinFile::removeControlPoint(int controlPointKey)
{
    return 0;
}

int MonofinFile::removeIntersectionPoint(int intersectionPointKey)
{
    return 0;
}

void MonofinFile::setIntersectionPoint(int intersectionPointKey, float x, float y)
{
}

void MonofinFile::setControlPoint(int controlPointKey, float x, float y)
{
}

void MonofinFile::undo(Modification)
{
}

void MonofinFile::redo(Modification)
{
}

const LayerFile * MonofinFile::getLayer(int rank) const
{
    return 0;
}

int MonofinFile::getHowManyLayers() const
{
    return 0;
}

float MonofinFile::getLayerConfigRho(int rank) const
{
    return 0.0;
}

float MonofinFile::getLayerConfigYoung(int rank) const
{
    return 0.0;
}

float MonofinFile::getLayerConfigPoisson(int rank) const
{
    return 0.0;
}

void MonofinFile::getIntersectionPoint(int intersectionPointKey, float & x, float & y)
{
}

void MonofinFile::getControlPoint(int controlPointKey, float & x, float & y)
{
}

void MonofinFile::getSegment(int segmentKey, int& intersectionPoint1, int& intersectionPoint2, int& controlPoint)
{
}

