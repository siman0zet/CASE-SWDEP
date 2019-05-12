#ifndef CENTITYITEM_H
#define CENTITYITEM_H

#include <QGraphicsItem>

class CEntityItem : public QGraphicsItem
{
public:
    CEntityItem();

    // QGraphicsItem interface
    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
    int   _width;
    int   _height;
};

#endif // CENTITYITEM_H
