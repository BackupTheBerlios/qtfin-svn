#include "segment.h"

namespace Data{

    /**
     * a constructor taking two parameters that must be filled and one optional
     *@param intersectionPointKey1 an integer that represent the key of an edge of the segment
     *@param intersectionPointKey2 an integer that represent the key of the other edge of the segment
     *@param controlPointKey an integer that represent the key of a control point (bezier curves), this one is optional
     */
    Segment::Segment(int intersectionPointKey1, int intersectionPointKey2,
            int controlPointKey): _intersectionPointKey1(intersectionPointKey1),
    _intersectionPointKey2(intersectionPointKey2), _controlPointKey(controlPointKey) {}

    /**
     * a getter on all attribute members
     *@param intersectionPointKey1 return an integer that represent the key of an edge of the segment
     *@param intersectionPointKey2 return an integer that represent the key of the other edge of the segment
     *@param controlPointKey return an integer that represent the key of a control point (bezier curves), may be MONOFIN_SURFACE_NO_CONTROL_POINT if no control point is specified
     */
    void Segment::getAll(int &intersectionPointKey1, int &intersectionPointKey2,
            int &controlPointKey){
        intersectionPointKey1 = _intersectionPointKey1;
        intersectionPointKey2 = _intersectionPointKey2;
        controlPointKey = _controlPointKey;
    }

    /**
     * a setter on all attribute members
     *@param intersectionPointKey1 an integer that represent the key of an edge of the segment
     *@param intersectionPointKey2 an integer that represent the key of the other edge of the segment
     *@param controlPointKey an integer that represent the key of a control point (bezier curves), may be MONOFIN_SURFACE_NO_CONTROL_POINT if no control point is specified
     */
    void Segment::setAll(int intersectionPointKey1, int intersectionPointKey2,
            int controlPointKey){
        _intersectionPointKey1 = intersectionPointKey1;
        _intersectionPointKey2 = intersectionPointKey2;
        _controlPointKey = controlPointKey;
    }

    /**
     * a getter on the first intersection point key (representing an edge of the segment)
     *@return an integer, the key of the intersection point
     */
    int Segment::getFirstIntersectionPointKey(){
        return _intersectionPointKey1;
    }

    /**
     * a setter on the first intersection point key (representing an edge of the segment)
     *@param firstIntersectionPointKey an integer, the key of the intersection point
     */
    void Segment::setFirstIntersectionPointKey(int firstIntersectionPointKey){
        _intersectionPointKey1 = firstIntersectionPointKey;
    }

    /**
     * a getter on the second intersection point key (representing an edge of the segment)
     *@return an integer, the key of the intersection point
     */
    int Segment::getSecondIntersectionPointKey(){
        return _intersectionPointKey2;
    }

    /**
     * a setter on the second intersection point key (representing an edge of the segment)
     *@param secondIntersectionPointKey an integer, the key of the intersection point
     */
    void Segment::setSecondIntersectionPointKey(int secondIntersectionPointKey){
        _intersectionPointKey2 = secondIntersectionPointKey;
    }

    /**
     * a getter on the control point key (bezier curves) may be MONOFIN_SURFACE_NO_CONTROL_POINT if no control point is specified
     *@return an integer, the key of the intersection point
     */
    int Segment::getControlPointKey(){
        return _controlPointKey;
    }

    /**
     * a setter on the control point key (bezier curves) may be MONOFIN_SURFACE_NO_CONTROL_POINT if no control point is specified
     *@param firstIntersectionPoint an integer, the key of the intersection point
     */
    void Segment::setControlPointKey(int controlPointKey){
        _controlPointKey = controlPointKey;
    }

} // namespace Data
