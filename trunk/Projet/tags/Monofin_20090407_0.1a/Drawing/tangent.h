#ifndef TANGENT_H
#define TANGENT_H

#include <QWidget>
#include <QGraphicsItem>
#include <QLineF>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QPen>

class PaintingScene;

class Tangent: public QGraphicsItem{

public:
    Tangent(QPointF p1, QPointF p2, PaintingScene* scene);
    ~Tangent();

    virtual QRectF boundingRect() const;
    QLineF line(){return *_line;}
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);
    void setLine(QPointF p1, QPointF p2);

protected:
    QLineF* _line;
    PaintingScene* _scene;

};

#endif // TANGENT_H
