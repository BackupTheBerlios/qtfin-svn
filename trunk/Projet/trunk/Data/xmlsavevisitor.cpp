#include "xmlsavevisitor.h"
#include "surface.h"
#include "profil.h"
#include "configfile.h"
#include <sstream>
#include <fstream>
#include <QImage>
#include <QBuffer>
#include <QFile>

namespace Data{

    void XMLSaveVisitor::save(std::string path, std::string projectName){
        std::fstream file(path.c_str(),std::fstream::out);

        file << "<project name=\""<< projectName << "\" ver=\"1.0\">" << std::endl;
        file << "<monofin>" << std::endl;
        file << _surface;
        file << _profil;
        file << "</monofin>" << std::endl;
        file << _configFile;
        file << "</project>" << std::endl;

        file.close();
    }

    void XMLSaveVisitor::saveForm(std::string path, std::string projectName, QImage & picture){
        std::fstream file(path.c_str(),std::fstream::out);

        file << "<project name=\""<< projectName << "\" ver=\"1.0\">" << std::endl;
        file << "<monofin>" << std::endl;
        file << _surface;
        file << "<layers>" << std::endl;
        file << "</layers>" << std::endl;
        file << "</monofin>" << std::endl;
        file << "<config>" << std::endl;
        file << "<layerConfig>" << std::endl;
        file << "</layerConfig>" << std::endl;
        file << "</config>" << std::endl;

        QBuffer test;
        test.open(QIODevice::WriteOnly);
        picture.save(&test,"XBM");
        QString sauvegarde(test.data());
        file << "<picture data=\"" << sauvegarde.toStdString() << "\"/>" << std::endl;

        file << "</project>" << std::endl;

        file.close();

    }

    void XMLSaveVisitor::visitSurface(Surface *s){
        //enregistrement des segments
        std::stringstream os;
        os << "\t<segments>" << std::endl;
        foreach(int segKey, s->getAllSegmentKeys()){
            int interKey1, interKey2, controlKey;

            s->getSegment(segKey,interKey1,interKey2,controlKey);
            os << "\t\t<segment number=\"" << segKey << "\">" << std::endl;
            os << "\t\t\t<intersectionpoint number=\"" << interKey1 << "\"/>" << std::endl;
            os << "\t\t\t<intersectionpoint number=\"" << interKey2 << "\"/>" << std::endl;

            if(controlKey != MONOFIN_SURFACE_NO_CONTROL_POINT)
                os << "\t\t\t<controlpoint number=\"" << controlKey << "\"/>" << std::endl;
            os << "\t\t</segment>" << std::endl;
        }
        os << "\t</segments>" << std::endl;

        //enregistrement des intersections points
        os << "\t<points>" << std::endl;

        os << "\t\t<intersection>" << std::endl;
        foreach(int interKey, s->getAllIntersectionPointKeys()){
            float x, y;

            s->getIntersectionPoint(interKey,x,y);

            os << "\t\t\t<point number=\"" << interKey << "\" X=\""<< std::fixed << x << "\" Y=\"" << std::fixed << y <<"\"/>" << std::endl;
        }
        os << "\t\t</intersection>" << std::endl;

        os << "\t\t<control>" << std::endl;
        foreach(int controlKey, s->getAllControlPointKeys()){
            float x, y;

            s->getControlPoint(controlKey,x,y);

            os << "\t\t\t<point number=\"" << controlKey << "\" X=\""<< std::fixed << x << "\" Y=\"" << std::fixed << y <<"\"/>" << std::endl;
        }
        os << "\t\t</control>" << std::endl;

        os << "\t</points>" << std::endl;
        _surface = os.str();
    }

    void  XMLSaveVisitor::visitProfil(Profil * prof){

        std::stringstream os;



        os << "<layers>\n";



        int pos;

        for(pos = 0; pos<prof->getHowManyLayers(); pos++){

            float height = prof->getLayerHeight(pos);

            float length = prof->getLayerLength(pos);



            os << "\t<layer position= \""<< pos << "\" height= \""<< std::fixed << height <<"\" length= \""<< std::fixed << length <<"\" />\n";

        }

        os << "</layers>\n";

        _profil = os.str();

    }



    void  XMLSaveVisitor::visitConfigFile(ConfigFile * conf){

        std::stringstream os;

        os << "<config>\n";
        os << "\t<layerConfig>\n";



        int pos;

        for(pos = 0; pos<conf->getHowManyLayers(); pos++){

            float young = conf->getYoung(pos);

            float poisson = conf->getPoisson(pos);

            float rho = conf->getRho(pos);



            os << "\t\t<layer position= \""<< pos <<"\" young= \""<< std::fixed << young <<"\" poisson= \""<<
                    std::fixed << poisson <<"\" rho= \""<< std::fixed << rho <<"\" />\n";

        }

        os << "\t</layerConfig>\n";

        os << "</config>\n";



        _configFile = os.str();



    }
}
