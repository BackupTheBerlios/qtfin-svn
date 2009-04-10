#include "profil.h"
#include "configfile.h"

namespace Data{

    Profil::Profil(): _linkedConfigFile(NULL) {}

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

        Layer * toDelete = _monofinLayers.value(rank);
        _monofinLayers.remove(rank);
        delete toDelete;
        notifyDelete(rank);
    }

    /*void Profil::acceptVisitor(Visitor* v){

    }*/

    /**
     * a setter on the length of the monofin
     *@param length a float, the length of the monofin
     */
    inline void Profil::setMonofinLength(float length){
        _monofinLength = length;
    }

    /**
     * a setter on the height of the monofin
     *@param height a float, the height of the monofin
     */
    void Profil::setMonofinHeight(float height){
        _monofinHeight = height;
    }

    /**
     * a getter on the length of the monofin
     *@return a float, the length of the monofin
     */
    float Profil::getMonofinLength() {
        return _monofinLength;
    }

    /**
     * a getter on the height of the monofin
     *@return a float, the height of the monofin
     */
    float Profil::getMonofinHeight(){
        return _monofinHeight;
    }

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
     * a function called at the initialisation of the structure to linkthe profil and the configfile (which also manage layers)
     *@param toLink a pointer to ConfigFile, the address of the current ConfigFile
     */
    void Profil::link(ConfigFile* toLink){
        _linkedConfigFile = toLink;
    }

} // namespace Data
