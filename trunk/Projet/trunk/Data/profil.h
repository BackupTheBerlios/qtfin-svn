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
			
		/**
		 * create a profil using a ConfigFile for the observateur
		 *@param link a ConfigFile pointer corresponding to the file which will be notified of any change in the layers' organisation
		 */
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
		
        /**
			* return the value of the highest layer
         *@return a float, corresponding to the highest value found for the attibute height
         */
        float getMaxHeight();
		
        /**
			* return the value of the longest layer
         *@return a float, corresponding to the longest value found for the attibute length
         */
        float getMaxLength();
		
		
        /**
			* create a new history, erasing the older one
         *@param t Modification (see DataConstants.h), if the type of t correspondes to MonofinLayer, a new list of HistoryHolders is created
         */
        void startHistory(Modification t);
		
        /**
			* return the current list of HistoryHolders if the Modification is MonofinLayer
         * the attribute _MakedHistory is then replaced by a new list of HistoryHolders
         *@param t Modification (see DataConstants.h)
         *@return a pointer to the former _makedHistory. if t if not MonofinLayer, return an empty list of HistoryHolders
         */
        QList<HistoryHolder<Modification> *> retrieveHistory(Modification t);
		
        /**
			* performes an undo operation using the list of HistoryHolder
         * For each HistoryHolderin the list it is poped and a switch choose which function must be used to un-do the action
         *@param history a list of HistoryHolders which has to be of the MonofinLayer type
         */
        void undo(QList<HistoryHolder<Modification> *> history);
		
		
        /**
			* allows the SaveVisitor to explore the profil
         * Call the visiteProfil method of the visitor
         *@param v pointer to a Savevisitor, which is going to explore Profil
         */
        void accept(SaveVisitor * v);
		
        /**
			* allows the LoadVisitor to explore the profil
         * Call the visiteProfil method of the visitor
         *@param v pointer to a SaveVisitor, which is going to explore Profil
         */
        void accept(LoadVisitor * v);
		
		
        /**
			* Load a layer according to the parameters
         *@param rank an integer, the rank of the layer to load, the new layer is inserted on this rank, and move the upper strates to an higher rank
         *@param length a float, the length value of the new layer
         *@param height a float, the height value of the new layer
         */
        void loadLayer(int rank, float length, float height);
		
        /**
			* erase the past profil in order to perform a safe loading
         */
        void clearProfil();
		
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
