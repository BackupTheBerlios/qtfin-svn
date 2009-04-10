#ifndef PROFIL_H
#define PROFIL_H

#include "layer.h"
#include <QVector>

namespace Data{

    class ConfigFile;
    class MonofinFile;

    class Profil
    {
        friend class MonofinFile;
    private:

        QVector<Layer*> _monofinLayers;

        ConfigFile * _linkedConfigFile;

        float _monofinLength, _monofinHeight;

    public:
        Profil();

        ~Profil();
    private:

        /**
         * return a pointer to a Layer
         *@param rank an integer, the rank of the Layer we want
         *@return a pointer, the address to the correct layer, NULL if an incorrect rank is given
         */
        Layer * getLayer(int rank);

        /**
         * a getter on the number of layer we have
         *@return an integer, the number of layer created
         */
        inline int getHowManyLayers();

        /**
         * insert a layer at the given rank (displace if necessary other layers), if the rank is outside the current number of layer it will be placed at the last rank
         *@param rank an integer, where we want to insert the new Layer, a negative rank will insert the layer at the begining, a bigger rank than that exist actually will place the layer at the end
         *@param newLayer a Layer pointer, the layer to insert
         */
        void addLayer(int rank, Layer *newLayer);

        /**
         * destroy the layer at the given rank, if no there are no layer at this rank, nothing will be done
         *@param rank an integer, the rank of the layer we want to remove
         */
        void remLayer(int rank);

        /**
         * a setter on the length of the monofin
         *@param length a float, the length of the monofin
         */
        inline void setMonofinLength(float length);

        /**
         * a setter on the height of the monofin
         *@param height a float, the height of the monofin
         */
        inline void setMonofinHeight(float height);

        /**
         * a getter on the length of the monofin
         *@return a float, the length of the monofin
         */
        inline float getMonofinLength();

        /**
         * a getter on the height of the monofin
         *@return a float, the height of the monofin
         */
        float getMonofinHeight();

        //void acceptVisitor(Visitor* v);

    private:
        /**
         * this function is called by addLayer to inform ConfigFile that a new layer has been created
         *@param rank an integer, the rank of the new layer
         */
        inline void notifyCreate(int rank);

        /**
         * this function is called by remLayer to inform ConfigFile that a layer has been destroyed
         *@param rank an integer, the rank of the layer we want to delete
         */
        inline void notifyDelete(int rank);

        /**
         * a function called at the initialisation of the structure to linkthe profil and the configfile (which also manage layers)
         *@param toLink a pointer to ConfigFile, the address of the current ConfigFile
         */
        inline void link(ConfigFile* toLink);
    };

} // namespace Data

#endif // PROFIL_H
