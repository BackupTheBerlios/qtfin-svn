#ifndef PROFIL_H
#define PROFIL_H

#include "stratum.h"
#include <vector>

class ConfigFile;

class Profil
{
private:
    //utilisation de vector pas thread safe
    //si soucis rflchir sur ce point.
    std::vector<Stratum> _monofinStratums;

    ConfigFile * _linkedConfigFile;

public:
    Profil(): _linkedConfigFile(NULL) {}

    Stratum * getStratum(int rang);

    int getHowManyStratums();

    void addStratum(int rang, Stratum s);

    void remStratum(int rang);

    void link(ConfigFile* toLink);

    void acceptVisitor(Visitor* v);

private:
    notifyCreate(int rang);

    notifyDelete(int rang);
};

#endif // PROFIL_H
