#ifndef CONFIGFILE_H
#define CONFIGFILE_H

#include <vector>
#include "StrateConfig.h"

class Profil;

class ConfigFile
{
    friend class Profil;
private:
    std::vector<StrateConfig> _monofinStratumConfig;
public:
    ConfigFile();

    StrateConfig& getStrateConfig(int rang){
        return _monofinStratumConfig.at(rang);
    }

private:

    void updateCreate(int rang);

    void updateRemove(int rang);

    void addStrateConfig(StrateConfig& s, int i);

    void removeStrateConfig(int rang);
};

#endif // CONFIGFILE_H
