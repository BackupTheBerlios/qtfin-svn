#ifndef XMLLOADVISITOR_H
#define XMLLOADVISITOR_H

#include "LoadVisitor.h"
#include "xmlParser/xmlParser.h"

namespace Data{

    class Surface;
    class Profil;
    class ConfigFile;

    class XMLLoadVisitor : public LoadVisitor
    {
    private:
        XMLNode _mainNode;
    public:
        //XMLLoadVisitor();

        void load(std::string path);

        void visitSurface(Surface *sf);
        void visitProfil(Profil *pr);
        void visitConfigFile(ConfigFile * cf);
    };
}
#endif // XMLLOADVISITOR_H
