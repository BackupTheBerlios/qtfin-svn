#include "drawpreview.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QPainter>
#include <QPainterPath>

using namespace Data;

DrawPreview::DrawPreview(QWidget* parent, ProjectFile* preview, qreal width, qreal height) :
        QWidget(parent, Qt::Window), _width(width), _height(height), _preview(preview)
{
    this->setWindowModality(Qt::WindowModal);
    this->resize(1024, 600);
    _keepButton = new QPushButton(this);
    _keepButton->setIcon(QIcon("../resources/icons/SnakeOK"));
    _keepButton->setIconSize(QSize(50, 50));
    _keepButton->setToolTip(tr("Keep the form"));
    _doNotKeepButton = new QPushButton(this);
    _doNotKeepButton->setIcon(QIcon("../resources/icons/SnakeNotOK"));
    _doNotKeepButton->setIconSize(QSize(50, 50));
    _doNotKeepButton->setToolTip(tr("Do not keep the form"));
    _doNotKeepButton->setDefault(true);
    QSpacerItem* spacerV = new QSpacerItem(10, (int)_height, QSizePolicy::Expanding, QSizePolicy::Expanding);
    QSpacerItem* spacerH1 = new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Expanding);
    QSpacerItem* spacerH2 = new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Expanding);
    QVBoxLayout* layoutV = new QVBoxLayout(this);
    QHBoxLayout* layoutH= new QHBoxLayout(this);
    layoutH->addSpacerItem(spacerH1);
    layoutH->addWidget(_keepButton);
    layoutH->addWidget(_doNotKeepButton);
    layoutH->addSpacerItem(spacerH2);
    layoutV->addSpacerItem(spacerV);
    layoutV->addLayout(layoutH);
    this->setLayout(layoutV);

    QObject::connect(_keepButton, SIGNAL(clicked()), this, SLOT(keep()));
    QObject::connect(_doNotKeepButton, SIGNAL(clicked()), this, SLOT(doNotKeep()));
    //QObject::connect(this->, SIGNAL(destroyed()), this, SLOT(doNotKeep()));
}

void DrawPreview::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    painter.setPen(QPen(QBrush("red"), 3));
    painter.drawLine(QPointF(0, this->height() / 2), QPointF(this->width(), this->height() / 2));
    QList<int> listSeg = _preview->getAllSegmentKeys();
    for(int i = 0; i < listSeg.size(); i++){
        int p1;
        int p2;
        int c;
        _preview->getSegment(listSeg.value(i), p1, p2, c);

        float p1x;
        float p1y;
        _preview->getIntersectionPoint(p1, p1x, p1y);
        float p1yS = this->height() / 2 + p1y;
        p1y = this->height() / 2 - p1y;

        float p2x;
        float p2y;
        _preview->getIntersectionPoint(p2, p2x, p2y);
        float p2yS = this->height() / 2 + p2y;
        p2y = this->height() / 2 - p2y;

        float cx;
        float cy;
        _preview->getControlPoint(c, cx, cy);
        float cyS = this->height() / 2 + cy;
        cy = this->height() / 2 - cy;

        QPainterPath bezier(QPointF(p1x, p1y));
        bezier.cubicTo(cx, cy, cx, cy, p2x, p2y);
        QPainterPath bezierS(QPointF(p1x, p1yS));
        bezierS.cubicTo(cx, cyS, cx, cyS, p2x, p2yS);

        painter.setPen(QPen(QBrush("black"), 2));
        painter.drawPath(bezier);
        painter.drawPath(bezierS);

        painter.setPen(QPen(QBrush("blue"), 4));
        painter.drawPoint((int)p1x, (int)p1y);
    }
    int p, lp, c;
    _preview->getSegment(listSeg.last(), p, lp, c);
    float x, y;
    _preview->getIntersectionPoint(lp, x , y);
    y = this->height() / 2 - y;
    painter.drawPoint((int)x, (int)y);
}

void DrawPreview::closeEvent(QCloseEvent* event){
    this->doNotKeep();
}

void DrawPreview::keep(){
    emit this->kept();
    this->close();
}

void DrawPreview::doNotKeep(){
    _preview->undo(Data::MonofinSurface);
    emit this->doNotKept();
    this->close();
}
