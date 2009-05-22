#include "xmlsavevisitor.h"
#include "surface.h"
#include "profil.h"
#include "configfile.h"
#include <sstream>
#include <fstream>

namespace Data{

    void XMLSaveVisitor::save(std::string path, std::string projectName){
        std::stringstream os;
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

    void XMLSaveVisitor::visitSurface(Surface *s){
        //enregistrement des segments
        std::stringstream os;
        os << "\t<segments>" << std::endl;
        foreach(int segKey, s->getAllSegmentKeys()){
            int interKey1, interKey2, controlKey;

            s->getSegment(segKey,interKey1,interKey2,controlKey);
            os << "\t\t<segment number=\"" << segKey << "\">" << std::endl;
            os << "\t\t\t<intersectionpoint number=\"" << interKey1 << "\"/>" << std::endl;
            os << "\t\t\t<intersectionpoint number=\"" << interKey1 << "\"/>" << std::endl;

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

            os << "\t\t\t<point number=\"" << interKey << "\" X=\""<< x << "\" Y=\"" << y <<"\"/>" << std::endl;
        }
        os << "\t\t</intersection>" << std::endl;

        os << "\t\t<control>" << std::endl;
        foreach(int controlKey, s->getAllControlPointKeys()){
            float x, y;

            s->getIntersectionPoint(controlKey,x,y);

            os << "\t\t\t<point number=\"" << controlKey << "\" X=\""<< x << "\" Y=\"" << y <<"\"/>" << std::endl;
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

        int height = prof->getLayerHeight(pos);

        int length = prof->getLayerLength(pos);



        os << "\t<layer position= \""<< pos << "\" height= \""<< height <<"\" length= \""<< length <<"\" />\n";

    }

    os << "<layers/>\n";

    _profil = os.str();

}



void  XMLSaveVisitor::visitConfigFile(ConfigFile * conf){

    std::stringstream os;

    os << "<config>\n";
    os << "\t<layerConfig>\n";



    int pos;

    for(pos = 0; pos<conf->getHowManyLayers(); pos++){

        int young = conf->getYoung(pos);

        int poisson = conf->getPoisson(pos);

        int rho = conf->getRho(pos);



        os << "\t\t<layer position= \""<< pos <<"\" young= \""<< young <<"\" poisson= \""<< poisson <<"\" rho= \""<< rho <<"\" />\n";

    }

    os << "\t<layerConfig/>\n";

    os << "<Config/>\n";



    _configFile = os.str();



}
}
