#include "CEntityItem.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>

CEntityItem::CEntityItem() :
    _width(100),
    _height(100)
{
    this->setFlags(ItemIsSelectable | ItemIsMovable);
    this->setAcceptHoverEvents(true);
}

QRectF CEntityItem::boundingRect() const
{
    return QRectF(-3, -3, _width + 6, _height + 6);
}

QPainterPath CEntityItem::shape() const
{
    QPainterPath path;
    path.addRoundedRect(0, 0, _width, _height, 5, 5);
    return path;
}

void CEntityItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    if(option->state & QStyle::State_Selected)
    {
        QBrush shadowBrush(QColor(192, 192, 192, 128), Qt::SolidPattern);
        painter->setBrush(shadowBrush);
        painter->setPen(Qt::NoPen);
        painter->drawRoundedRect(-3, -3, _width + 6, _height + 6, 5, 5);
    }

    QColor penColor = QColor(Qt::black);
    if(option->state & QStyle::State_Selected)
        penColor = QColor(Qt::blue);
    if(option->state & QStyle::State_MouseOver)
        penColor = QColor(Qt::red);

    QBrush fillBrush(Qt::white, Qt::SolidPattern);
    QBrush penBrush(penColor, Qt::SolidPattern);
    QPen pen(penBrush, 1);
    painter->setBrush(fillBrush);
    painter->setPen(pen);

    painter->drawRoundedRect(0, 0, _width, _height, 5, 5);
}
