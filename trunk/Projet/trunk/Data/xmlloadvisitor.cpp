#include "xmlloadvisitor.h"
#include "surface.h"
#include "profil.h"
#include "configfile.h"
#include <sstream>
#include <iostream>

namespace Data{
    void XMLLoadVisitor::load(std::string path){
        _mainNode = XMLNode::openFileHelper(path.c_str(),"project");
        std::cout << "ICI" << std::endl;
    }

    void XMLLoadVisitor::visitSurface(Surface *sf){
        XMLNode surface = _mainNode.getChildNode("monofin");
        //chargement des points intersections
        XMLNode inter = surface.getChildNode("points").getChildNode("intersection");
        //nombre de points intersections
        int nbInter = inter.nChildNode("point");
        for (int i = 0; i <nbInter; ++i){
            int key;
            float x,y;
            std::stringstream entry;
            entry.str(inter.getChildNode("point",i).getAttribute("number"));
            entry >> key;
            entry.clear();
            entry.str(inter.getChildNode("point",i).getAttribute("X"));
            entry >> x;
            entry.clear();
            entry.str(inter.getChildNode("point",i).getAttribute("Y"));
            entry >> y;
            entry.clear();
            sf->loadIntersectionPoint(key,x,y);
        }

        //chargement des points de controle
        XMLNode control = surface.getChildNode("points").getChildNode("control");
        int nbControl = control.nChildNode("point");
        for (int i = 0; i <nbControl; ++i){
            int key;
            float x,y;
            std::stringstream entry;
            entry.str(control.getChildNode("point",i).getAttribute("number"));
            entry >> key;
            entry.clear();
            entry.str(control.getChildNode("point",i).getAttribute("X"));
            entry >> x;
            entry.clear();
            entry.str(control.getChildNode("point",i).getAttribute("Y"));
            entry >> y;
            entry.clear();
            sf->loadControlPoint(key,x,y);
        }

        //chargement des segments
        XMLNode segments = surface.getChildNode("segments");
        int nbSeg = segments.nChildNode("segment");
        for(int i = 0; i < nbSeg; ++i){
            int segKey,interKey[2],controlKey;
            std::stringstream entry;
            entry.str(segments.getChildNode("segment",i).getAttribute("number"));
            entry >> segKey;
            entry.clear();
            for(int j=0;j<2;++j){
                entry.str(segments.getChildNode("segment",i).getChildNode("intersectionpoint",j).getAttribute("number"));
                entry >> interKey[j];
                entry.clear();
            }
            if (segments.getChildNode("segment",i).nChildNode("controlpoint")>0){
                entry.str(segments.getChildNode("segment",i).getChildNode("controlpoint").getAttribute("number"));
                entry >> controlKey;
                entry.clear();
            }else
                controlKey = Data::MONOFIN_SURFACE_NO_CONTROL_POINT;
            std::cout << segKey << " " << interKey[0] << " " << interKey[1] << " " << controlKey << std::endl;
            sf->loadSegment(segKey,interKey[0],interKey[1],controlKey);
        }

    }

    void XMLLoadVisitor::visitProfil(Profil *pr){
        //chargement des layers
        XMLNode layers = _mainNode.getChildNode("monofin").getChildNode("layers");
        int nbLayer =  layers.nChildNode("layer");
        for(int i = 0; i< nbLayer; ++i){
            int position;
            float height,length;
            std::stringstream entry;

            entry.str(layers.getChildNode("layer",i).getAttribute("position"));
            entry >> position;
            entry.clear();
            entry.str(layers.getChildNode("layer",i).getAttribute("height"));
            entry >> height;
            entry.clear();
            entry.str(layers.getChildNode("layer",i).getAttribute("length"));
            entry >> length;
            entry.clear();

            pr->loadLayer(position,length, height);

        }

    }

    void XMLLoadVisitor::visitConfigFile(ConfigFile * cf){
        //chargement des layers
        XMLNode layers = _mainNode.getChildNode("config").getChildNode("layerConfig");
        int nbLayer =  layers.nChildNode("layer");
        for(int i = 0; i< nbLayer; ++i){
            int position;
            float poisson,rho,young;
            std::stringstream entry;

            entry.str(layers.getChildNode("layer",i).getAttribute("position"));
            entry >> position;
            entry.clear();
            entry.str(layers.getChildNode("layer",i).getAttribute("poisson"));
            entry >> poisson;
            entry.clear();
            entry.str(layers.getChildNode("layer",i).getAttribute("rho"));
            entry >> rho;
            entry.clear();
            entry.str(layers.getChildNode("layer",i).getAttribute("young"));
            entry >> young;
            entry.clear();

            cf->loadLayerConfig(position,young,poisson,rho);

        }
    }
}
