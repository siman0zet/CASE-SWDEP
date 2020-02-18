#include "ctableitem.h"
#include "crelationshipitem.h"
#include "../model/ctable.h"
#include "../model/crow.h"
#include "../model/cforeignrow.h"

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>
#include <QDebug>

CTableItem::CTableItem(CTable *table) :
    CObjectItem(table),
    _table(table),
    _width(150),
    _height(30),
    _selectedForRelation(false),
    _color(Qt::lightGray)
{
    this->setFlag(ItemIsMovable, true);

    _polygon << QPointF(0, 0) << QPointF(_width, 0)
              << QPointF(_width, _height) << QPointF(0, _height)
              << QPointF(0, 0);
}

CTableItem::~CTableItem()
{
    _table = NULL;
}

int CTableItem::type() const
{
    return Type;
}

CTable *CTableItem::table() const
{
    return _table;
}

int CTableItem::width() const
{
    return _width;
}

void CTableItem::setWidth(int width)
{
    _width = width;

    _polygon.clear();
    _polygon << QPointF(0, 0) << QPointF(_width, 0)
              << QPointF(_width, _height) << QPointF(0, _height)
              << QPointF(0, 0);
}

int CTableItem::height() const
{
    return _height;
}

void CTableItem::setHeight(int height)
{
    _height = height;

    _polygon.clear();
    _polygon << QPointF(0, 0) << QPointF(_width, 0)
              << QPointF(_width, _height) << QPointF(0, _height)
              << QPointF(0, 0);
}

QPolygonF CTableItem::polygon() const
{
    return _polygon;
}

void CTableItem::removeRelationship(CRelationshipItem *relationship)
{
    int index = _relationships.indexOf(relationship);
    if(index != -1)
        _relationships.removeAt(index);
}

void CTableItem::removeRelationships()
{
    foreach (CRelationshipItem *relationship, _relationships)
    {
        relationship->startItem()->removeRelationship(relationship);
        relationship->endItem()->removeRelationship(relationship);
        this->scene()->removeItem(relationship);
        delete relationship;
    }
}

void CTableItem::addRelationship(CRelationshipItem *relationship)
{
    _relationships.append(relationship);
}

void CTableItem::setColor(const QColor &color)
{
    _color = color;
}

void CTableItem::setSelectedForRelation(bool selectedForRelation)
{
    if(this != 0)
        _selectedForRelation = selectedForRelation;
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
    font.setBold(true);
    painter->setFont(font);

    painter->drawText(QRect(2, 5, _width - 2, 25), _table->name());

    // rows text
    this->setHeight(30 + _table->totalRowCount() * 20);
    font.setPointSize(10);
    font.setBold(false);
    painter->setFont(font);
    int textHeight = 30;
    for(int i = 0; i < _table->rowCount(); i++)
    {
        QString constraint = "";
        if(_table->row(i)->unique())
            constraint = "[UQ]";
        if(_table->row(i)->notNull())
            constraint = "[NN]";
        if(_table->row(i)->primaryKey())
            constraint = "[PK]";

        painter->drawText(QRect(2, textHeight, _width - 2, 20),
                          QString("%1 %2 %3")
                          .arg(constraint)
                          .arg(_table->row(i)->name())
                          .arg(_table->row(i)->typeAsString()));
        textHeight += 20;
    }
    for(int i = 0; i < _table->foreignRowCount(); i++)
    {
        QString constraint = "[FK]";
        if(_table->foreignRow(i)->primaryKey())
            constraint = "[PF]";
        painter->drawText(QRect(2, textHeight, _width + 2, 20),
                          QString("%1 %2 %3")
                          .arg(constraint)
                          .arg(_table->foreignRow(i)->name())
                          .arg(_table->foreignRow(i)->typeAsString()));
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

QList<CRelationshipItem *> CTableItem::relationships() const
{
    return _relationships;
}
