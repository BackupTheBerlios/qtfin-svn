#ifndef SAVEVISITOR_H
#define SAVEVISITOR_H
#include <string>

namespace Data{

    class Surface;
    class Profil;
    class ConfigFile;

    class SaveVisitor{
    public:

        virtual ~SaveVisitor(){}

        virtual void save(std::string path, std::string projectName) = 0;

        virtual void visitSurface(Surface * s) = 0;
        virtual void visitProfil(Profil * p) = 0;
        virtual void visitConfigFile(ConfigFile * cf) = 0;

    };

}

#endif // SAVEVISITOR_H
