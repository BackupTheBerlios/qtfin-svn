#ifndef SURFACE_H
#define SURFACE_H

#include <QPointF>
#include <QHash>
#include <QList>
#include "segment.h"
#include "historyholder.h"
#include "DataConstants.h"
#include "SaveVisitor.h"
#include "LoadVisitor.h"

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
        Surface(): HistoryMaker<Modification>(), _segmentKey(0), _intersectionKey(0), _controlKey(0) {}

        virtual ~Surface();

        /**
         * Surface Modification
         */

        /**
         * adds a segment to the segment table
         *@param intersectionPointKey1 an integer, the key of the point representing the first edge of the segment, must be a valid key, otherwise the segment will not be created
         *@param intersectionPointKey2 an integer, the key of the point representing the second edge of the segment, must be a valid key, otherwise the segment will not be created
         *@param controlPointKey an integer, the, must be a valid key or MONOFIN_SURFACE_NO_CONTROL_POINT, otherwise the segment will not be created
         *@return an integer, the key of the created segment or MONOFIN_SURFACE_NOT_CREATED_SEGMENT if an error occured.
         */
        int addSegment(int intersectionPointKey1, int intersectionPointKey2, int controlPointKey);

        /**
         * adds a point to the control point table
         *@param x a float, x-axis coordinate
         *@param y a float, y-axis coordinate
         *@return an integer, the key of the created control point, or  MONOFIN_SURFACE_NOT_CREATED_POINT
         */
        int addControlPoint(float x, float y);

        /**
         * adds a point to the intersection point table
         *@param x a float, x-axis coordinate
         *@param y a float, y-axis coordinate
         *@return an integer, the key of the created intersection point, or  MONOFIN_SURFACE_NOT_CREATED_POINT
         */
        int addIntersectionPoint(float x, float y);

        /**
         * subdivides a segment by the right
         *@param segmentKey an integer, the segment to subdivide, must be a valid segment key otherwise, the function will have no effect
         *@param intersectionPointKey, the intersection point that will subdivide the original segment into two segments, must be a valid key otherwise the function will have no effect
         *@return an integer, the key of the created segment or MONOFIN_SURFACE_NOT_CREATED_SEGMENT if a key is not valid or an error occured
         */
        int subdivideSegment(int segmentKey, int intersectionPointKey);

        /**
         * removes the control point which key is given
         *@param controlPointKey an integer, the key of the control point to remove, must be a valid key otherwise nothing will be done
         */
        void removeControlPoint(int controlPointKey);

        /**
         * removes the intersection point which key is given and merge the two segments using this point
         *@param intersectionPointKey an integer, the key of the intersection point to remove, must be a valid key otherwise nothing will be done
         *@return an integer, the key of the erased segement, or MONOFIN_SURFACE_NO_SEGMENT_ERASED if the key weren't valid or an error occured
         */
        int removeIntersectionPoint(int intersectionPointKey);

        /**
         * sets new coordinates for the intersection point which key is given
         *@param intersectionPointKey an integer, the key of the intersection point to modify
         *@param x a float, the new x-axis coordinate of the point
         *@param y a float, the new y-axis coordinate of the point
         */
        void setIntersectionPoint(int intersectionPointKey, float x, float y);

        /**
         * sets new coordinates for the control point which key is given
         *@param intersectionPointKey an integer, the key of the control point to modify
         *@param x a float, the new x-axis coordinate of the point
         *@param y a float, the new y-axis coordinate of the point
         */
        void setControlPoint(int controlPointKey, float x, float y);

        /**
         * sets an existing control point to an existing segment, if keys are incorrect nothing will be done
         *@param segmentKey an integer, the key of the segment we want to add a control point
         *@param controlPointKey an integer, the key of the control point to add
         */
        void addControlPointToSegment(int segmentKey, int controlPointKey);

        /**
         * getters on surface
         */

        /**
         * gets the coordinates of the intersection point which key is given
         *@param intersectionPointKey an integer, the key of the point we want the coordinates
         *@param x a float, return the x-axis coordinate
         *@param y a float, return the y-axis coordinate
         */
        void getIntersectionPoint(int intersectionPointKey, float &x, float &y);

        /**
         * gets the coordinates of the control point which key is given
         *@param controlPointKey an integer, the key of the point we want the coordinates
         *@param x a float, return the x-axis coordinate
         *@param y a float, return the y-axis coordinate
         */
        void getControlPoint(int controlPointKey, float &x, float &y);

        /**
         * gets all the informations over a segment
         *@param segmentKey an integer, the key of the segment we want informations over
         *@param intersectionPointKey1 an integer, return the key of the first intersection point of the segment
         *@param intersectionPointKey2 an integer, return the key of the second intersection point of the segment
         *@param controlPointKey an integer, return the key of the control point (bezier curve), may return MONOFIN_SURFACE_NO_SEGMENT_ERASED if the point don't exist
         */
        void getSegment(int segmentKey, int &intersectionPointKey1, int &intersectionPointKey2, int &controlPointKey);

        /**
         * returns all segment's keys
         *@return QList<int>, key's list
         */
        QList<int> getAllSegmentKeys();

        /**
         * returns all intersection point's keys
         *@return QList<int>, key's list
         */
        QList<int> getAllIntersectionPointKeys();

        /**
         * returns all control point's keys
         *@return QList<int>, key's list
         */
        QList<int> getAllControlPointKeys();

        /**
          * returns all IntersectionPoint only linked to one segment
          *@return QList<int>, key's list
          */
        QList<int> getExtremityPoint();

        /**
          * returns for one point the list of the segment which have this point for extremity
          *@param pointKey an integer, the key of the point concerned
          *@return QList<int>, list of the linked segments' keys
          */
        QList<int> getSegmentKeysLinkedToPoint(int pointKey);

        /**
         * clear all the structure (delete everything), undo/redo works on it
         */
        void clearSurface();


        /************************
         * UNDO/REDO Management *
         ************************/

       /**
         * creates a new history, erasing the older one
         *@param t Modification (see DataConstants.h), if the type of t correspondes to MonofinSurface, a new list of HistoryHolders is created
         */
        void startHistory(Modification t);

        /**
         * returns the current list of HistoryHolders if the Modification is MonofinSurface
         * the attribute _MakedHistory is then replaced by a new list of HistoryHolders
         *@param t Modification (see DataConstants.h)
         *@return a pointer to the former _makedHistory. if t if not MonofinSurface, return an empty list of HistoryHolders
         */
        QList<HistoryHolder<Modification> *> retrieveHistory(Modification t);

        /**
         * performs an undo operation using the list of HistoryHolder
         * For each HistoryHolderin the list it is poped and a switch choose which function must be used to un-do the action
         *@param history a list of HistoryHolders which has to be of the MonofinSurface type
         */
        void undo(QList<HistoryHolder<Modification> *> history);


        /**
         * allows the SaveVisitor to explore the profil
         * Call the visiteProfil method of the visitor
         *@param v pointer to a Savevisitor, which is going to explore Profil
         */
        void accept(SaveVisitor *sv);

        /**
         * allows the LoadVisitor to explore the profil
         * Call the visiteProfil method of the visitor
         *@param v pointer to a Loadvisitor, which is going to explore Profil
         */
        void accept(LoadVisitor *lv);

        /**
         * loads a new IntersectionPoint according to the prameters
         *@param key an integer, it define the position of the point in the table (can erase if the key is already allocated)
         *@param x a float, the x coordinate
         *@param y a float, the y coordinate
         */
        void loadIntersectionPoint(int key, float x, float y);

        /**
         * loads a new ControlPoint according to the prameters
         *@param key an integer, it define the position of the point in the table (can erase if the key is already allocated)
         *@param x a float, the x coordinate
         *@param y a float, the y coordinate
         */
        void loadControlPoint(int key, float x, float y);


        /**
         * loads a new Segment according to the prameters from a request
         *@param segmentKey an integer, it define the position of the segment in the table (can erase if the key is already allocated)
         *@param intersectionPointKey1 an integer, it is the reference in the point table to the first point defining the segment
         *@param intersectionPointKey2 an integer, it is the reference in the point table to the second point defining the segment
         *@param controlPointKey an integer, it refers to the controle point parametring the Bezier curve (if MONOFIN_SURFACE_NO_CONTROL_POINT, creates one)
         */
        void loadSegment(int segmentKey,int intersectionPointKey1, int intersectionPoint2, int controlPointKey);

    private:

        /**
         * inserts a prec-reated controlPoint in the table
         *@param key an integer, it define the position of the point in the table (can erase if the key is already allocated)
         *@param controlPoint a pointer to a Point, to be direcly entered in the controlPoint table
         */
        void insertControlPoint(int key, Point *controlPoint);

        /**
         * inserts a pre-created intersectionPoint in the table
         *@param key an integer, it define the position of the point in the table (can erase if the key is already allocated)
         *@param intersectionPoint a pointer to a Point, to be direcly entered in the intersectionPoint table
         */
        void insertIntersectionPoint(int key, Point *intersectionPoint);

        /**
         * inserts a segment in the table (Called by loadSegment)
         * manages the number of segments
         *@param segmentKey an integer, it define the position of the segment in the table (can erase if the key is already allocated)
         *@param intersectionPointKey1 an integer, it is the reference in the point table to the first point defining the segment
         *@param intersectionPointKey2 an integer, it is the reference in the point table to the second point defining the segment
         *@param controlPointKey an integer, it refers to the controle point parametring the Bezier curve
         */
        void insertSegment(int segmentKey,int intersectionPointKey1,int intersectionPointKey2,int controlPointKey);

        /**
         * sets the new points designating the segment
         *@param segmentKey an integer, the position of the segment in the table (can erase if the key is already allocated)
         *@param intersectionPointKey1 an integer, it is the reference in the point table to the first point defining the segment
         *@param intersectionPointKey2 an integer, it is the reference in the point table to the second point defining the segment
         *@param controlPointKey an integer, it refers to the controle point parametring the Bezier curve
         */
        void setSegment(int segmentKey, int intersectionKey1, int intersectionKey2, int controlKey);

        /**
         * removes the designated segment
         *@param segmentKey an integer, it define the position of the segment in the table (if no segment, no removal)
         */
        void removeSegment(int segmentKey);
    };

}

#endif // SURFACE_H
