#include "CEntityItem.h"
#include "CRelationshipItem.h"
#include "CEntity.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>

CEntityItem::CEntityItem(CEntity *entity) :
    _width(100),
    _height(90),
    _color(Qt::lightGray),
    _entity(entity),
    _selectedForRelation(false)
{
    this->setFlags(ItemIsSelectable |
                   ItemIsMovable |
                   ItemSendsGeometryChanges);
    this->setAcceptHoverEvents(true);

    _polygon << QPointF(0, 0) << QPointF(_width, 0)
              << QPointF(_width, _height) << QPointF(0, _height)
              << QPointF(0, 0);
    setPolygon(_polygon);
}

int CEntityItem::width() const
{
    return _width;
}

int CEntityItem::height() const
{
    return _height;
}

QPolygonF CEntityItem::polygon() const
{
    return _polygon;
}

void CEntityItem::removeRelationship(CRelationshipItem *relationship)
{
    int index = _relationships.indexOf(relationship);
    if(index != -1)
        _relationships.removeAt(index);
}

void CEntityItem::removeRelationships()
{
    foreach (CRelationshipItem *relationship, _relationships)
    {
        relationship->startItem()->removeRelationship(relationship);
        relationship->endItem()->removeRelationship(relationship);
        this->scene()->removeItem(relationship);
        delete relationship;
    }
}

void CEntityItem::addRelationship(CRelationshipItem *relationship)
{
    _relationships.append(relationship);
}

void CEntityItem::setColor(const QColor &color)
{
    _color = color;
}

void CEntityItem::setSelectedForRelation(bool selectedForRelation)
{
    _selectedForRelation = selectedForRelation;
}

int CEntityItem::type() const
{
    return Type;
}

int CEntityItem::id()
{
    if(_entity != 0)
        return _entity->id();
    else
        return -1;
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

    // gray outline of selected item
    if((option->state & QStyle::State_Selected) || _selectedForRelation)
    {
        QPen selectPen(QBrush(QColor(Qt::gray)), 5);

        painter->setPen(selectPen);
        painter->setBrush(Qt::NoBrush);

        painter->drawRect(-1, -1, _width + 2, _height + 2);
    }

    // main outline of item
    // changes color if item is selected, mouseovered or not
    QColor penColor = QColor(Qt::black);
    if(option->state & QStyle::State_Selected)
        penColor = QColor(61, 141, 253);
    if((option->state & QStyle::State_MouseOver) || _selectedForRelation)
        penColor = QColor(206, 70, 72);
    QPen pen(penColor, 1);
    QBrush brush(QColor(Qt::white));
    painter->setPen(pen);
    painter->setBrush(brush);

    painter->drawRect(-1, -1, _width + 2, _height + 2);

    // title rect
    // changes color brightness if item is selected, mouseovered or not
    QBrush titleBrush(_color);
    if(option->state & QStyle::State_Selected)
        titleBrush.setColor(_color.dark(120));
    if(option->state & QStyle::State_MouseOver)
        titleBrush.setColor(_color.light(120));
    painter->setPen(Qt::NoPen);
    painter->setBrush(titleBrush);

    painter->drawRect(0, 0, _width, 30);

    // title text
    painter->setPen(oldPen);
    painter->setBrush(oldBrush);
    QFont font("Calibri", 12);
    font.setStyleStrategy(QFont::NoAntialias);
    font.setBold(true);
    painter->setFont(font);

    painter->drawText(QRect(2, 5, _width - 2, 25), "table @");

    // rows text
    font.setBold(false);
    painter->setFont(font);
    int textHeight = 30;
    for(int i = 0; i < 3; i++)
    {
        painter->drawText(QRect(2, textHeight, _width + 2, 20), QString("[ ] row %1").arg(i));
        textHeight += 20;
    }
}

QVariant CEntityItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if(change == QGraphicsItem::ItemPositionChange)
    {
        foreach (CRelationshipItem *relationship, _relationships) {
            relationship->updatePosition();
        }
    }
    return value;
}
