#ifndef PROJECTFILE_H
#define PROJECTFILE_H

#include "DataConstants.h"
#include "historyholder.h"
#include <QString>
#include <QImage>

namespace Data{

    class MonofinFile;
    class ConfigFile;

    class ProjectFile : public HistoryCareTaker<Modification>
    {
    private:
        MonofinFile *_monofinGeometry;
        ConfigFile *_monofinPhysicalProperties;

    public:
        ProjectFile();

        ~ProjectFile();

        /*****************************
         *      modifying datas      *
         *****************************/

        /**
         * adds a layer on designated position
         *@param rank an integer, the position where to insert the layer
         *@param length a float, the length of the layer
         *@param height a float, the height of the layer
         */
        void addLayer(int rank, float length, float height);

        /**
         * changes the value of the length of a layer
         *@param rank an integer, the position of the layer
         *@param length a float, the length of the layer
         */
        void setLayerLength(int rank, float length);

        /**
         * changes the value of the length of a layer
         *@param rank an integer, the position of the layer
         *@param length a float, the height of the layer
         */
        void setLayerHeight(int rank, float height);

        /**
         * removes the designated layer
         *@param rank an integer, the position of the layer
         */
        void removeLayer(int rank);

        /**
         * changes the value of rho of a layerConfig
         *@param rank an integer, the position of the layerConfig
         *@param rho a float, the rho value of the layerConfig
         */
        void setLayerConfigRho(int rank, float rho);

        /**
         * changes the value of young of a layerConfig
         *@param rank an integer, the position of the layerConfig
         *@param young a float, the young value of the layerConfig
         */
        void setLayerConfigYoung(int rank, float young);

        /**
         * changes the value of poisson of a layerConfig
         *@param rank an integer, the position of the layerConfig
         *@param poisson a float, the poisson value of the layerConfig
         */
        void setLayerConfigPoisson(int rank, float poisson);

        /**
         * inserts a segment
         * chooses a correct key
         *@param intersectionPointKey1 an integer, it is the reference in the point table to the first point defining the segment
         *@param intersectionPointKey2 an integer, it is the reference in the point table to the second point defining the segment
         *@param controlPointKey an integer, it refers to the controle point parametring the Bezier curve
         */
        int addSegment(int intersectionPoint1Key, int intersectionPoint2Key, int controlPointKey);

        /**
         * inserts a controlPoint
         * chooses a corect key
         *@param x a float, the x coordinate
         *@param y a float, the y coordinate
         */
        int addControlPoint(float x, float y);

        /**
         * inserts an intersectionPoint
         * chooses a corect key
         *@param x a float, the x coordinate
         *@param y a float, the y coordinate
         */
        int addIntersectionPoint(float x, float y);

        /**
         * cut the designated segment in two using the designated point
         * the orignal segment's key is kept for the first of the two segments
         * chooses a corect key for the second segment
         *@param segmentKey an integer, the segment's key
         *@param intersectionPointKey an integer, the intersectionPoint's key
         *@return an integer, the key of the second segment
         */
        int subdivideSegment(int segmentKey, int intersectionPointKey);

        /**
         * removes an controlPoint
         *@param controlPointKey an integer, the controlPoint's key
         */
        void removeControlPoint(int controlPointKey);

        /**
         * removes an intersectionPoint
         *@param intersectionPointKey an integer, the intersectionPoint's key
         */
        int removeIntersectionPoint(int intersectionPointKey);

        /**
         * changes the values of a intersectionPoint's cordinates
         *@param intersectionPointKey an integer, the intersectionPoint's key
         *@param x a float, the x coordinate
         *@param y a float, the y coordinate
         */
        void setIntersectionPoint(int intersectionPointKey, float x, float y);

         /**
         * changes the values of a controlPoint's cordinates
         *@param controlPointKey an integer, the controlPoint's key
         *@param x a float, the x coordinate
         *@param y a float, the y coordinate
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

        /****************************
         *     Consulting datas     *
         ****************************/


        /**
         * gets the designated layer's length
         *@param rank an integer, the rank of the layer
         *@return a float, the length
         */
        float getLayerLength(int rank) const;

        /**
         * gets the designated layer's height
         *@param rank an integer, the rank of the layer
         *@return a float, the height
         */
        float getLayerHeight(int rank) const;

        /**
         * return the value of the longest layer
         *@return a float, corresponding to the longest value found for the attibute length
         */
        float getLayerMaxLength() const;

        /**
         * return the value of the highest layer
         *@return a float, corresponding to the highest value found for the attibute length
         */
        float getLayerMaxHeight() const;


        /**
         * a getter on the number of layer we have
         *@return an integer, the number of layer created
         */
        int getHowManyLayers() const;


        /**
         * gets the value of rho of a layerConfig
         *@param rank an integer, the position of the layerConfig
         *@return a float, the rho value of the layerConfig
         */
        float getLayerConfigRho(int rank) const;

        /**
         * gets the value of young of a layerConfig
         *@param rank an integer, the position of the layerConfig
         *@return a float, the young value of the layerConfig
         */
        float getLayerConfigYoung(int rank) const;

        /**
         * gets the value of poisson of a layerConfig
         *@param rank an integer, the position of the layerConfig
         *@return a float, the poisson value of the layerConfig
         */
        float getLayerConfigPoisson(int rank) const;

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
         * returns the list of all the keys of all the segments
         *@return a list of int, the keys of all the segments
         */
        QList<int> getAllSegmentKeys();

        /**
         * returns the list of all the keys of all the intersectionPoints
         *@return a list of int, the keys of all the intersectionPoints
         */
        QList<int> getAllIntersectionPointKeys();

        /**
         * returns the list of all the keys of all the controlPoints
         *@return a list of int, the keys of all the controlPoints
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

        /***************
         * Load & Save *
         ***************/

        /**
          * save the entire project using SaveVisitor
          * create a XML save file
          *@param path a string pointer, the position where to save
          *@param projectName, the name given to the save file (deffault : untitled)
          */
        void saveProject(const QString &path, const QString &projectName = QString("Untitled"));

        /**
          * save the surface information only
          * create a XML save file for the shape library
          *@param path a string pointer, the position where to save
          *@param projectName, the name given to the save file (default : untitled)
          *@param picture a QImage, an aperçu of the shape saved (default : empty)
          */
        void saveForm(const QString &path, const QString &projectName = QString("Untitled"), QImage picture = QImage());

        /**
          * load the entire project using LoadVisitor
          * use a XML save file
          *@param path a string pointer, the position of the file to load
          *@return a string, "none" when its done
          */
        QString loadProject(const QString &path);

        /**
          * load the surface information only
          * use a XML save file for the shape library
          * add the shade loaded to the active structure (erase if it is not empty)
          *@param path a string pointer, the position of the file to load
          */
        void loadForm(const QString &path);

        /**
          * get an aperçu from a path
          *@param path a string pointer, the position of image
          *@return a QImage for the aperçu
          */
        static QImage getImage(const QString &path);
    };

} // namespace Data

#endif // PROJECTFILE_H
