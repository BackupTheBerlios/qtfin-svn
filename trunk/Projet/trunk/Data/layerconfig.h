#ifndef STRATUMCONFIG_H
#define STRATUMCONFIG_H

class StratumConfig
{
private:
    float _young,_poisson,_rho;
public:
    StratumConfig();

    /*********************
     * Getters & Setters *
     *********************/

    void setYoung(float young){
        _young = young;
    }

    void setPoisson(float poisson){
        _poisson = poisson;
    }

    void setRho(float rho){
        _rho = rho;
    }

    float getYoung(){
        return _young;
    }

    float getPoisson(){
        return _poisson;
    }

    float getRho(){
        return _rho;
    }
};

#endif // STRATUMCONFIG_H
