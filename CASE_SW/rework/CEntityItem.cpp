#include "CEntityItem.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>

CEntityItem::CEntityItem() :
    _width(100),
    _height(90)
{
    this->setFlags(ItemIsSelectable | ItemIsMovable);
    this->setAcceptHoverEvents(true);
}

QRectF CEntityItem::boundingRect() const
{
    return QRectF(-5, -5, _width + 10 , _height + 10);
}

QPainterPath CEntityItem::shape() const
{
    QPainterPath path;
    path.addRect(0, 0, _width, _height);
    return path;
}

void CEntityItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    prepareGeometryChange();

    QPen oldPen = painter->pen();
    QBrush oldBrush = painter->brush();

    if(option->state & QStyle::State_Selected)
    {
        QPen selectPen(QBrush(QColor(Qt::gray)), 5);

        painter->setPen(selectPen);
        painter->setBrush(Qt::NoBrush);

        painter->drawRect(0, 0, _width, _height);
    }

    QColor penColor = QColor(Qt::black);
    if(option->state & QStyle::State_Selected)
        penColor = QColor(61, 141, 253);
    if(option->state & QStyle::State_MouseOver)
        penColor = QColor(206, 70, 72);
    QPen pen(penColor, 1);
    QBrush brush(QColor(Qt::white));
    painter->setPen(pen);
    painter->setBrush(brush);

    painter->drawRect(-1, -1, _width + 2, _height + 2);

    //content
    QBrush titleBrush(QColor(225, 225, 225));
    painter->setPen(Qt::NoPen);
    painter->setBrush(titleBrush);

    painter->drawRect(0, 0, _width, 30);

    //title
    painter->setPen(oldPen);
    painter->setBrush(oldBrush);
    QFont font("Calibri", 12);
    font.setStyleStrategy(QFont::NoAntialias);
    font.setBold(true);
    painter->setFont(font);

    painter->drawText(QRect(2, 5, _width - 2, 25), "table @");

    //rows
    font.setBold(false);
    painter->setFont(font);
    int textHeight = 30;
    for(int i = 0; i < 3; i++)
    {
        painter->drawText(QRect(2, textHeight, _width + 2, 20), QString("[ ] row %1").arg(i));
        textHeight += 20;
    }
}
