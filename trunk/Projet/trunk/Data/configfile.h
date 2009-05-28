#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include <QVector>
#include "LayerConfig.h"
#include "DataConstants.h"
#include "historyholder.h"
#include "SaveVisitor.h"
#include "LoadVisitor.h"

namespace Data{
	
    class Profil;
    class LayerConfig;
	
    class ConfigFile: public HistoryMaker<Modification>
    {
        friend class Profil;
private:
        QVector<LayerConfig*> _monofinLayerConfig;
public:
        ConfigFile();
		
        /**
		* a getter on the number of layer we have
         *@return an integer, the number of layer created
         */
        int getHowManyLayers();
		
		
        virtual ~ConfigFile();
		
		
        /**
			* changes the value of young of a layerConfig
         *@param rank an integer, the position of the layerConfig
         *@param young a float, the young value of the layerConfig
         */
        void setYoung(int rank, float young);
		
        /**
			* changes the value of poisson of a layerConfig
         *@param rank an integer, the position of the layerConfig
         *@param poisson a float, the poisson value of the layerConfig
         */
        void setPoisson(int rank, float poisson);
		
        /**
			* changes the value of rho of a layerConfig
         *@param rank an integer, the position of the layerConfig
         *@param rho a float, the rho value of the layerConfig
         */
        void setRho(int rank, float rho);
		
        /**
			* gets the value of young of a layerConfig
         *@param rank an integer, the position of the layerConfig
         *@return a float, the young value of the layerConfig
         */
        float getYoung(int rank);
		
        /**
			* gets the value of poisson of a layerConfig
         *@param rank an integer, the position of the layerConfig
         *@return a float, the poisson value of the layerConfig
         */
        float getPoisson(int rank);
		
        /**
			* gets the value of rho of a layerConfig
         *@param rank an integer, the position of the layerConfig
         *@return a float, the rho value of the layerConfig
         */
        float getRho(int rank);
		
		/**
			* creates a new history, erasing the older one
         *@param t Modification (see DataConstants.h), if the type of t correspondes to MonofinLayerConfig, a new list of HistoryHolders is created
         */
        void startHistory(Modification t);
		
        /**
			* returns the current list of HistoryHolders if the Modification is MonofinLayerConfig
         * the attribute _MakedHistory is then replaced by a new list of HistoryHolders
         *@param t Modification (see DataConstants.h)
         *@return a pointer to the former _makedHistory. if t if not MonofinLayerConfig, return an empty list of HistoryHolders
         */
        QList<HistoryHolder<Modification> *> retrieveHistory(Modification t);
		
        /**
			* performs an undo operation using the list of HistoryHolder
         * For each HistoryHolderin the list it is poped and a switch choose which function must be used to un-do the action
         *@param history a list of HistoryHolders which has to be of the MonofinLayerConfig type
         */
        void undo(QList<HistoryHolder<Modification> *> history);
		
        /**
			* allows the SaveVisitor to explore the CinfigFile
         * Call the visiteProfil method of the visitor
         *@param v pointer to a Savevisitor, which is going to explore ConfigFile
         */
        void accept(SaveVisitor * v);
		
        /**
			* allows the LoadVisitor to explore the ConfigFile
         * Call the visiteProfil method of the visitor
         *@param v pointer to a Loadvisitor, which is going to explore ConfigFile
         */
        void accept(LoadVisitor * v);
		
        /**
			* loads a new LayerConfig according to the prameters
         *@param rank an integer, it define the position of the Layer in the table (can erase if the key is already allocated)
         *@param young a float, the young value
         *@param poisson a float, the poisson value
         *@param rho a float, the rho value
         */
        void loadLayerConfig(int rank, float young, float poisson, float rho);
		
        /**
			* clear all the structure (delete everything), undo/redo works on it
         */
        void clearConfigFile();
		
private:
			
			/**
			* adds a layer at the position parametred
			 * called from Profil
			 *@param rank an integer, the position of the new Layer
			 */
			void updateCreate(int rank);
		
        /**
			* removes a layer at the position parametred
         * called from Profil
         *@param rank an integer, the position of the Layer
         */
        void updateRemove(int rank);
		
        /**
			* adds a layer given at the position parametred
         * called from updateCreate
         *@param s a LayerConfig pointer, the LayerConfig to add
         *@param rank an integer, the position of the new Layer
         */
        void addLayerConfig(LayerConfig * s, int rank);
		
        /**
			* removes a layer at the position parametred
         * called from updateRemove
         *@param rank an integer, the position of the Layer
         */
        void removeLayerConfig(int rank);
    };
	
} // namespace Data

#endif // CONFIGFILE_H
