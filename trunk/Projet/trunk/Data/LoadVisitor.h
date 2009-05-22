#ifndef LOADVISITOR_H
#define LOADVISITOR_H

#include <string>

namespace Data{

    class Surface;
    class Profil;
    class ConfigFile;

    class LoadVisitor{

    public:

        virtual ~LoadVisitor() {}

        virtual void load(std::string path) = 0;

        virtual void visitSurface(Surface *sf) = 0;
        virtual void visitProfil(Profil *pr) = 0;
        virtual void visitConfigFile(ConfigFile * cf) = 0;

    };

}

#endif // LOADVISITOR_H
