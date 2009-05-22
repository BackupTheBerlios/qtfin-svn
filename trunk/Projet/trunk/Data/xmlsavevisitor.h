#ifndef XMLSAVEVISITOR_H
#define XMLSAVEVISITOR_H
#include "SaveVisitor.h"

namespace Data{

    class XMLSaveVisitor : public SaveVisitor
    {
    private:
        std::string _surface;
        std::string _profil;
        std::string _configFile;
    public:
        void save(std::string path, std::string projectName);

        void visitSurface(Surface *s);
        void visitProfil(Profil *p);
        void visitConfigFile(ConfigFile *cf);
    };

}
#endif // XMLSAVEVISITOR_H
