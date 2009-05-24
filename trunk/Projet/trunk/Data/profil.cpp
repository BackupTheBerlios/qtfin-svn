#include "configfile.h"
#include "layerconfig.h"
#include "profil.h"
#include <iostream>

namespace Data{


    enum ProfilUndoRedoCode{AddLayer, RemLayer, SetLayerLength, SetLayerHeight};


    Profil::Profil(ConfigFile * link) : _linkedConfigFile(link) {}

    Profil::~Profil(){
        while(!_monofinLayers.empty()){
            Layer * toDelete = _monofinLayers.back();
            _monofinLayers.pop_back();
            delete toDelete;
        }
    }

    /**
     * return a pointer to a Layer
     *@param rank an integer, the rank of the Layer we want
     *@return a pointer, the address to the correct layer, NULL if an incorrect rank is given
     */
    Layer * Profil::getLayer(int rank){

        if (rank<0 || rank>=_monofinLayers.size())
            return NULL;

        return _monofinLayers.value(rank);
    }

    /**
     * a getter on the number of layer we have
     *@return an integer, the number of layer created
     */
    int Profil::getHowManyLayers(){
        return _monofinLayers.size();
    }

    /**
     * insert a layer at the given rank (displace if necessary other layers), if the rank is outside the current number of layer it will be placed at the last rank
     *@param rank an integer, where we want to insert the new Layer, a negative rank will insert the layer at the begining, a bigger rank than that exist actually will place the layer at the end
     *@param newLayer a Layer pointer, the layer to insert
     */
    void Profil::addLayer(int rank, Layer *newLayer){



        rank = rank<0 ? 0:rank;

        rank = rank>=_monofinLayers.size() ? _monofinLayers.size():rank;



        HistoryHolder<Modification> * current = getCurrentHistoryHolder(MonofinLayer);



        current->pushInt(rank);

        current->pushInt(AddLayer);


        _monofinLayers.insert(rank,newLayer);
        notifyCreate(rank);
    }

    /**
     * destroy the layer at the given rank, if no there are no layer at this rank, nothing will be done
     *@param rank an integer, the rank of the layer we want to remove
     */
    void Profil::remLayer(int rank){
        if (rank<0 || rank>=_monofinLayers.size())
            return;

        Layer * exlayer = _monofinLayers.value(rank);



        HistoryHolder<Modification> * current = getCurrentHistoryHolder(MonofinLayer);



        current->pushPtr(exlayer);

        current->pushInt(rank);

        current->pushInt(RemLayer);


        _monofinLayers.remove(rank);
        notifyDelete(rank);
    }

    /*void Profil::acceptVisitor(Visitor* v){

    }*/


    /**
     * this function is called by addLayer to inform ConfigFile that a new layer has been created
     *@param rank an integer, the rank of the new layer
     */
    void Profil::notifyCreate(int rank){
        _linkedConfigFile->updateCreate(rank);
    }

    /**
     * this function is called by remLayer to inform ConfigFile that a layer has been destroyed
     *@param rank an integer, the rank of the layer we want to delete
     */
    void Profil::notifyDelete(int rank){
        _linkedConfigFile->updateRemove(rank);
    }

    /**
     * a setter on the length  of the layer at the given rank
     *@param rank an integer, the rank of the layer to modify, if it's an incorrect rank, nothing will be done
     *@param length a float, the new length
     */
    void Profil::setLayerLength(int rank, float length){
        if(getLayer(rank)!=NULL){



            float * exvalue = new float;
            *exvalue = getLayer(rank)->getLength();



            HistoryHolder<Modification> * current = getCurrentHistoryHolder(MonofinLayer);




            current->pushPtr(exvalue);

            current->pushInt(rank);

            current->pushInt(SetLayerLength);


            getLayer(rank)->setLength(length);
        }
    }

    /**
     * a setter on the heigth  of the layer at the given rank
     *@param height a float, the new heigtht
     */
    void Profil::setLayerHeight(int rank, float height){
        if(getLayer(rank)!=NULL){



            float *exvalue = new float;
            *exvalue = getLayer(rank)->getHeight();



            HistoryHolder<Modification> * current = getCurrentHistoryHolder(MonofinLayer);



            current->pushPtr(exvalue);

            current->pushInt(rank);

            current->pushInt(SetLayerHeight);


            getLayer(rank)->setHeight(height);
        }
    }

    /**
     * a getter on the length  of the ranked layer
     *@param rank an integer, the rank of the layer we want the length
     *@return a float, the length  of the layer, MONOFIN_PROFIL_BAD_RANK if rank don't exist
     */
    float Profil::getLayerLength(int rank){
        if (getLayer(rank)==NULL)
            return MONOFIN_PROFIL_BAD_LAYER;

        return getLayer(rank)->getLength();
    }

    /**
     * a getter on the height  of the ranked layer
     *@param rank an integer, the rank of the layer we want the height
     *@return a float, the height  of the layer, MONOFIN_PROFIL_BAD_RANK if rank don't exist
     */
    float Profil::getLayerHeight(int rank){
        if (getLayer(rank)==NULL)
            return MONOFIN_PROFIL_BAD_LAYER;

        return getLayer(rank)->getHeight();
    }


    void Profil::startHistory(Modification t){

        if (t==MonofinLayer){
            if(!_makedHistory.isEmpty()){
                foreach(HistoryHolder<Modification> * toDelete, _makedHistory)
                    delete toDelete;
            }
            _makedHistory = QList<HistoryHolder<Modification> *>();
        }
    }

    QList<HistoryHolder<Modification> *> Profil::retrieveHistory(Modification t){
        if (t==MonofinLayer){
            QList<HistoryHolder<Modification> *> elReturn = _makedHistory;
            _makedHistory = QList<HistoryHolder<Modification> *>();
            return elReturn;
        }else{
            return QList<HistoryHolder<Modification> *>();
        }
    }

    void Profil::undo(QList<HistoryHolder<Modification> *> history){
        if(history.isEmpty())
            return;

        if (history.first()->getType()==MonofinLayer){
        // if actions to undo are from us
            while(!history.isEmpty()){
                HistoryHolder<Modification> * historyReader = history.first();
                history.removeFirst();
                int code_action = (int)(historyReader->pop());
                switch(code_action){
                case AddLayer:{
                    remLayer((int)(historyReader->pop()));
                    break;
                }
                case RemLayer:{
                    int rank = (int)(historyReader->pop());
                    Layer * lay = (Layer *)historyReader->pop();

                    addLayer(rank, lay);
                    break;
                }
                case SetLayerLength:{
                    int rank = (int)(historyReader->pop());
                    float * value = (float *) (historyReader->pop());
                    setLayerLength(rank,*value);
                    delete value;
                    break;
                }
                case SetLayerHeight:{
                    int rank = (int)(historyReader->pop());
                    float * value = (float *) (historyReader->pop());

                    setLayerHeight(rank,*value);
                    delete value;
                    break;
                }

                default:
                    std::cout << "default : code : " << code_action << std::endl;
                    break;
                }

                //next action to undo
                delete historyReader;
            }
        }
    }

   void Profil::accept(SaveVisitor * v){

        v->visitProfil(this);
    }

   void Profil::accept(LoadVisitor * v){

        v->visitProfil(this);

    }

   void Profil::loadLayer(int rank, float length, float height){
       Layer * newLayer = new Layer(length,height);
       _monofinLayers.insert(rank,newLayer);
   }

   float Profil::getMaxHeight(){
       float len = 0.0f;
       foreach(Layer* lay, _monofinLayers)
           if (lay->getLength() > len)
               len = lay->getLength();
       return len;
   }

   float Profil::getMaxLength(){
        float hei;
        foreach(Layer* lay, _monofinLayers)
           if (lay->getHeight() > hei)
               hei = lay->getHeight();
       return hei;
   }

} // namespace Data
