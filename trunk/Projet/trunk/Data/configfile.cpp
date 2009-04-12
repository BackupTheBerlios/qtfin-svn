#include "configfile.h"

namespace Data{

    ConfigFile::ConfigFile() {}

    ConfigFile::~ConfigFile(){}

    void ConfigFile::updateCreate(int rank){}

    void ConfigFile::updateRemove(int rank){}

    void ConfigFile::addLayerConfig(LayerConfig * s, int rank){}

    void ConfigFile::removeStrateConfig(int rank){}

    void ConfigFile::setYoung(int rank, float young){

    }

    void ConfigFile::setPoisson(int rank, float poisson){

    }

    void ConfigFile::setRho(int rank, float rho){

    }

    float ConfigFile::getYoung(int rank){
        return 0;
    }

    float ConfigFile::getPoisson(int rank){
        return 0;
    }

    float ConfigFile::getRho(int rank){
        return 0;
    }

    void ConfigFile::startHistory(Modification t){

    }

    HistoryHolder<Modification> * ConfigFile::retrieveHistory(Modification t){
        return NULL;
    }

    void ConfigFile::undo(HistoryHolder<Modification> * history){

    }
} // namespace Data
