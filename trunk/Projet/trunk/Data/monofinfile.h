#ifndef MONOFINFILE_H
#define MONOFINFILE_H

#include "DataConstants.h"
#include "historyholder.h"
#include "SaveVisitor.h"
#include "LoadVisitor.h"

namespace Data{

    class Surface;
    class ProjectFile;
    class Profil;
    class ConfigFile;

    class MonofinFile : public HistoryMaker<Modification>
    {
    private:
        Profil *_monofinProfil;
        Surface *_monofinSurface;
    public:
        MonofinFile(ConfigFile * link);

        virtual ~MonofinFile();

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
        void addLayer(int rank, float length, float height);

        /**
         * a setter on the length  of the layer at the given rank
         *@param rank an integer, the rank of the layer to modify, if it's an incorrect rank, nothing will be done
         *@param length a float, the new length
         */
        void setLayerLength(int rank, float length);

        /**
         * a setter on the heigth  of the layer at the given rank
         *@param height a float, the new heigtht
         */
        void setLayerHeight(int rank, float height);

        /**
         * destroy the layer at the given rank, if no there are no layer at this rank, nothing will be done
         *@param rank an integer, the rank of the layer we want to remove
         */
        void removeLayer(int rank);

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
        int addSegment(int intersectionPointKey1, int intersectionPointKey2, int controlPointKey);

        /**
         * add a point to the control point table
         *@param x a float, x-axis coordinate
         *@param y a float, y-axis coordinate
         *@return an integer, the key of the created control point, or  MONOFIN_SURFACE_NOT_CREATED_POINT
         */
        int addControlPoint(float x, float y);

        /**
         * add a point to the intersection point table
         *@param x a float, x-axis coordinate
         *@param y a float, y-axis coordinate
         *@return an integer, the key of the created intersection point, or  MONOFIN_SURFACE_NOT_CREATED_POINT
         */
        int addIntersectionPoint(float x, float y);

        /**
         * subdivide a segment by the right
         *@param segmentKey an integer, the segment to subdivide, must be a valid segment key otherwise, the function will have no effect
         *@param intersectionPointKey, the intersection point that will subdivide the original segment into two segments, must be a valid key otherwise the function will have no effect
         *@return an integer, the key of the created segment or MONOFIN_SURFACE_NOT_CREATED_SEGMENT if a key is not valid or an error occured
         */
        int subdivideSegment(int segmentKey, int intersectionPointKey);

        /**
         * remove the control point which key is given
         *@param controlPointKey an integer, the key of the control point to remove, must be a valid key otherwise nothing will be done
         */
        void removeControlPoint(int controlPointKey);

        /**
         * remove the intersection point which key is given and merge the two segments using this point
         *@param intersectionPointKey an integer, the key of the intersection point to remove, must be a valid key otherwise nothing will be done
         *@return an integer, the key of the erased segement, or MONOFIN_SURFACE_NO_SEGMENT_ERASED if the key weren't valid or an error occured
         */
        int removeIntersectionPoint(int intersectionPointKey);

        /**
         * set new coordinates for the intersection point which key is given
         *@param intersectionPointKey an integer, the key of the intersection point to modify
         *@param x a float, the new x-axis coordinate of the point
         *@param y a float, the new y-axis coordinate of the point
         */
        void setIntersectionPoint(int intersectionPointKey, float x, float y);

        /**
         * set new coordinates for the control point which key is given
         *@param intersectionPointKey an integer, the key of the control point to modify
         *@param x a float, the new x-axis coordinate of the point
         *@param y a float, the new y-axis coordinate of the point
         */
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
        float getLayerLength(int rank);

        /**
         * a getter on the height  of the ranked layer
         *@param rank an integer, the rank of the layer we want the height
         *@return a float, the height  of the layer, MONOFIN_PROFIL_BAD_RANK if rank don't exist
         */
        float getLayerHeight(int rank);

        /**
         * a getter on the number of layer we have
         *@return an integer, the number of layer created
         */
        int getHowManyLayers();

        /***********
         * Surface *
         ***********/


        /**
         * get the coordinates of the intersection point which key is given
         *@param intersectionPointKey an integer, the key of the point we want the coordinates
         *@param x a float, return the x-axis coordinate
         *@param y a float, return the y-axis coordinate
         */
        void getIntersectionPoint(int intersectionPointKey, float &x, float &y);

        /**
         * get the coordinates of the control point which key is given
         *@param controlPointKey an integer, the key of the point we want the coordinates
         *@param x a float, return the x-axis coordinate
         *@param y a float, return the y-axis coordinate
         */
        void getControlPoint(int controlPointKey, float &x, float &y);

        /**
         * get all the informations over a segment
         *@param segmentKey an integer, the key of the segment we want informations over
         *@param intersectionPointKey1 an integer, return the key of the first intersection point of the segment
         *@param intersectionPointKey2 an integer, return the key of the second intersection point of the segment
         *@param controlPointKey an integer, return the key of the control point (bezier curve), may return MONOFIN_SURFACE_NO_SEGMENT_ERASED if the point don't exist
         */
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

        void startHistory(Modification t);

        QList<HistoryHolder<Modification> *> retrieveHistory(Modification t);

        void undo(QList<HistoryHolder<Modification> *> history);

        void accept(SaveVisitor * sv);

        void accept(LoadVisitor *lv);

    private:
        /**
         * a function called at the initialisation of the structure to linkthe profil and the configfile (which also manage layers)
         *@param toLink a pointer to ConfigFile, the address of the current ConfigFile
         */
        //void linkProfilWithConfigFile(ConfigFile * toLink);

    }; // class MonofinFile

} // namespace Data

#endif // MONOFINFILE_H
