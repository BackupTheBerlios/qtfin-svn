#ifndef SEGMENT_H
#define SEGMENT_H

#include "DataConstants.h"

namespace Data{

    class Segment
    {
    private:

        int _intersectionPointKey1, _intersectionPointKey2, _controlPointKey;

    public:

        /**
         * a constructor taking two parameters that must be filled and one optional
         *@param intersectionPointKey1 an integer that represent the key of an edge of the segment
         *@param intersectionPointKey2 an integer that represent the key of the other edge of the segment
         *@param controlPointKey an integer that represent the key of a control point (bezier curves), this one is optional
         */
        Segment(int intersectionPointKey1, int intersectionPointKey2,
                int controlPointKey = MONOFIN_SURFACE_NO_CONTROL_POINT);

        /**
         * Getters &  Setters
         */

        /**
         * a getter on all attribute members
         *@param intersectionPointKey1 return an integer that represent the key of an edge of the segment
         *@param intersectionPointKey2 return an integer that represent the key of the other edge of the segment
         *@param controlPointKey return an integer that represent the key of a control point (bezier curves), may be MONOFIN_SURFACE_NO_CONTROL_POINT if no control point is specified
         */
        void getAll(int &intersectionPointKey1, int &intersectionPointKey2,
                int &controlPointKey);

        /**
         * a setter on all attribute members
         *@param intersectionPointKey1 an integer that represent the key of an edge of the segment
         *@param intersectionPointKey2 an integer that represent the key of the other edge of the segment
         *@param controlPointKey an integer that represent the key of a control point (bezier curves), may be MONOFIN_SURFACE_NO_CONTROL_POINT if no control point is specified
         */
        void setAll(int intersectionPointKey1, int intersectionPointKey2,
                int controlPointKey = MONOFIN_SURFACE_NO_CONTROL_POINT);

        /**
         * a getter on the first intersection point key (representing an edge of the segment)
         *@return an integer, the key of the intersection point
         */
        int getFirstIntersectionPointKey();

        /**
         * a setter on the first intersection point key (representing an edge of the segment)
         *@param firstIntersectionPointKey an integer, the key of the intersection point
         */
        void setFirstIntersectionPointKey(int firstIntersectionPointKey);

        /**
         * a getter on the second intersection point key (representing an edge of the segment)
         *@return an integer, the key of the intersection point
         */
        int getSecondIntersectionPointKey();

        /**
         * a setter on the second intersection point key (representing an edge of the segment)
         *@param secondIntersectionPointKey an integer, the key of the intersection point
         */
        void setSecondIntersectionPointKey(int secondIntersectionPointKey);

        /**
         * a getter on the control point key (bezier curves) may be MONOFIN_SURFACE_NO_CONTROL_POINT if no control point is specified
         *@return an integer, the key of the intersection point
         */
        int getControlPointKey();

        /**
         * a setter on the control point key (bezier curves) may be MONOFIN_SURFACE_NO_CONTROL_POINT if no control point is specified
         *@param firstIntersectionPoint an integer, the key of the intersection point
         */
        void setControlPointKey(int controlPointKey = MONOFIN_SURFACE_NO_CONTROL_POINT);

    };

} // namespace Data

#endif // SEGMENT_H
