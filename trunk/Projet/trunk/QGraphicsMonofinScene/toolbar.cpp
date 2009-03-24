#include "toolbar.h"
#include "paintingscene.h"

//PUBLIC

ToolBar::ToolBar(PaintingScene* scene, QWidget* parent)
        :QWidget(parent), _scene(scene){

    _vlayout = new QVBoxLayout;
    this->setLayout(_vlayout);

    _toolButtonPoint = new QPushButton("Create points",this);
    _toolButtonPoint->setCheckable(true);
    _vlayout->addWidget(_toolButtonPoint);

    _toolButtonAddControl = new QPushButton("Add control points",this);
    _toolButtonAddControl->setCheckable(true);
    _toolButtonAddControl->setDisabled(true);
    _vlayout->addWidget(_toolButtonAddControl);

    _toolButtonRemoveControl = new QPushButton("Remove control points",this);
    _toolButtonRemoveControl->setCheckable(true);
    _toolButtonRemoveControl->setDisabled(true);
    _vlayout->addWidget(_toolButtonRemoveControl);

    _toolButtonAddPoint = new QPushButton("Insert points",this);
    _toolButtonAddPoint->setCheckable(true);
    _toolButtonAddPoint->setDisabled(true);
    _vlayout->addWidget(_toolButtonAddPoint);

    _toolButtonAlignTangents = new QPushButton("Align tangents",this);
    _toolButtonAlignTangents->setDisabled(true);
    _vlayout->addWidget(_toolButtonAlignTangents);

    _toolButtonClean = new QPushButton("Clean",this);
    _toolButtonClean->setDisabled(true);
    _vlayout->addWidget(_toolButtonClean);

    /*QPushButton* viewCoords = new QPushButton("View coords", this);
    _vlayout->addWidget(viewCoords);*/

    QPushButton* koin = new QPushButton("Koin !",this);
    koin->setDisabled(true);
    _vlayout->addWidget(koin);


    QObject::connect(_toolButtonPoint,SIGNAL(clicked(bool)),_scene,SLOT(activateCreateLine(bool)));
    QObject::connect(_toolButtonAddControl, SIGNAL(clicked(bool)), _scene, SLOT(activateAddControl(bool)));
    QObject::connect(_toolButtonAddControl, SIGNAL(clicked(bool)), this, SLOT(activeAddControl(bool)));
    QObject::connect(_toolButtonRemoveControl, SIGNAL(clicked(bool)), _scene, SLOT(activateRemoveControlPoint(bool)));
    QObject::connect(_toolButtonRemoveControl, SIGNAL(clicked(bool)), this, SLOT(activateRemoveControl(bool)));
    QObject::connect(_toolButtonAddPoint, SIGNAL(clicked(bool)), _scene, SLOT(activateAddPoint(bool)));
    QObject::connect(_toolButtonAddPoint, SIGNAL(clicked(bool)), this, SLOT(activeAddPoint(bool)));
    QObject::connect(_toolButtonPoint, SIGNAL(clicked(bool)), this, SLOT(beginLine(bool)));
    QObject::connect(_scene, SIGNAL(lineFinished(bool)), this, SLOT(finishedLine(bool)));
    QObject::connect(_toolButtonAlignTangents, SIGNAL(clicked()), _scene, SLOT(alignTangents()));
    QObject::connect(_toolButtonClean, SIGNAL(clicked()), _scene, SLOT(cleanPoints()));
    QObject::connect(_toolButtonClean, SIGNAL(clicked()), this, SLOT(clean()));
    //QObject::connect(viewCoords, SIGNAL(clicked()), _scene, SLOT(showCoords()));
}

//PUBLIC SLOTS

void ToolBar::activeAddControl(bool a){
    _toolButtonAddPoint->setDisabled(a);
    _toolButtonAlignTangents->setDisabled(a);
    _toolButtonClean->setDisabled(a);
    _toolButtonRemoveControl->setDisabled(a);
}

void ToolBar::activeAddPoint(bool a){
    _toolButtonAddControl->setDisabled(a);
    _toolButtonAlignTangents->setDisabled(a);
    _toolButtonClean->setDisabled(a);
    _toolButtonRemoveControl->setDisabled(a);
}

void ToolBar::activateRemoveControl(bool a){
    _toolButtonAddPoint->setDisabled(a);
    _toolButtonAddControl->setDisabled(a);
    _toolButtonAlignTangents->setDisabled(a);
    _toolButtonClean->setDisabled(a);
}

void ToolBar::beginLine(bool a){
    _toolButtonPoint->setDisabled(a);
    _toolButtonAddControl->setDisabled(a);
    _toolButtonAddPoint->setDisabled(a);
    _toolButtonAlignTangents->setDisabled(a);
    _toolButtonClean->setDisabled(a);
    _toolButtonRemoveControl->setDisabled(a);
}

void ToolBar::clean(){
    _toolButtonPoint->setEnabled(true);
    _toolButtonAddControl->setDisabled(true);
    _toolButtonAddPoint->setDisabled(true);
    _toolButtonAlignTangents->setDisabled(true);
    _toolButtonClean->setDisabled(true);
    _toolButtonRemoveControl->setDisabled(true);
}

void ToolBar::finishedLine(bool a){
    if(a){
        _toolButtonPoint->setEnabled(true);
        _toolButtonPoint->click();
        _toolButtonPoint->setDisabled(true);
    }
}
