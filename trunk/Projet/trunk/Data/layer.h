#ifndef LAYER_H
#define LAYER_H


namespace Data{

    class Layer
    {
    private:
        float _lengthRatio, _height;
    public:
        /**
          */
        Stratum(float length=0.0f, float height=0.0f): _length(length), _height(height) {}

        /************************
         * Getters & Setters    *
         ************************/

        float getLength(){
            return _length;
        }

        float getHeight(){
            return _height;
        }

        void setLength(float length){
            _length = length;
        }

        void setHeight(float heigth){
            _height = height;
        }
    };

} // namespace Data

#endif // LAYER_H
