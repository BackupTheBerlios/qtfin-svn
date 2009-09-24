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
	
	/**
	* changes the value of young of a layerConfig
	 *@param young a float, the young value of the layerConfig
	 */
	void setYoung(float young){
		_young = young;
	}
	
	/**
	* changes the value of poisson of a layerConfig
	 *@param poisson a float, the poisson value of the layerConfig
	 */
	
	void setPoisson(float poisson){
		_poisson = poisson;
	}
	
	/**
	* changes the value of rho of a layerConfig
	 *@param rho a float, the rho value of the layerConfig
	 */
	void setRho(float rho){
		_rho = rho;
	}
	
	/**
	* gets the value of young of a layerConfig
	 *@return young a float, the young value of the layerConfig
	 */
	float getYoung(){
		return _young;
	}
	
	/**
	* gets the value of poisson of a layerConfig
	 *@return poisson a float, the poisson value of the layerConfig
	 */
	float getPoisson(){
		return _poisson;
	}
	
	/**
	* gets the value of rho of a layerConfig
	 *@return rho a float, the rho value of the layerConfig
	 */
	float getRho(){
		return _rho;
	}
};

} // namespace Data

#endif // LAYERCONFIG_H
