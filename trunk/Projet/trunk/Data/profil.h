#ifndef PROFIL_H
#define PROFIL_H

#include "layer.h"
#include "DataConstants.h"
#include "historyholder.h"
#include "SaveVisitor.h"
#include "LoadVisitor.h"
#include <QVector>

namespace Data{

    class ConfigFile;
    class MonofinFile;

    class Profil: public HistoryMaker<Modification>
    {
        friend class MonofinFile;
    private:

        QVector<Layer*> _monofinLayers;

        ConfigFile * _linkedConfigFile;


    public:
        Profil(ConfigFile * link);

        virtual ~Profil();

        /**
         * a getter on the number of layer we have
         *@return an integer, the number of layer created
         */
         int getHowManyLayers();

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

        float getMaxHeight();

        float getMaxLength();


        void startHistory(Modification t);

        QList<HistoryHolder<Modification> *> retrieveHistory(Modification t);

        void undo(QList<HistoryHolder<Modification> *> history);

        void accept(SaveVisitor * v);
        void accept(LoadVisitor * v);

        void loadLayer(int rank, float length, float height);

    private:

        /**
         * return a pointer to a Layer
         *@param rank an integer, the rank of the Layer we want
         *@return a pointer, the address to the correct layer, NULL if an incorrect rank is given
         */
        Layer * getLayer(int rank);

        /**
         * this function is called by addLayer to inform ConfigFile that a new layer has been created
         *@param rank an integer, the rank of the new layer
         */
         void notifyCreate(int rank);

        /**
         * this function is called by remLayer to inform ConfigFile that a layer has been destroyed
         *@param rank an integer, the rank of the layer we want to delete
         */
         void notifyDelete(int rank);

        /**
         * a function called at the initialisation of the structure to linkthe profil and the configfile (which also manage layers)
         *@param toLink a pointer to ConfigFile, the address of the current ConfigFile
         */
        // void link(ConfigFile* toLink);

    };

} // namespace Data

#endif // PROFIL_H
