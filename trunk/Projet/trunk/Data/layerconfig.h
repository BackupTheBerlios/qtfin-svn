#ifndef LAYERCONFIG_H
#define LAYERCONFIG_H

namespace Data{

    class LayerConfig
    {
    private:
        float _young,_poisson,_rho;
    public:
        //LayerConfig();

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

} // namespace Data

#endif // LAYERCONFIG_H
