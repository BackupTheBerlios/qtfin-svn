#ifndef PROFIL_H
#define PROFIL_H

#include "layer.h"
#include <QVector>

namespace Data{

    class ConfigFile;

    class Profil
    {
    private:

        QVector<Layer> _monofinLayers;

        ConfigFile * _linkedConfigFile;

    public:
        Profil(): _linkedConfigFile(NULL) {}

        Layer * getLayer(int rank);

        int getHowManyLayers();

        void addLayer(int rang, Layer s);

        void remLayer(int rank);

        void link(ConfigFile* toLink);

        void acceptVisitor(Visitor* v);

    private:
        notifyCreate(int rank);

        notifyDelete(int rank);
    };

} // namespace Data

#endif // PROFIL_H
