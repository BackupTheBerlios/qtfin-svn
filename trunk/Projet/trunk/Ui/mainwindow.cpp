#include "mainwindow.h"
#include "../ui_mainwindow.h"
#include "startupdialog.h"
#include "parametersdialog.h"
#include "Drawing/paintingview.h"
#include "Drawing/paintingscene.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindowClass)
{
    ui->setupUi(this);
    _scene = new PaintingScene(2048, 1536, this);
    _paramDiag = new ParametersDialog(this);

    setConnections();
    setCentralWidget(new PaintingView(_scene));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setConnections()
{
    // TOOLBAR
    connect(ui->actionCreatePolygon, SIGNAL(toggled(bool)),_scene,SLOT(activateCreateLine(bool)));
    connect(ui->actionCreatePolygon, SIGNAL(toggled(bool)), this, SLOT(beginLine(bool)));

    connect(ui->actionCleanPolygon, SIGNAL(triggered()), _scene, SLOT(cleanPoints()));
    connect(ui->actionCleanPolygon, SIGNAL(triggered()), this, SLOT(clean()));

    connect(ui->actionAddControl, SIGNAL(toggled(bool)), _scene, SLOT(activateAddControl(bool)));
    connect(ui->actionAddControl, SIGNAL(toggled(bool)), this, SLOT(activeAddControl(bool)));

    connect(ui->actionRemoveControl, SIGNAL(toggled(bool)), _scene, SLOT(activateRemoveControlPoint(bool)));
    connect(ui->actionRemoveControl, SIGNAL(toggled(bool)), this, SLOT(activateRemoveControl(bool)));

    connect(ui->actionAddPoint, SIGNAL(toggled(bool)), _scene, SLOT(activateAddPoint(bool)));
    connect(ui->actionAddPoint, SIGNAL(toggled(bool)), this, SLOT(activeAddPoint(bool)));

    connect(ui->actionAlignTangents, SIGNAL(triggered()), _scene, SLOT(alignTangents()));

    connect(_scene, SIGNAL(lineFinished(bool)), this, SLOT(finishedLine(bool)));

    // MENU
    connect(ui->actionE_xit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->action_Configurate, SIGNAL(triggered()), this, SLOT(configurate()));
}

void MainWindow::activeAddControl(bool a){
    ui->actionAddPoint->setDisabled(a);
    ui->actionAlignTangents->setDisabled(a);
    ui->actionCleanPolygon->setDisabled(a);
    ui->actionRemoveControl->setDisabled(a);
}

void MainWindow::activeAddPoint(bool a){
    ui->actionAddControl->setDisabled(a);
    ui->actionAlignTangents->setDisabled(a);
    ui->actionCleanPolygon->setDisabled(a);
    ui->actionRemoveControl->setDisabled(a);
}

void MainWindow::activateRemoveControl(bool a){
    ui->actionCreatePolygon->setDisabled(a);
    ui->actionAddControl->setDisabled(a);
    ui->actionAlignTangents->setDisabled(a);
    ui->actionCleanPolygon->setDisabled(a);
}

void MainWindow::beginLine(bool a){
    ui->actionCreatePolygon->setDisabled(a);
    ui->actionAddControl->setDisabled(a);
    ui->actionAddPoint->setDisabled(a);
    ui->actionAlignTangents->setDisabled(a);
    ui->actionCleanPolygon->setDisabled(a);
    ui->actionRemoveControl->setDisabled(a);
}

void MainWindow::clean(){
    ui->actionCreatePolygon->setEnabled(true);
    ui->actionAddControl->setDisabled(true);
    ui->actionAddPoint->setDisabled(true);
    ui->actionAlignTangents->setDisabled(true);
    ui->actionCleanPolygon->setDisabled(true);
    ui->actionRemoveControl->setDisabled(true);
}

void MainWindow::finishedLine(bool a){
    if(a){
        ui->actionCreatePolygon->toggle();
        ui->actionCreatePolygon->setDisabled(true);
    }
}

#include "layerParameters.h"
void MainWindow::configurate()
{
    if(_paramDiag->exec()) {
        QTabWidget *qtw = _paramDiag->layerTabWidget;
        for(int i = 0; i<qtw->count(); ++i) {
            LayerParameters * ll = static_cast<LayerParameters*> (qtw->widget(i));
            qDebug("%f", ll->youngDoubleSpinBox->value());
        }
    }
}
