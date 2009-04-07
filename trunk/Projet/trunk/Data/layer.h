#ifndef STRATUM_H
#define STRATUM_H

class Stratum
{
private:
    float _length, _height;
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

#endif // STRATUM_H
