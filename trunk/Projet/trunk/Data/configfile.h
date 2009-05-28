#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include <QVector>
#include "layerconfig.h"
#include "DataConstants.h"
#include "historyholder.h"
#include "SaveVisitor.h"
#include "LoadVisitor.h"

namespace Data{

    class Profil;
    class LayerConfig;

    class ConfigFile: public HistoryMaker<Modification>
    {
        friend class Profil;
    private:
        QVector<LayerConfig*> _monofinLayerConfig;
    public:
        ConfigFile();


        int getHowManyLayers();


        virtual ~ConfigFile();

        void setYoung(int rank, float young);

        void setPoisson(int rank, float poisson);

        void setRho(int rank, float rho);

        float getYoung(int rank);

        float getPoisson(int rank);

        float getRho(int rank);

        void startHistory(Modification t);

        QList<HistoryHolder<Modification> *> retrieveHistory(Modification t);

        void undo(QList<HistoryHolder<Modification> *> history);



        void accept(SaveVisitor * v);
        void accept(LoadVisitor * v);

        void loadLayerConfig(int rank, float young, float poisson, float rho);

        void clearConfigFile();

    private:

        void updateCreate(int rank);

        void updateRemove(int rank);

        void addLayerConfig(LayerConfig * s, int rank);

        void removeLayerConfig(int rank);
    };

} // namespace Data

#endif // CONFIGFILE_H
