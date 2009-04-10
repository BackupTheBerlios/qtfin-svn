#ifndef LAYER_H
#define LAYER_H


namespace Data{

    class Layer
    {
    private:
        float _lengthRatio, _heightRatio;
    public:
        /**
         * the constructor of layers
         *@param lengthRatio a float, the ratio (between 0 and 1) between the length of the layer and the length of the fin
         *@param heightRatio a float, the ratio (between 0 and 1) between the height if the layer and the height of the fin
         */
        Layer(float lengthRatio=0.0f, float heightRatio=0.0f): _lengthRatio(lengthRatio), _heightRatio(heightRatio) {}

        /************************
         * Getters & Setters    *
         ************************/

        /**
         * a getter on the length ratio
         *@return a float, the length ratio of the layer
         */
        float getLengthRatio(){
            return _lengthRatio;
        }

        /**
         * a getter on the height ratio
         *@return a float, the height ratio of the layer
         */
        float getHeightRatio(){
            return _heightRatio;
        }

        /**
         * a setter on the length ratio
         *@param lengthRatio a float, the new length ratio
         */
        void setLengthRatio(float lengthRatio){
            _lengthRatio = lengthRatio;
        }

        /**
         * a setter on the height ratio
         *@param heightRatio a float, the new height ratio
         */
        void setHeightRatio(float heightRatio){
            _heightRatio = heightRatio;
        }
    };

} // namespace Data

#endif // LAYER_H
