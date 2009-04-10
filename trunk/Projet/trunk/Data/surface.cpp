#include "surface.h"

namespace Data{


    Surface::~Surface(){
        foreach (int segmentKey, _segmentTable.keys()){
            Segment * toDelete = _segmentTable.value(segmentKey);
            _segmentTable.remove(segmentKey);
            delete toDelete;
        }

        foreach (int interKey, _intersectionPointTable.keys()){
            Point * toDelete = _intersectionPointTable.value(interKey);
            _intersectionPointTable.remove(interKey);
            delete toDelete;
        }

        foreach (int controlKey, _controlPointTable.keys()){
            Point * toDelete = _controlPointTable.value(controlKey);
            _controlPointTable.remove(controlKey);
            delete toDelete;
        }
    }

    /**
     * add a segment to the segment table
     *@param intersectionPointKey1 an integer, the key of the point representing the first edge of the segment, must be a valid key, otherwise the segment will not be created
     *@param intersectionPointKey2 an integer, the key of the point representing the second edge of the segment, must be a valid key, otherwise the segment will not be created
     *@param controlPointKey an integer, the, must be a valid key or MONOFIN_SURFACE_NO_CONTROL_POINT, otherwise the segment will not be created
     *@return an integer, the key of the created segment or MONOFIN_SURFACE_NOT_CREATED_SEGMENT if an error occured.
     */
    int Surface::addSegment(int intersectionPointKey1, int intersectionPointKey2, int controlPointKey){
        if(!_intersectionPointTable.contains(intersectionPointKey1) ||
           !_intersectionPointTable.contains(intersectionPointKey2))
            return MONOFIN_SURFACE_NOT_CREATED_SEGMENT;


        //intersection points are already used by two or more segment they can't
        // be used anymore
        if(_intersectionPointTable.value(intersectionPointKey1)->countUsage>=2 ||
           _intersectionPointTable.value(intersectionPointKey2)->countUsage>=2)
            return MONOFIN_SURFACE_NOT_CREATED_SEGMENT;

        if(controlPointKey!=MONOFIN_SURFACE_NO_CONTROL_POINT){

            if(!_controlPointTable.contains(controlPointKey))
                return MONOFIN_SURFACE_NOT_CREATED_SEGMENT;

            //the control can't be used by more than one segment
            if(_controlPointTable.value(controlPointKey)->countUsage>=1)
                return MONOFIN_SURFACE_NOT_CREATED_SEGMENT;

            _controlPointTable.value(controlPointKey)->countUsage++;
            _controlPointTable.value(controlPointKey)->whereUsed.append(_segmentKey);
        }


        _intersectionPointTable.value(intersectionPointKey1)->countUsage++;
        _intersectionPointTable.value(intersectionPointKey1)->whereUsed.push_back(_segmentKey);

        _intersectionPointTable.value(intersectionPointKey2)->countUsage++;
        _intersectionPointTable.value(intersectionPointKey2)->whereUsed.push_back(_segmentKey);

        _segmentTable.insert(_segmentKey++,new Segment(intersectionPointKey1,intersectionPointKey2,controlPointKey));

        return (_segmentKey-1);
    }

    /**
     * add a point to the control point table
     *@param x a float, x-axis coordinate
     *@param y a float, y-axis coordinate
     *@return an integer, the key of the created control point, or  MONOFIN_SURFACE_NOT_CREATED_POINT
     */
    int Surface::addControlPoint(float x, float y){
        Point *newControlPoint = new Point;
        newControlPoint->coordinates = QPointF(x,y);
        newControlPoint->countUsage = 0;
        _controlPointTable.insert(_controlKey++,newControlPoint);
        return (_controlKey-1);
    }

    /**
     * add a point to the intersection point table
     *@param x a float, x-axis coordinate
     *@param y a float, y-axis coordinate
     *@return an integer, the key of the created intersection point, or  MONOFIN_SURFACE_NOT_CREATED_POINT
     */
    int Surface::addIntersectionPoint(float x, float y){
        Point *newIntersectionPoint = new Point;
        newIntersectionPoint->coordinates = QPointF(x,y);
        newIntersectionPoint->countUsage = 0;
        _intersectionPointTable.insert(_intersectionKey++, newIntersectionPoint);
        return (_intersectionKey-1);
    }

    /**
     * subdivide a segment by the right
     *@param segmentKey an integer, the segment to subdivide, must be a valid segment key otherwise, the function will have no effect
     *@param intersectionPointKey, the intersection point that will subdivide the original segment into two segments, must be a valid key otherwise the function will have no effect
     *@return an integer, the key of the created segment or MONOFIN_SURFACE_NOT_CREATED_SEGMENT if a key is not valid or an error occured
     */
    int Surface::subdivideSegment(int segmentKey, int intersectionPointKey){

        //the initial segment or the intersection point don't exist
        if(!_segmentTable.contains(segmentKey) ||
           !_intersectionPointTable.contains(intersectionPointKey))
            return MONOFIN_SURFACE_NOT_CREATED_SEGMENT;

        //the intersection point is use by at least a segment
        if(_intersectionPointTable.value(intersectionPointKey)->countUsage!=0)
            return MONOFIN_SURFACE_NOT_CREATED_SEGMENT;

        int interKey1,interKey2,controlKey;

        //get all key
        _segmentTable.value(segmentKey)->getAll(interKey1,interKey2,controlKey);

        //prepare the last intersection point in order to create a new segment
        _intersectionPointTable.value(interKey2)->countUsage--;
        _intersectionPointTable.value(interKey2)->whereUsed.removeAll(segmentKey);

        int newSegment = addSegment(intersectionPointKey,interKey2,MONOFIN_SURFACE_NO_CONTROL_POINT);

        //if an error occured during the creation, we reset all information and return an error code
        if(newSegment==MONOFIN_SURFACE_NOT_CREATED_SEGMENT){
            _intersectionPointTable.value(interKey2)->countUsage++;
            _intersectionPointTable.value(interKey2)->whereUsed.append(segmentKey);

            return MONOFIN_SURFACE_NOT_CREATED_SEGMENT;
        }

        if(controlKey!=MONOFIN_SURFACE_NO_CONTROL_POINT)
            removeControlPoint(controlKey);

        _segmentTable.value(segmentKey)->setAll(interKey1,intersectionPointKey,MONOFIN_SURFACE_NO_CONTROL_POINT);
        _intersectionPointTable.value(intersectionPointKey)->countUsage++;
        _intersectionPointTable.value(intersectionPointKey)->whereUsed.append(segmentKey);

        return newSegment;

    }

    /**
     * remove the control point which key is given
     *@param controlPointKey an integer, the key of the control point to remove, must be a valid key otherwise nothing will be done
     */
    void Surface::removeControlPoint(int controlPointKey){
        if (_controlPointTable.contains(controlPointKey)){
            Point * controlPoint = _controlPointTable.value(controlPointKey);
            //remove control point from the segment it is used in
            while(!controlPoint->whereUsed.isEmpty()){
                int segmentKey = controlPoint->whereUsed.back();
                controlPoint->whereUsed.pop_back();
                if(_segmentTable.contains(segmentKey)){
                    _segmentTable.value(segmentKey)->setControlPointKey(MONOFIN_SURFACE_NO_CONTROL_POINT);
                }
            }

            _controlPointTable.remove(controlPointKey);
            delete controlPoint;
        }
    }

    /**
     * remove the intersection point which key is given and merge the two segments using this point
     *@param intersectionPointKey an integer, the key of the intersection point to remove, must be a valid key otherwise nothing will be done
     *@return an integer, the key of the erased segement, or MONOFIN_SURFACE_NO_SEGMENT_ERASED if the key weren't valid or an error occured
     */
    int Surface::removeIntersectionPoint(int intersectionPointKey){
        if(!_intersectionPointTable.contains(intersectionPointKey))
            return MONOFIN_SURFACE_NO_SEGMENT_ERASED;

        //if the intersection point is at an edge of the semi-surface
        if(_intersectionPointTable.value(intersectionPointKey)->countUsage==1)
            return MONOFIN_SURFACE_NO_SEGMENT_ERASED;

        if(_intersectionPointTable.value(intersectionPointKey)->countUsage==2){
            // the intersection point is between the two segments, we will merge the two
            int segmentKey1, segmentKey2;
            segmentKey1 = _intersectionPointTable.value(intersectionPointKey)->whereUsed.back();
            _intersectionPointTable.value(intersectionPointKey)->whereUsed.pop_back();
            segmentKey2 = _intersectionPointTable.value(intersectionPointKey)->whereUsed.back();
            _intersectionPointTable.value(intersectionPointKey)->whereUsed.pop_back();

            int inter11, inter12, inter21, inter22, control1, control2;

            if(!_segmentTable.contains(segmentKey1) ||
               !_segmentTable.contains(segmentKey2))
                return MONOFIN_SURFACE_NO_SEGMENT_ERASED;

            _segmentTable.value(segmentKey1)->getAll(inter11,inter12,control1);
            _segmentTable.value(segmentKey2)->getAll(inter21,inter22,control2);

            if( intersectionPointKey!=inter11 && intersectionPointKey!=inter12 ||
                intersectionPointKey!=inter21 && intersectionPointKey!=inter22)
                return MONOFIN_SURFACE_NO_SEGMENT_ERASED;

            removeControlPoint(control1);
            removeControlPoint(control2);

            if (intersectionPointKey == inter11){
                if (intersectionPointKey == inter22){
                    //we have identified the two modifications to do
                    _intersectionPointTable.value(inter21)->whereUsed.removeAll(segmentKey2);
                    _intersectionPointTable.value(inter21)->whereUsed.push_back(segmentKey1);
                    //we decide here to delete the segment identified by segmentKey2
                    _segmentTable.value(segmentKey1)->setAll(inter21,inter12,MONOFIN_SURFACE_NO_CONTROL_POINT);
                    //we modify the other segment, setting its intersection points
                    _segmentTable.remove(segmentKey2);
                    //we remove the segment

                    //finally, we remove the intersection point
                    Point *toRemove = _intersectionPointTable.value(inter11);
                    _intersectionPointTable.remove(inter11);
                    delete toRemove;

                }else{
                    _intersectionPointTable.value(inter22)->whereUsed.removeAll(segmentKey2);
                    _intersectionPointTable.value(inter22)->whereUsed.push_back(segmentKey1);

                    _segmentTable.value(segmentKey1)->setAll(inter22,inter12,MONOFIN_SURFACE_NO_CONTROL_POINT);
                    _segmentTable.remove(segmentKey2);

                    Point *toRemove = _intersectionPointTable.value(inter11);
                    _intersectionPointTable.remove(inter11);
                    delete toRemove;
                }

                return segmentKey2;
            }else{
                if (intersectionPointKey == inter21){
                    _intersectionPointTable.value(inter11)->whereUsed.removeAll(segmentKey1);
                    _intersectionPointTable.value(inter11)->whereUsed.push_back(segmentKey2);

                    _segmentTable.value(segmentKey2)->setAll(inter11,inter22,MONOFIN_SURFACE_NO_CONTROL_POINT);
                    _segmentTable.remove(segmentKey1);

                    Point *toRemove = _intersectionPointTable.value(inter21);
                    _intersectionPointTable.remove(inter21);
                    delete toRemove;

                }else{
                    _intersectionPointTable.value(inter11)->whereUsed.removeAll(segmentKey1);
                    _intersectionPointTable.value(inter11)->whereUsed.push_back(segmentKey2);

                    _segmentTable.value(segmentKey2)->setAll(inter11,inter21,MONOFIN_SURFACE_NO_CONTROL_POINT);
                    _segmentTable.remove(segmentKey1);

                    Point *toRemove = _intersectionPointTable.value(inter22);
                    _intersectionPointTable.remove(inter22);
                    delete toRemove;
                }

                return segmentKey1;
            }



        }

    }

    /**
     * set new coordinates for the intersection point which key is given
     *@param intersectionPointKey an integer, the key of the intersection point to modify
     *@param x a float, the new x-axis coordinate of the point
     *@param y a float, the new y-axis coordinate of the point
     */
    void Surface::setIntersectionPoint(int intersectionPointKey, float x, float y){
        if(_intersectionPointTable.contains(intersectionPointKey))
            _intersectionPointTable.value(intersectionPointKey)->coordinates=QPointF(x,y);

    }

    /**
     * set new coordinates for the control point which key is given
     *@param intersectionPointKey an integer, the key of the control point to modify
     *@param x a float, the new x-axis coordinate of the point
     *@param y a float, the new y-axis coordinate of the point
     */
    void Surface::setControlPoint(int controlPointKey, float x, float y){
        if(_controlPointTable.contains(controlPointKey))
            _controlPointTable.value(controlPointKey)->coordinates=QPointF(x,y);

    }

    /**
     * set an existing control point to an existing segment, if keys are incorrect nothing will be done
     *@param segmentKey an integer, the key of the segment we want to add a control point
     *@param controlPointKey an integer, the key of the control point to add
     */
    void Surface::addControlPointToSegment(int segmentKey, int controlPointKey){
        if(_segmentTable.contains(segmentKey) &&
           _controlPointTable.contains(controlPointKey)){
            if(_controlPointTable.value(controlPointKey)->countUsage!=0)
                return;

            if(_segmentTable.value(segmentKey)->getControlPointKey()==MONOFIN_SURFACE_NO_CONTROL_POINT){
                _controlPointTable.value(controlPointKey)->countUsage++;
                _controlPointTable.value(controlPointKey)->whereUsed.push_back(segmentKey);
                _segmentTable.value(segmentKey)->setControlPointKey(controlPointKey);
            }
        }
    }

    /**
     * get the coordinates of the intersection point which key is given
     *@param intersectionPointKey an integer, the key of the point we want the coordinates
     *@param x a float, return the x-axis coordinate
     *@param y a float, return the y-axis coordinate
     */
    void Surface::getIntersectionPoint(int intersectionPointKey, float &x, float &y){
        if(_intersectionPointTable.contains(intersectionPointKey)){
            QPointF point = _intersectionPointTable.value(intersectionPointKey)->coordinates;
            x = point.x();
            y = point.y();
        }
    }

    /**
     * get the coordinates of the control point which key is given
     *@param controlPointKey an integer, the key of the point we want the coordinates
     *@param x a float, return the x-axis coordinate
     *@param y a float, return the y-axis coordinate
     */
    void Surface::getControlPoint(int controlPointKey, float &x, float &y){
        if(_controlPointTable.contains(controlPointKey)){
            QPointF point = _controlPointTable.value(controlPointKey)->coordinates;
            x = point.x();
            y = point.y();
        }
    }

    /**
     * get all the informations over a segment
     *@param segmentKey an integer, the key of the segment we want informations over
     *@param intersectionPointKey1 an integer, return the key of the first intersection point of the segment
     *@param intersectionPointKey2 an integer, return the key of the second intersection point of the segment
     *@param controlPointKey an integer, return the key of the control point (bezier curve), may return MONOFIN_SURFACE_NO_SEGMENT_ERASED if the point don't exist
     */
    void Surface::getSegment(int segmentKey, int &intersectionPointKey1, int &intersectionPointKey2, int &controlPointKey){
        if(_segmentTable.contains(segmentKey)){
            _segmentTable.value(segmentKey)->getAll(intersectionPointKey1,intersectionPointKey2,controlPointKey);
        }
    }
} // namespace Data
