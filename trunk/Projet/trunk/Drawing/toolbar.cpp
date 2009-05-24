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

    _keepBezierCheckBox = new QCheckBox("Keep bezier curves", this);
    _keepBezierCheckBox->click();
    _vlayout->addWidget(_keepBezierCheckBox);

    _toolButtonAlignTangents = new QPushButton("Align tangents",this);
    _toolButtonAlignTangents->setDisabled(true);
    _vlayout->addWidget(_toolButtonAlignTangents);

    _toolButtonClean = new QPushButton("Clean",this);
    _toolButtonClean->setDisabled(true);
    _vlayout->addWidget(_toolButtonClean);

    _toolButtonSimplifyView = new QPushButton("Simplify view",this);
    _toolButtonSimplifyView->setCheckable(true);
    _toolButtonSimplifyView->setEnabled(true);
    _vlayout->addWidget(_toolButtonSimplifyView);

    _undoButton = new QPushButton("Undo",this);
    _vlayout->addWidget(_undoButton);

    _redoButton = new QPushButton("Redo",this);
    _vlayout->addWidget(_redoButton);

    _magnetCheckBox = new QCheckBox("Activate magnet", this);
    _magnetCheckBox->click();
    _magnetCheckBox->click();
    _vlayout->addWidget(_magnetCheckBox);

    _toolButtonOpenImage = new QPushButton("Open Image", this);
    _vlayout->addWidget(_toolButtonOpenImage);

    _toolButtonModifyPicture = new QPushButton("Modify backgroud picture");
    _toolButtonModifyPicture->setCheckable(true);
    _toolButtonModifyPicture->setEnabled(true);
    _vlayout->addWidget(_toolButtonModifyPicture);

    _toolButtonRemovePicture = new QPushButton("Remove backgroud picture");
    _toolButtonRemovePicture->setEnabled(true);
    _vlayout->addWidget(_toolButtonRemovePicture);

    /*QPushButton* viewCoords = new QPushButton("View coords", this);
    _vlayout->addWidget(viewCoords);*/

    QPushButton* koin = new QPushButton("Koin !",this);
    koin->setEnabled(true);
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
    QObject::connect(_scene, SIGNAL(lineInterrupted()), this, SLOT(lineInterrupted()));
    QObject::connect(_toolButtonAlignTangents, SIGNAL(clicked()), _scene, SLOT(alignTangents()));
    QObject::connect(_toolButtonClean, SIGNAL(clicked()), _scene, SLOT(cleanPoints()));
    QObject::connect(_toolButtonClean, SIGNAL(clicked()), this, SLOT(clean()));
    QObject::connect(_toolButtonSimplifyView, SIGNAL(clicked(bool)), _scene, SLOT(simplifyView(bool)));
    QObject::connect(_undoButton, SIGNAL(clicked()), _scene, SLOT(undo()));
    QObject::connect(_redoButton, SIGNAL(clicked()), _scene, SLOT(redo()));
    QObject::connect(_scene, SIGNAL(pointsOnScene(bool)), this, SLOT(pointsOnScene(bool)));
    QObject::connect(_keepBezierCheckBox, SIGNAL(clicked(bool)), _scene, SLOT(keepBezierCurve(bool)));
    QObject::connect(_magnetCheckBox, SIGNAL(clicked(bool)), _scene, SLOT(activateMagnet(bool)));
    QObject::connect(_toolButtonOpenImage, SIGNAL(clicked()), this, SLOT(openImage()));
    QObject::connect(_toolButtonModifyPicture, SIGNAL(clicked(bool)), _scene, SLOT(modifyBackgroundPicture(bool)));
    QObject::connect(_toolButtonModifyPicture, SIGNAL(clicked(bool)), this, SLOT(modifyPicture(bool)));
    QObject::connect(_toolButtonRemovePicture, SIGNAL(clicked()), _scene, SLOT(removeBackgroundPicture()));


    QObject::connect(koin, SIGNAL(clicked()), this, SLOT(changeColor()));
    QObject::connect(this, SIGNAL(changeColor(int,int,QColor)), _scene, SLOT(changeColor(int,int,QColor)));

    //QObject::connect(viewCoords, SIGNAL(clicked()), _scene, SLOT(showCoords()));
}

//PUBLIC SLOTS

void ToolBar::activeAddControl(bool a){
    _toolButtonAddPoint->setDisabled(a);
    _toolButtonAlignTangents->setDisabled(a);
    _toolButtonClean->setDisabled(a);
    _toolButtonRemoveControl->setDisabled(a);
    _undoButton->setDisabled(a);
    _redoButton->setDisabled(a);
    _toolButtonModifyPicture->setDisabled(a);
}

void ToolBar::activeAddPoint(bool a){
    _toolButtonAddControl->setDisabled(a);
    _toolButtonAlignTangents->setDisabled(a);
    _toolButtonClean->setDisabled(a);
    _toolButtonRemoveControl->setDisabled(a);
    _undoButton->setDisabled(a);
    _redoButton->setDisabled(a);
    _toolButtonModifyPicture->setDisabled(a);
}

void ToolBar::activateRemoveControl(bool a){
    _toolButtonAddPoint->setDisabled(a);
    _toolButtonAddControl->setDisabled(a);
    _toolButtonAlignTangents->setDisabled(a);
    _toolButtonClean->setDisabled(a);
    _undoButton->setDisabled(a);
    _redoButton->setDisabled(a);
    _toolButtonModifyPicture->setDisabled(a);
}

void ToolBar::beginLine(bool a){
    _toolButtonPoint->setDisabled(a);
    _toolButtonAddControl->setDisabled(a);
    _toolButtonAddPoint->setDisabled(a);
    _toolButtonAlignTangents->setDisabled(a);
    _toolButtonClean->setDisabled(a);
    _toolButtonRemoveControl->setDisabled(a);
    _undoButton->setDisabled(a);
    _redoButton->setDisabled(a);
    _toolButtonModifyPicture->setDisabled(a);
}

void ToolBar::changeColor(){
    qsrand(QTime::currentTime().msec());

    int r = qrand()% 256;
    int g = qrand()% 256;
    int b = qrand()% 256;
    emit changeColor(PaintingScene::BoundingPointColor, PaintingScene::NormalColor, QColor(r,g,b));

    r = qrand()% 256;
    g = qrand()% 256;
    b = qrand()% 256;
    emit changeColor(PaintingScene::BoundingPointColor, PaintingScene::HighlightingColor, QColor(r,g,b));

    r = qrand()% 256;
    g = qrand()% 256;
    b = qrand()% 256;
    emit changeColor(PaintingScene::BoundingPointColor, PaintingScene::SelectionColor, QColor(r,g,b));

    r = qrand()% 256;
    g = qrand()% 256;
    b = qrand()% 256;
    emit changeColor(PaintingScene::ControlPointColor, PaintingScene::NormalColor, QColor(r,g,b));

    r = qrand()% 256;
    g = qrand()% 256;
    b = qrand()% 256;
    emit changeColor(PaintingScene::ControlPointColor, PaintingScene::HighlightingColor, QColor(r,g,b));

    r = qrand()% 256;
    g = qrand()% 256;
    b = qrand()% 256;
    emit changeColor(PaintingScene::LineColor, PaintingScene::NormalColor, QColor(r,g,b));

    r = qrand()% 256;
    g = qrand()% 256;
    b = qrand()% 256;
    emit changeColor(PaintingScene::LineColor, PaintingScene::HighlightingColor, QColor(r,g,b));


}

void ToolBar::clean(){
    _toolButtonPoint->setEnabled(true);
    _toolButtonAddControl->setDisabled(true);
    _toolButtonAddPoint->setDisabled(true);
    _toolButtonAlignTangents->setDisabled(true);
    _toolButtonClean->setDisabled(true);
    _toolButtonRemoveControl->setDisabled(true);
    _undoButton->setEnabled(true);
    _redoButton->setEnabled(true);
}

void ToolBar::finishedLine(bool a){
    if(a){
        _toolButtonPoint->setEnabled(true);
        _toolButtonPoint->click();
        _toolButtonPoint->setDisabled(true);
    }
}

void ToolBar::lineInterrupted(){
    _toolButtonPoint->setEnabled(true);
    _toolButtonPoint->click();
    this->pointsOnScene(false);
}

void ToolBar::modifyPicture(bool a){
    _toolButtonAddPoint->setDisabled(a);
    _toolButtonAddControl->setDisabled(a);
    _toolButtonAlignTangents->setDisabled(a);
    _toolButtonClean->setDisabled(a);
    _toolButtonRemoveControl->setDisabled(a);
    _undoButton->setDisabled(a);
    _redoButton->setDisabled(a);

}

void ToolBar::openImage(){
    QString file = QFileDialog::getOpenFileName(this, "Ouvrir un fichier", QString(), "Images (*.png *.bmp *.jpg)");
    if(!file.isNull()){
        QPixmap pix(file);
        _scene->setBackGroundPicture(pix);
    }
}

void ToolBar::pointsOnScene(bool a){
    if(a){
        _toolButtonPoint->setDisabled(true);
        _toolButtonAddControl->setEnabled(true);
        _toolButtonAddPoint->setEnabled(true);
        _toolButtonAlignTangents->setEnabled(true);
        _toolButtonClean->setEnabled(true);
        _toolButtonRemoveControl->setEnabled(true);
    }else{
        _toolButtonPoint->setEnabled(true);
        _toolButtonAddControl->setDisabled(true);
        _toolButtonAddPoint->setDisabled(true);
        _toolButtonAlignTangents->setDisabled(true);
        _toolButtonClean->setDisabled(true);
        _toolButtonRemoveControl->setDisabled(true);
    }

}
