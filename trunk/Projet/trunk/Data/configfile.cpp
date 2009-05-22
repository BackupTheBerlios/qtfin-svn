#include "configfile.h"
#include <iostream>

namespace Data{



    enum ConfigFileUndoRedoCode{SetYoung, SetPoisson, SetRho};


    ConfigFile::ConfigFile() {}

    ConfigFile::~ConfigFile(){

        int i = 0;

        for(i=0; i<_monofinLayerConfig.size(); i++){

            LayerConfig * toDelete = _monofinLayerConfig.back();
            _monofinLayerConfig.pop_back();

            delete toDelete;

        }

    }


    int ConfigFile::getHowManyLayers(){

        return _monofinLayerConfig.size();

    }


    void ConfigFile::updateCreate(int rank){

        LayerConfig * inter = new LayerConfig;

        addLayerConfig(inter, rank);

    }

    void ConfigFile::updateRemove(int rank){
        removeLayerConfig(rank);
    }

    void ConfigFile::addLayerConfig(LayerConfig * s, int rank){

        _monofinLayerConfig.insert(rank, s);

    }

    void ConfigFile::removeLayerConfig(int rank){

        LayerConfig * toDelete = _monofinLayerConfig.at(rank);
        _monofinLayerConfig.remove(rank);
        delete toDelete;
    }

    void ConfigFile::setYoung(int rank, float young){
        if(_monofinLayerConfig.capacity()){
            float *exvalue = new float;
            *exvalue = _monofinLayerConfig.value(rank)->getYoung();

            HistoryHolder<Modification> * current = getCurrentHistoryHolder(MonofinLayerConfig);

            current->pushPtr(exvalue);
            current->pushInt(rank);
            current->pushInt(SetYoung);

            _monofinLayerConfig[rank]->setYoung(young);
        }
    }

    void ConfigFile::setPoisson(int rank, float poisson){
        if(_monofinLayerConfig.capacity()){
            float * exvalue = new float;
            *exvalue = _monofinLayerConfig.value(rank)->getPoisson();

            HistoryHolder<Modification> * current = getCurrentHistoryHolder(MonofinLayerConfig);

            current->pushPtr(exvalue);
            current->pushInt(rank);
            current->pushInt(SetPoisson);

            _monofinLayerConfig[rank]->setPoisson(poisson);
        }
    }

    void ConfigFile::setRho(int rank, float rho){
        if(_monofinLayerConfig.capacity()){
            float *exvalue = new float;
            *exvalue = _monofinLayerConfig.value(rank)->getRho();

            HistoryHolder<Modification> * current = getCurrentHistoryHolder(MonofinLayerConfig);

            current->pushPtr(exvalue);
            current->pushInt(rank);
            current->pushInt(SetRho);

            _monofinLayerConfig[rank]->setRho(rho);
        }
    }

    float ConfigFile::getYoung(int rank){
        if(rank>=0 && rank <_monofinLayerConfig.size())
            return _monofinLayerConfig[rank]->getYoung();
        else
            return MONOFIN_CONFIGFILE_BAD_LAYER;
    }

    float ConfigFile::getPoisson(int rank){
        if(rank>=0 && rank <_monofinLayerConfig.size())
            return _monofinLayerConfig[rank]->getPoisson();
        else
            return MONOFIN_CONFIGFILE_BAD_LAYER;
    }

    float ConfigFile::getRho(int rank){
        if(rank>=0 && rank <_monofinLayerConfig.size())
            return _monofinLayerConfig[rank]->getRho();
        else
            return MONOFIN_CONFIGFILE_BAD_LAYER;
    }

    void ConfigFile::startHistory(Modification t){
        if (t==MonofinLayerConfig){
            _makedHistory = NULL;
        }
    }

    HistoryHolder<Modification> * ConfigFile::retrieveHistory(Modification t){
        if (t==MonofinLayerConfig)
            return _makedHistory;
        else
            return NULL;
    }

    void ConfigFile::undo(HistoryHolder<Modification> * history){
        if(history == NULL)
            return;

        if (history->getType()==MonofinLayerConfig){
        // if actions to undo are from us
            HistoryHolder<Modification> * historyReader = history;
            while(historyReader!=NULL){

                int code_action = (int)(historyReader->pop());

                switch(code_action){
                case SetYoung:{
                    int rank = (int) (historyReader->pop());
                    float * young = (float*)(historyReader->pop());
                    setYoung(rank, * young);
                    delete young;
                    break;
                }

                case SetPoisson:{
                    int rank = (int) (historyReader->pop());
                    float * poisson = (float*)(historyReader->pop());
                    setPoisson(rank, * poisson);
                    delete poisson;
                    break;
                }

                case SetRho:{
                    int rank = (int) (historyReader->pop());
                    float * rho = (float*)(historyReader->pop());
                    setRho(rank, * rho);
                    delete rho;
                    break;
                }

                default:
                    std::cout << "default : code : " << code_action << std::endl;
                    break;
                }

                //next action to undo
                HistoryHolder<Modification> * toDelete = historyReader;
                historyReader = historyReader->getNext();
                delete toDelete;
            }
        }
    }



    void ConfigFile::acceptVisitor(SaveVisitor * v){
        v->visitConfigFile(this);
    }
} // namespace Data
