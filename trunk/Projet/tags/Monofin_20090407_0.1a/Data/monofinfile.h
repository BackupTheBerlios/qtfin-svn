#ifndef MONOFINFILE_H
#define MONOFINFILE_H

namespace Data{
    class LayerFile;

    enum Modification {MonofinSurface,MonofinLayer,MonofinLayerConfig};
    const int MONOFIN_SURFACE_NO_CONTROL_POINT = 42; //extrait de surface.h

    class MonofinFile
    {
    public:
        MonofinFile();

        /****************************
        * Modification des donnes *
         ****************************/

        //pour commencer les modifications
        void beginModification(Modification);

        void endModification();

        //modifications possibles sur les strates
        void addLayer(int rank, float lengthRatio);

        void setLayerLengthRatio(int rank, float lengthRatio);

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

        int removeControlPoint(int controlPointKey);
        int removeIntersectionPoint(int intersectionPointKey);

        void setIntersectionPoint(int intersectionPointKey, float x, float y);
        void setControlPoint(int controlPointKey, float x, float y);

        /****************************
         *         Undo Redo        *
         ****************************/

        void undo(Modification);

        void redo(Modification);


        /****************************
         * Consultation des donnes *
         ****************************/

        //consultations possible sur les strates
        const LayerFile * getLayer(int rank) const;

        int getHowManyLayers() const;

        //consultations possible sur les strates config
        float getLayerConfigRho(int rank) const;
        float getLayerConfigYoung(int rank) const;
        float getLayerConfigPoisson(int rank) const;

        //consultations possible sur la surface
        void getIntersectionPoint(int intersectionPointKey, float & x, float & y);
        void getControlPoint(int controlPointKey, float & x, float & y);
        void getSegment(int segmentKey, int& intersectionPoint1, int& intersectionPoint2, int& controlPoint);


    }; // class MonofinFile

} // namespace Data

#endif // MONOFINFILE_H
