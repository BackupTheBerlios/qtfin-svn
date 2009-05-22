#ifndef LAYER_H
#define LAYER_H


namespace Data{

    class Layer
    {
    private:
        float _length, _height;
    public:
        /**
         * the constructor of layers
         *@param length a float, the  (between 0 and 1) between the length of the layer and the length of the fin
         *@param height a float, the  (between 0 and 1) between the height if the layer and the height of the fin
         */
        Layer(float length=0.0f, float height=0.0f): _length(length), _height(height) {}

        /************************
         * Getters & Setters    *
         ************************/

        /**
         * a getter on the length
         *@return a float, the length  of the layer
         */
        float getLength(){
            return _length;
        }

        /**
         * a getter on the height
         *@return a float, the height  of the layer
         */
        float getHeight(){
            return _height;
        }

        /**
         * a setter on the length
         *@param length a float, the new length
         */
        void setLength(float length){
            _length = length;
        }

        /**
         * a setter on the height
         *@param height a float, the new height
         */
        void setHeight(float height){
            _height = height;
        }
    };

} // namespace Data

#endif // LAYER_H
