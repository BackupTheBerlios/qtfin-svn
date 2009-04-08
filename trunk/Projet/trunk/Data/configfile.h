#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include <QVector>
#include "LayerConfig.h"

namespace Data{

    class Profil;

    class ConfigFile
    {
        friend class Profil;
    private:
        QVector<LayerConfig> _monofinLayerConfig;
    public:
        ConfigFile();

        LayerConfig& getLayerConfig(int rank){
            return _monofinStratumConfig.at(rank);
        }

    private:

        void updateCreate(int rank);

        void updateRemove(int rank);

        void addLayerConfig(StrateConfig& s, int rank);

        void removeStrateConfig(int rank);
    };

} // namespace Data

#endif // CONFIGFILE_H
