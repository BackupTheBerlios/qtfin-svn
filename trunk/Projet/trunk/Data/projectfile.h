#ifndef PROJECTFILE_H
#define PROJECTFILE_H

#include "DataConstants.h"
#include "historyholder.h"
#include <QStack>

namespace Data{

    class MonofinFile;
    class ConfigFile;
    class Action;

    class ProjectFile : public HistoryCareTaker<Modification>
    {
    private:
        MonofinFile *_monofinGeometry;
        ConfigFile *_monofinPhysicalProperties;

    public:
        ProjectFile();

        ~ProjectFile();

        /****************************
         * Modification des donnees *
         ****************************/

        //modifications possibles sur les strates
        void addLayer(int rank, float length, float height);

        void setLayerLength(int rank, float length);
        void setLayerHeight(int rank, float height);

        void removeLayer(int rank);

        //modifications possibles sur les strates config
        void setLayerConfigRho(int rank, float rho);
        void setLayerConfigYoung(int rank, float young);
        void setLayerConfigPoisson(int rank, float poisson);

        //modifications possibles sur la surface
        int addSegment(int intersectionPoint1Key, int intersectionPoint2Key, int controlPointKey);
        int addControlPoint(float x, float y);
        int addIntersectionPoint(float x, float y);

        int subdivideSegment(int segmentKey, int intersectionPointKey);

        void removeControlPoint(int controlPointKey);
        int removeIntersectionPoint(int intersectionPointKey);

        void setIntersectionPoint(int intersectionPointKey, float x, float y);
        void setControlPoint(int controlPointKey, float x, float y);
        /**
         * set an existing control point to an existing segment, if keys are incorrect nothing will be done
         *@param segmentKey an integer, the key of the segment we want to add a control point
         *@param controlPointKey an integer, the key of the control point to add
         */
        void addControlPointToSegment(int segmentKey, int controlPointKey);

        /**
         * clear all the surface's structure(delete everything), undo/redo works on it
         */
        void clearSurface();

        /****************************
         * Consultation des donnes *
         ****************************/

        //consultations possible sur les strates
        float getLayerLength(int rank) const;
        float getLayerHeight(int rank) const;

        int getHowManyLayers() const;

        //consultations possible sur les strates config
        float getLayerConfigRho(int rank) const;
        float getLayerConfigYoung(int rank) const;
        float getLayerConfigPoisson(int rank) const;

        //consultations possible sur la surface
        void getIntersectionPoint(int intersectionPointKey, float &x, float &y);
        void getControlPoint(int controlPointKey, float &x, float &y);
        void getSegment(int segmentKey, int &intersectionPointKey1, int &intersectionPointKey2, int &controlPointKey);
        QList<int> getAllSegmentKeys();
        QList<int> getAllIntersectionPointKeys();
        QList<int> getAllControlPointKeys();
        /**
          * return all IntersectionPoint only linked to one segment
          *@return QList<int>, key's list
          */
        QList<int> getExtremityPoint();

        QList<int> getSegmentKeysLinkedToPoint(int pointKey);
    };

} // namespace Data

#endif // PROJECTFILE_H
