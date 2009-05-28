#ifndef LOADVISITOR_H
#define LOADVISITOR_H

#include <string>
#include <QString>
#include <QImage>

namespace Data{

    class Surface;
    class Profil;
    class ConfigFile;

    class LoadVisitor{

    public:

        virtual ~LoadVisitor() {}

        virtual void load(std::string path) = 0;
        virtual void loadForm(std::string path) = 0;
        virtual QImage getImage(std::string path) const = 0;

        virtual void visitSurface(Surface *sf) = 0;
        virtual void visitProfil(Profil *pr) = 0;
        virtual void visitConfigFile(ConfigFile * cf) = 0;

    };

}

#endif // LOADVISITOR_H
