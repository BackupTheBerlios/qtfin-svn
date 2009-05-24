#include "xmlloadvisitor.h"
#include "surface.h"
#include "profil.h"
#include "configfile.h"
#include <sstream>
#include <iostream>
#include <QDomNode>
#include <QFile>

namespace Data{
    void XMLLoadVisitor::load(std::string path){
        //_mainNode = XMLNode::openFileHelper(path.c_str(),"project");
        _mainDocument = QDomDocument("project");
        QFile test(QString(path.c_str()));
        //test = QFile(QString(path));
        _mainDocument.setContent(&test);
        _mainNode = _mainDocument.documentElement();
        test.close();
    }

    void XMLLoadVisitor::visitSurface(Surface *sf){
        //XMLNode surface = _mainNode.getChildNode("monofin");
        QDomElement surface = _mainNode.elementsByTagName("monofin").at(0).toElement();
        //chargement des points intersections
        //XMLNode inter = surface.getChildNode("points").getChildNode("intersection");
        QDomElement inter = surface.elementsByTagName("points").at(0).toElement().elementsByTagName("intersection").at(0).toElement();
        //nombre de points intersections
        //int nbInter = inter.nChildNode("point");
        QDomNode nodeInter= inter.firstChild();
        while(!nodeInter.isNull()){
            QDomElement currentInter = nodeInter.toElement();
            if(!currentInter.isNull()){
                if(currentInter.tagName() == "point"){
                    int key;
                    float x,y;
                    bool okKey,okX,okY;
                    key = currentInter.attribute("number","").toInt(&okKey,10);
                    x = currentInter.attribute("X","").toFloat(&okX);
                    y = currentInter.attribute("Y","").toFloat(&okY);
                    if(okKey && okX && okY)
                        sf->loadIntersectionPoint(key,x,y);
                }
            }
            nodeInter = nodeInter.nextSibling();
        }

        //chargement des points de controle
        QDomElement control = surface.elementsByTagName("points").at(0).toElement().elementsByTagName("control").at(0).toElement();

        //nombre de points intersections
        //int nbInter = inter.nChildNode("point");
        QDomNode nodeControl= control.firstChild();
        while(!nodeControl.isNull()){
            QDomElement currentControl = nodeControl.toElement();
            if(!currentControl.isNull()){
                if(currentControl.tagName() == "point"){
                    int key;
                    float x,y;
                    bool okKey,okX,okY;
                    key = currentControl.attribute("number","").toInt(&okKey,10);
                    x = currentControl.attribute("X","").toFloat(&okX);
                    y = currentControl.attribute("Y","").toFloat(&okY);
                    if(okKey && okX && okY)
                        sf->loadControlPoint(key,x,y);
                }
            }
            nodeControl = nodeControl.nextSibling();
        }

        //chargement des segments
        //XMLNode segments = surface.getChildNode("segments");
        QDomElement segment = surface.elementsByTagName("segments").at(0).toElement();
        //int nbSeg = segments.nChildNode("segment");
        QDomNode segmentNode= segment.firstChild();
        while(!segmentNode.isNull()){
            QDomElement currentSegment = segmentNode.toElement();
            if(!currentSegment.isNull()){
                if(currentSegment.tagName() == "segment"){
                    int segKey, interKey[2], controlKey = Data::MONOFIN_SURFACE_NO_CONTROL_POINT,
                    interCount = 0;
                    bool okKey, okInter[2], okControl = true;
                    segKey = currentSegment.attribute("number","").toInt(&okKey,10);
                    QDomNode keyPoint = currentSegment.firstChild();
                    while(!keyPoint.isNull()){
                        QDomElement currentKey = keyPoint.toElement();
                        if(currentKey.tagName() == "intersectionpoint" && interCount < 2){
                            interKey[interCount] = currentKey.attribute("number").toInt(&okInter[interCount],10);
                            interCount++;
                        }else if(currentKey.tagName() == "controlpoint"){
                            controlKey = currentKey.attribute("number").toInt(&okControl,10);
                        }
                        keyPoint = keyPoint.nextSibling();
                    }

                    if(okKey && okInter[0] && okInter[1] && okControl)
                        sf->loadSegment(segKey,interKey[0],interKey[1],controlKey);
                }
            }
            segmentNode = segmentNode.nextSibling();
        }

    }

    void XMLLoadVisitor::visitProfil(Profil *pr){
        //chargement des layers
        //XMLNode layers = _mainNode.getChildNode("monofin").getChildNode("layers");
        QDomElement layers = _mainNode.elementsByTagName("monofin").at(0).toElement().elementsByTagName("layers").at(0).toElement();
        //int nbLayer =  layers.nChildNode("layer");
        QDomNode layerNode = layers.firstChild();
        while(!layerNode.isNull()){
            QDomElement currentLayer = layerNode.toElement();
            if(!currentLayer.isNull()){
                if(currentLayer.tagName() == "layer"){
                    int position;
                    float height, length;
                    bool okPos, okHei, okLen;
                    position = currentLayer.attribute("position","").toInt(&okPos,10);
                    height = currentLayer.attribute("height","").toFloat(&okHei);
                    length = currentLayer.attribute("length","").toFloat(&okLen);
                    if(okPos && okHei && okLen)
                        pr->loadLayer(position, length,height);
                }
            }
            layerNode = layerNode.nextSibling();
        }

    }

    void XMLLoadVisitor::visitConfigFile(ConfigFile * cf){
        //chargement des layers
        //XMLNode layers = _mainNode.getChildNode("config").getChildNode("layerConfig");
        QDomElement layers = _mainNode.elementsByTagName("config").at(0).toElement().elementsByTagName("layerConfig").at(0).toElement();
        //int nbLayer =  layers.nChildNode("layer");
        QDomNode layerNode = layers.firstChild();
        while(!layerNode.isNull()){
            QDomElement currentLayer = layerNode.toElement();
            if(!currentLayer.isNull()){
                if(currentLayer.tagName() == "layer"){
                    int position;
                    float poisson, rho, young;
                    bool okPos, okPoi, okRho, okYou;

                    position = currentLayer.attribute("position","").toInt(&okPos,10);
                    poisson = currentLayer.attribute("poisson","").toFloat(&okPoi);
                    rho = currentLayer.attribute("rho","").toFloat(&okRho);
                    young = currentLayer.attribute("young","").toFloat(&okYou);

                    if(okPos && okPoi && okRho && okYou)
                        cf->loadLayerConfig(position, young, poisson, rho);

                }
            }
        }
        layerNode  = layerNode.nextSibling();
    }
}
