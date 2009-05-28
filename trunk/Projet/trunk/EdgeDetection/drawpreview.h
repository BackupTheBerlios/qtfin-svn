#ifndef DRAWPREVIEW_H
#define DRAWPREVIEW_H

#include <QWidget>
#include <QPushButton>
#include "drawpreviewview.h"

class DrawPreview : public QWidget{
    Q_OBJECT

public:
    DrawPreview(QWidget* parent, Data::ProjectFile* preview, qreal width, qreal height, qreal scale);

protected:
    qreal _width;
    qreal _height;
    QGraphicsView* _view;
    QPushButton* _keepButton;
    QPushButton* _doNotKeepButton;
    Data::ProjectFile* _preview;

    //virtual void paintEvent(QPaintEvent * event);
    virtual void closeEvent(QCloseEvent*);

public slots:
    void keep();
    void doNotKeep();

signals:
    void kept();
    void doNotKept();


};

#endif // DRAWPREVIEW_H
