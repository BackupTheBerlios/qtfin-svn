#include "surface.h"
#include <iostream>

using namespace std;

namespace Data{

    enum SurfaceUndoRedoCodes{AddSegment,AddControlPoint,AddIntersectionPoint,
                   RemoveControlPoint,RemoveIntersectionPoint,RemoveSegment,
                   SetSegment,SetControlPoint,SetIntersectionPoint};


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

        //save for undo/redo
        HistoryHolder<Modification> * current = getCurrentHistoryHolder(MonofinSurface);
        current->push((void*)_segmentKey);
        current->push((void*)AddSegment);

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

        if(newControlPoint == NULL)
            return MONOFIN_SURFACE_NOT_CREATED_POINT;

        newControlPoint->coordinates = QPointF(x,y);
        newControlPoint->countUsage = 0;

        //manage undo/redo history
        HistoryHolder<Modification> * current = getCurrentHistoryHolder(MonofinSurface);
        current->push((void *)(_controlKey));
        current->push((void *)(AddControlPoint));

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

        if(newIntersectionPoint == NULL)
            return MONOFIN_SURFACE_NOT_CREATED_POINT;

        newIntersectionPoint->coordinates = QPointF(x,y);
        newIntersectionPoint->countUsage = 0;

        //save for undo/redo
        HistoryHolder<Modification> * current = getCurrentHistoryHolder(MonofinSurface);
        current->push((void*)_intersectionKey);
        current->push((void*)AddIntersectionPoint);

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

        _segmentTable.value(segmentKey)->getAll(interKey1,interKey2,controlKey);

        setSegment(segmentKey,interKey1,intersectionPointKey,MONOFIN_SURFACE_NO_CONTROL_POINT);

        int newSegment = addSegment(intersectionPointKey,interKey2,MONOFIN_SURFACE_NO_CONTROL_POINT);

        //undo/redo ==> Not Needed here, composed of atomic actions already reversible
        //current->push((void *)newSegment);
        //current->push((void *)segmentKey);
        //current->push((void *)intersectionPointKey);
        //current->push((void *)SubdivideSegment);

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
            if(!controlPoint->whereUsed.isEmpty()){
                int segmentKey = controlPoint->whereUsed.back();
                if(_segmentTable.contains(segmentKey)){
                    _segmentTable.value(segmentKey)->setControlPointKey(MONOFIN_SURFACE_NO_CONTROL_POINT);
                }
            }

            //undo/redo
            HistoryHolder<Modification> * current = getCurrentHistoryHolder(MonofinSurface);
            current->push((void *)controlPointKey);
            current->push((void *)controlPoint);
            current->push((void *)RemoveControlPoint);

            _controlPointTable.remove(controlPointKey);
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
        if(_intersectionPointTable.value(intersectionPointKey)->countUsage==1 ||
           _intersectionPointTable.value(intersectionPointKey)->countUsage>2 )
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

            removeSegment(segmentKey2);

            if (intersectionPointKey == inter22){
                if (intersectionPointKey == inter11){
                    setSegment(segmentKey1,inter21,inter12,MONOFIN_SURFACE_NO_CONTROL_POINT);
                }else{
                    setSegment(segmentKey1,inter11,inter21,MONOFIN_SURFACE_NO_CONTROL_POINT);
                }

            }else{
                if (intersectionPointKey == inter11){
                    setSegment(segmentKey1,inter12,inter22,MONOFIN_SURFACE_NO_CONTROL_POINT);
                }else{
                    setSegment(segmentKey1,inter11,inter22,MONOFIN_SURFACE_NO_CONTROL_POINT);
                }
            }

            Point * save = _intersectionPointTable.value(intersectionPointKey);
            _intersectionPointTable.remove(intersectionPointKey);

            save->countUsage = 0;
            save->whereUsed.clear();

            HistoryHolder<Modification> * current = getCurrentHistoryHolder(MonofinSurface);

            current->push((void *)intersectionPointKey);
            current->push((void *)save);
            current->push((void *)RemoveIntersectionPoint);

            return segmentKey2;
        }

        if(_intersectionPointTable.value(intersectionPointKey)->countUsage==0){
            Point * save = _intersectionPointTable.value(intersectionPointKey);
            _intersectionPointTable.remove(intersectionPointKey);

            save->whereUsed.clear();

            HistoryHolder<Modification> * current = getCurrentHistoryHolder(MonofinSurface);

            current->push((void *)intersectionPointKey);
            current->push((void *)save);
            current->push((void *)RemoveIntersectionPoint);
        }
        return MONOFIN_SURFACE_NO_SEGMENT_ERASED;
    }

    /**
     * set new coordinates for the intersection point which key is given
     *@param intersectionPointKey an integer, the key of the intersection point to modify
     *@param x a float, the new x-axis coordinate of the point
     *@param y a float, the new y-axis coordinate of the point
     */
    void Surface::setIntersectionPoint(int intersectionPointKey, float x, float y){
        if(_intersectionPointTable.contains(intersectionPointKey)){
            QPointF * save = new QPointF(_intersectionPointTable.value(intersectionPointKey)->coordinates.x(),
                                         _intersectionPointTable.value(intersectionPointKey)->coordinates.y());

            HistoryHolder<Modification> * current = getCurrentHistoryHolder(MonofinSurface);

            current->push((void *)intersectionPointKey);
            current->push((void *)save);
            current->push((void *)SetIntersectionPoint);

            _intersectionPointTable.value(intersectionPointKey)->coordinates=QPointF(x,y);
        }

    }

    /**
     * set new coordinates for the control point which key is given
     *@param intersectionPointKey an integer, the key of the control point to modify
     *@param x a float, the new x-axis coordinate of the point
     *@param y a float, the new y-axis coordinate of the point
     */
    void Surface::setControlPoint(int controlPointKey, float x, float y){
        if(_controlPointTable.contains(controlPointKey)){
            QPointF * save = new QPointF(_controlPointTable.value(controlPointKey)->coordinates.x(),
                                         _controlPointTable.value(controlPointKey)->coordinates.y());

            HistoryHolder<Modification> * current = getCurrentHistoryHolder(MonofinSurface);

            current->push((void *)controlPointKey);
            current->push((void *)save);
            current->push((void *)SetControlPoint);

            _controlPointTable.value(controlPointKey)->coordinates=QPointF(x,y);
        }

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

            int inter1,inter2,control;
            _segmentTable.value(segmentKey)->getAll(inter1,inter2,control);
            //manage easily undo/redo actions
            setSegment(segmentKey,inter1,inter2,controlPointKey);
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
        }else{
            x=-1.0f;
            y=-1.0f;
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

    void Surface::startHistory(Modification t){
        if (t==MonofinSurface)
            _makedHistory = NULL;
    }

    HistoryHolder<Modification> * Surface::retrieveHistory(Modification t){
        if (t==MonofinSurface)
            return _makedHistory;
        else
            return NULL;
    }

    void Surface::undo(HistoryHolder<Modification> * history){
        if(history == NULL)
            return;

        if (history->getType()==MonofinSurface){
        // if actions to undo are from us
            HistoryHolder<Modification> * historyReader = history;
            while(historyReader!=NULL){

                int code_action = (int)(historyReader->pop());
                switch(code_action){
                case AddControlPoint:{
                    removeControlPoint((int)(historyReader->pop()));
                    break;
                }

                case RemoveControlPoint:{
                    Point * controlPoint;
                    int key;
                    controlPoint = (Point *)historyReader->pop();
                    key = (int)(historyReader->pop());

                    insertControlPoint(key, controlPoint);
                    break;
                }

                case AddIntersectionPoint:{
                    removeIntersectionPoint((int)(historyReader->pop() ) );
                    break;
                }

                case RemoveIntersectionPoint:{
                    Point * intersectionPoint;
                    int key;

                    intersectionPoint = (Point *) (historyReader->pop());
                    key = (int) (historyReader->pop());
                    insertIntersectionPoint(key,intersectionPoint);
                    break;
                }

                case AddSegment:{
                    removeSegment((int)historyReader->pop());
                    break;
                }

                case RemoveSegment:{
                    int segKey,inter1,inter2,control;
                    control = (int)(historyReader->pop());
                    inter2 = (int)(historyReader->pop());
                    inter1 = (int)(historyReader->pop());
                    segKey = (int)(historyReader->pop());
                    insertSegment(segKey,inter1,inter2,control);
                    break;
                }

                case SetControlPoint:{
                    QPointF * coord;
                    int key;
                    coord = (QPointF *) (historyReader->pop());
                    key = (int) (historyReader->pop());
                    setControlPoint(key,coord->x(),coord->y());
                    delete coord;
                    break;
                }

                case SetIntersectionPoint:{
                    QPointF * coord;
                    int key;
                    coord = (QPointF *) (historyReader->pop());
                    key = (int) (historyReader->pop());
                    setIntersectionPoint(key,coord->x(),coord->y());
                    delete coord;
                    break;
                }

                case SetSegment:{
                    int segKey,inter1,inter2,control;
                    control = (int)(historyReader->pop());
                    inter2 = (int)(historyReader->pop());
                    inter1 = (int)(historyReader->pop());
                    segKey = (int)(historyReader->pop());
                    setSegment(segKey,inter1,inter2,control);
                    break;
                }
                default:
                    cout << "default : code : " << code_action << endl;
                    break;
                }

                //next action to undo
                HistoryHolder<Modification> * toDelete = historyReader;
                historyReader = historyReader->getNext();
                delete toDelete;
            }
        }
    }

    void Surface::insertControlPoint(int key, Point * controlPoint){

        //manage undo/redo history
        HistoryHolder<Modification> * current = getCurrentHistoryHolder(MonofinSurface);
        current->push((void *)(key));
        current->push((void *)(AddControlPoint));
        
        _controlPointTable.insert(key,controlPoint);
    }

    void Surface::insertIntersectionPoint(int key, Point *intersectionPoint){
        //manage undo/redo history
        HistoryHolder<Modification> * current = getCurrentHistoryHolder(MonofinSurface);
        current->push((void *)(key));
        current->push((void *)(AddIntersectionPoint));

        _intersectionPointTable.insert(key,intersectionPoint);
    }

    void Surface::insertSegment(int segmentKey, int intersectionPointKey1,int intersectionPointKey2,int controlPointKey){
        Segment * toInsert = new Segment(intersectionPointKey1,intersectionPointKey2,controlPointKey);

        _intersectionPointTable.value(intersectionPointKey1)->countUsage++;
        _intersectionPointTable.value(intersectionPointKey2)->countUsage++;
        _intersectionPointTable.value(intersectionPointKey1)->whereUsed.append(segmentKey);
        _intersectionPointTable.value(intersectionPointKey2)->whereUsed.append(segmentKey);

        if(controlPointKey != MONOFIN_SURFACE_NO_CONTROL_POINT){
            _controlPointTable.value(controlPointKey)->countUsage++;
            _controlPointTable.value(controlPointKey)->whereUsed.append(segmentKey);
        }

        //save for undo/redo
        HistoryHolder<Modification> * current = getCurrentHistoryHolder(MonofinSurface);
        current->push((void*)segmentKey);
        current->push((void*)AddSegment);

        _segmentTable.insert(segmentKey,toInsert);

    }

    void Surface::setSegment(int segmentKey, int intersectionKey1, int intersectionKey2, int controlKey){

        int inter1,inter2,control;
        _segmentTable.value(segmentKey)->getAll(inter1,inter2,control);

        _intersectionPointTable.value(inter1)->countUsage--;
        _intersectionPointTable.value(inter2)->countUsage--;
        _intersectionPointTable.value(inter1)->whereUsed.removeAll(segmentKey);
        _intersectionPointTable.value(inter2)->whereUsed.removeAll(segmentKey);

        if(control != MONOFIN_SURFACE_NO_CONTROL_POINT){
            _controlPointTable.value(control)->countUsage--;
            _controlPointTable.value(control)->whereUsed.removeAll(segmentKey);
        }

        HistoryHolder<Modification> * current = getCurrentHistoryHolder(MonofinSurface);
        current->push((void *)segmentKey);
        current->push((void *)inter1);
        current->push((void *)inter2);
        current->push((void *)control);
        current->push((void *)SetSegment);

        _intersectionPointTable.value(intersectionKey1)->countUsage++;
        _intersectionPointTable.value(intersectionKey2)->countUsage++;
        _intersectionPointTable.value(intersectionKey1)->whereUsed.append(segmentKey);
        _intersectionPointTable.value(intersectionKey2)->whereUsed.append(segmentKey);

        if(controlKey != MONOFIN_SURFACE_NO_CONTROL_POINT){
            _controlPointTable.value(controlKey)->countUsage++;
            _controlPointTable.value(controlKey)->whereUsed.append(segmentKey);
        }

        _segmentTable.value(segmentKey)->setAll(intersectionKey1,intersectionKey2,controlKey);
    }

    void Surface::removeSegment(int segmentKey){
        int inter1,inter2,control;


        _segmentTable.value(segmentKey)->getAll(inter1,inter2,control);
        Segment * toDelete = _segmentTable.value(segmentKey);

        _segmentTable.remove(segmentKey);
        delete toDelete;

        HistoryHolder<Modification> * current = getCurrentHistoryHolder(MonofinSurface);
        current->push((void *)segmentKey);
        current->push((void *)inter1);
        current->push((void *)inter2);
        current->push((void *)control);
        current->push((void *)RemoveSegment);

        _intersectionPointTable.value(inter1)->countUsage--;
        _intersectionPointTable.value(inter1)->whereUsed.removeAll(segmentKey);

        /*if(_intersectionPointTable.value(inter1)->countUsage==0){
            removeIntersectionPoint(inter1);
        }*/
        _intersectionPointTable.value(inter2)->countUsage--;
        _intersectionPointTable.value(inter2)->whereUsed.removeAll(segmentKey);

        /*if(_intersectionPointTable.value(inter2)->countUsage==0){
            removeIntersectionPoint(inter1);
        }*/

        if(control!=MONOFIN_SURFACE_NO_CONTROL_POINT){
            _controlPointTable.value(control)->countUsage--;
            _controlPointTable.value(control)->whereUsed.removeAll(segmentKey);
        }
    }

    QList<int> Surface::getAllSegmentKeys(){
        return _segmentTable.keys();
    }

    QList<int> Surface::getAllIntersectionPointKeys(){
        return _intersectionPointTable.keys();
    }

    QList<int> Surface::getAllControlPointKeys(){
        return _controlPointTable.keys();
    }
} // namespace Data
