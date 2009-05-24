#ifndef XMLLOADVISITOR_H
#define XMLLOADVISITOR_H

#include "LoadVisitor.h"
//#include "xmlparser/xmlParser.h"
#include <QDomElement>
#include <QDomDocument>

namespace Data{

    class Surface;
    class Profil;
    class ConfigFile;

    class XMLLoadVisitor : public LoadVisitor
    {
    private:
        QDomDocument _mainDocument;
        QDomElement _mainNode;
    public:
        //XMLLoadVisitor();

        void load(std::string path);

        void visitSurface(Surface *sf);
        void visitProfil(Profil *pr);
        void visitConfigFile(ConfigFile * cf);
    };
}
#endif // XMLLOADVISITOR_H
