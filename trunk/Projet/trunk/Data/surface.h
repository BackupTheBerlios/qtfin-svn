#ifndef SURFACE_H
#define SURFACE_H

#include <QPointf>
#include <QHash>
#include <QList>
#include "segment.h"
#include "historyholder.h"
#include "DataConstants.h"

namespace Data{

    class Surface : public HistoryMaker<Modification>
    {
    private:
        struct Point{
            QPointF coordinates;
            int countUsage;
            QList<int> whereUsed;
        };

        QHash<int, Segment*> _segmentTable;
        QHash<int, Point*> _intersectionPointTable;
        QHash<int, Point*> _controlPointTable;

        int _segmentKey, _intersectionKey, _controlKey;
    public:
        Surface(): _segmentKey(0), _intersectionKey(0), _controlKey(0) {}

        virtual ~Surface();

        /**
         * Surface Modification
         */

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
         * getters on surface
         */

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

        /**
         * return all segment's keys
         *@return QList<int>, key's list
         */
        QList<int> getAllSegmentKeys();

        /**
         * return all intersection point's keys
         *@return QList<int>, key's list
         */
        QList<int> getAllIntersectionPointKeys();

        /**
         * return all control point's keys
         *@return QList<int>, key's list
         */
        QList<int> getAllControlPointKeys();

        /**
         * clear all the structure (delete everything), undo/redo works on it
         */
        void clearSurface();


        /************************
         * UNDO/REDO Management *
         ************************/

        void startHistory(Modification t);

        HistoryHolder<Modification> * retrieveHistory(Modification t);

        void undo(HistoryHolder<Modification> * history);

    private:

        void insertControlPoint(int key, Point *controlPoint);

        void insertIntersectionPoint(int key, Point *intersectionPoint);

        void insertSegment(int segmentKey,int intersectionPointKey1,int intersectionPointKey2,int controlPointKey);

        void setSegment(int segmentKey, int intersectionKey1, int intersectionKey2, int controlKey);

        void removeSegment(int segmentKey);
    };

}

#endif // SURFACE_H
