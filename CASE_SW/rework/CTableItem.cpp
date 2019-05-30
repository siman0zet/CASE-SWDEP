#include "CTableItem.h"
#include "CRelationshipItem.h"
#include "CTable.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>

CTableItem::CTableItem(CTable *table) :
    _table(table),
    _width(100),
    _height(90),
    _selectedForRelation(false),
    _color(Qt::lightGray)
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

int CTableItem::width() const
{
    return _width;
}

int CTableItem::height() const
{
    return _height;
}

QPolygonF CTableItem::polygon() const
{
    return _polygon;
}

void CTableItem::removeRelationship(int id)
{
    _relationships.remove(id);
}

void CTableItem::removeRelationships()
{
    foreach (CRelationshipItem *relationship, _relationships)
    {
        relationship->startItem()->removeRelationship(relationship->id());
        relationship->endItem()->removeRelationship(relationship->id());
        this->scene()->removeItem(relationship);
        delete relationship;
    }
}

void CTableItem::addRelationship(CRelationshipItem *relationship)
{
    _relationships.insert(relationship->id(), relationship);
}

void CTableItem::setColor(const QColor &color)
{
    _color = color;
}

void CTableItem::setSelectedForRelation(bool selectedForRelation)
{
    _selectedForRelation = selectedForRelation;
}

int CTableItem::type() const
{
    return Type;
}

int CTableItem::id()
{
    if(_table != 0)
        return _table->id();
    else
        return -1;
}

QString CTableItem::name()
{
    if(_table != 0)
        return _table->name();
    else
        return "";
}

CTable *CTableItem::table() const
{
    return _table;
}

QRectF CTableItem::boundingRect() const
{
    return QRectF(-5, -5, _width + 10 , _height + 10);
}

QPainterPath CTableItem::shape() const
{
    QPainterPath path;
    path.addRect(0, 0, _width, _height);
    return path;
}

void CTableItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
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

QVariant CTableItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if(change == QGraphicsItem::ItemPositionChange)
    {
        foreach (CRelationshipItem *relationship, _relationships) {
            relationship->updatePosition();
        }
    }
    return value;
}
