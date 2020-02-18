#include "crelationshipitem.h"
#include "ctableitem.h"
#include "../model/crelationship.h"

#include <QPainter>
#include <qmath.h>
#include <QDebug>
#include <QStyleOptionGraphicsItem>

CRelationshipItem::CRelationshipItem(CTableItem *startItem, CTableItem *endItem, CRelationship *relationship) :
    CObjectItem(relationship),
    _relationship(relationship),
    _startItem(startItem),
    _endItem(endItem)
{
    setZValue(-1000.0);

    _startItem->addRelationship(this);
    _endItem->addRelationship(this);
    updatePosition();
}

CRelationshipItem::~CRelationshipItem()
{
    _relationship = NULL;
    _startItem = NULL;
    _endItem = NULL;
}

int CRelationshipItem::type() const
{
    return Type;
}

CTableItem *CRelationshipItem::startItem() const
{
    return _startItem;
}

void CRelationshipItem::setStartItem(CTableItem *startItem)
{
    if(_startItem)
    {
        _startItem->removeRelationship(this);
    }
    _startItem = startItem;
    _startItem->addRelationship(this);
    updatePosition();
}

CTableItem *CRelationshipItem::endItem() const
{
    return _endItem;
}

void CRelationshipItem::setEndItem(CTableItem *endItem)
{
    if(_endItem)
    {
        _endItem->removeRelationship(this);
    }
    _endItem = endItem;
    _endItem->addRelationship(this);
    updatePosition();
}

void CRelationshipItem::updatePosition()
{
    if (_startItem->collidesWithItem(_endItem))
        return;

    prepareGeometryChange();

    // find start and end points
    QLineF line(mapFromItem(_startItem, _startItem->width() / 2, _startItem->height() / 2),
                mapFromItem(_endItem, _endItem->width() / 2, _endItem->height() / 2));
    _line = line;
    _startPoint = findIntersectionPoint(line, _startItem);
    _endPoint = findIntersectionPoint(line, _endItem);

    // find middle line points
    QRectF rect(_startPoint, _endPoint);
    if(abs(rect.width()) > abs(rect.height()))
    {
        // from top to bottom
        _midPoint1 = QPointF(rect.topLeft().x() + rect.width() / 2, rect.topLeft().y());
        _midPoint2 = QPointF(rect.topLeft().x() + rect.width() / 2, rect.bottomRight().y());
    }
    else
    {
        // from left to right
        _midPoint1 = QPointF(rect.topLeft().x(), rect.topLeft().y() + rect.height() / 2);
        _midPoint2 = QPointF(rect.bottomRight().x(), rect.topLeft().y() + rect.height() / 2);
    }

    updatePolygons();
}

void CRelationshipItem::updatePolygons()
{
    prepareGeometryChange();
    _startMaxPolygon.clear();
    _endMaxPolygon.clear();
    qreal delta = 6;
    switch (_relationship->startType()) {
    case CRelationship::MANY:
        _startMaxPolygon = createManyPolygon(_line, _startItem, delta);
        break;
    case CRelationship::AGGREGATE:
        _startMaxPolygon = createAggregatePolygon(_line, _startItem, delta);
        break;
    case CRelationship::ARROW:
        _startMaxPolygon = createArrowPolygon(_line, _startItem, delta);
        break;
    default:
        break;
    }
    switch (_relationship->endType()) {
    case CRelationship::MANY:
        _endMaxPolygon = createManyPolygon(_line, _endItem, delta);
        break;
    case CRelationship::AGGREGATE:
        _endMaxPolygon = createAggregatePolygon(_line, _endItem, delta);
        break;
    case CRelationship::ARROW:
        _endMaxPolygon = createArrowPolygon(_line, _endItem, delta);
        break;
    default:
        break;
    }

    _startMinCenterPoint = findMinCenterPoint(_line, _startItem, delta);
    _endMinCenterPoint = findMinCenterPoint(_line, _endItem, delta);
}

QRectF CRelationshipItem::boundingRect() const
{
    qreal extra = 40;

    return QRectF(_line.p1(), QSizeF(_line.p2().x() - _line.p1().x(),
                                    _line.p2().y() - _line.p1().y()))
            .normalized()
            .adjusted(-extra, -extra, extra, extra);
}

QPainterPath CRelationshipItem::shape() const
{
    QPainterPath path;
    path.addPolygon(_startMaxPolygon);
    path.addPolygon(_endMaxPolygon);
    int a = 5;
    path.addRect(QRectF(_startPoint, QSizeF(_midPoint1.x() - _startPoint.x(),
                                           _midPoint1.y() - _startPoint.y()))
                 .normalized()
                 .adjusted(-a, -a, a, a));
    path.addRect(QRectF(_midPoint1, QSizeF(_midPoint2.x() - _midPoint1.x(),
                                           _midPoint2.y() - _midPoint1.y()))
                 .normalized()
                 .adjusted(-a, -a, a, a));
    path.addRect(QRectF(_midPoint2, QSizeF(_endPoint.x() - _midPoint2.x(),
                                           _endPoint.y() - _midPoint2.y()))
                 .normalized()
                 .adjusted(-a, -a, a, a));
    int d = 3;
    path.addEllipse(_startMinCenterPoint, d, d);
    path.addEllipse(_endMinCenterPoint, d, d);
    return path;
}

void CRelationshipItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *)
{
    if (_startItem->collidesWithItem(_endItem))
        return;

    prepareGeometryChange();
    updatePolygons();

    // colored outline if selected or mouseovered
    if((option->state & QStyle::State_Selected) ||
        (option->state & QStyle::State_MouseOver))
    {
        QColor penColor;
        if(option->state & QStyle::State_Selected)
            penColor.setRgb(61, 141, 253);
        if(option->state & QStyle::State_MouseOver)
            penColor.setRgb(206, 70, 72);
        painter->setPen(QPen(penColor.light(), 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        painter->drawLine(_startPoint, _midPoint1);
        painter->drawLine(_midPoint1, _midPoint2);
        painter->drawLine(_midPoint2, _endPoint);
    }

    // main line
    painter->setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->setBrush(Qt::NoBrush);
    painter->drawLine(_startPoint, _midPoint1);
    painter->drawLine(_midPoint1, _midPoint2);
    painter->drawLine(_midPoint2, _endPoint);

    // endline shapes
    painter->drawPolygon(_startMaxPolygon);
    painter->drawPolygon(_endMaxPolygon);

    if(_relationship->startType() != CRelationship::ARROW &&
       _relationship->endType() != CRelationship::ARROW)
    {
        qreal d = 3;
        painter->setBrush(Qt::white);
        if(_relationship->startMandatory())
        {
            painter->drawLine(QPointF(_startMinCenterPoint.x(), _startMinCenterPoint.y() - d),
                              QPointF(_startMinCenterPoint.x(), _startMinCenterPoint.y() + d));
            painter->drawLine(QPointF(_startMinCenterPoint.x() - d, _startMinCenterPoint.y()),
                              QPointF(_startMinCenterPoint.x() + d, _startMinCenterPoint.y()));
        }
        else
        {
            painter->drawEllipse(_startMinCenterPoint, d, d);
        }
        if(_relationship->endMandatory())
        {
            painter->drawLine(QPointF(_endMinCenterPoint.x(), _endMinCenterPoint.y() - d),
                              QPointF(_endMinCenterPoint.x(), _endMinCenterPoint.y() + d));
            painter->drawLine(QPointF(_endMinCenterPoint.x() - d, _endMinCenterPoint.y()),
                              QPointF(_endMinCenterPoint.x() + d, _endMinCenterPoint.y()));
        }
        else
        {
            painter->drawEllipse(_endMinCenterPoint, d, d);
        }
    }
}

CRelationship *CRelationshipItem::relationship() const
{
    return _relationship;
}

QPointF CRelationshipItem::findIntersectionPoint(const QLineF &line, const CTableItem *item) const
{
    QLineF centerLine(line);
    QPolygonF polygon = item->polygon();
    QPointF p1 = polygon.first() + item->pos();
    QPointF p2;    
    QPointF intersectPoint;
    QLineF polyLine;
    for (int i = 1; i < polygon.count(); ++i) {
        p2 = polygon.at(i) + item->pos();
        polyLine = QLineF(p1, p2);
        QLineF::IntersectType intersectType =
            polyLine.intersect(centerLine, &intersectPoint);
        if (intersectType == QLineF::BoundedIntersection)
            break;
        p1 = p2;
    }
    return intersectPoint;
}

int CRelationshipItem::findIntersectionSide(const QLineF &line, const CTableItem *item) const
{
    QLineF centerLine(line);
    QPolygonF polygon = item->polygon();
    QPointF p1 = polygon.first() + item->pos();
    QPointF p2;
    QPointF intersectPoint;
    QLineF polyLine;
    for (int i = 1; i < polygon.count(); ++i) {
        p2 = polygon.at(i) + item->pos();
        polyLine = QLineF(p1, p2);
        QLineF::IntersectType intersectType =
            polyLine.intersect(centerLine, &intersectPoint);
        if (intersectType == QLineF::BoundedIntersection)
            return i;
        p1 = p2;
    }
    return -1;
}

QPolygonF CRelationshipItem::createManyPolygon(const QLineF &line, const CTableItem *item, qreal delta) const
{
    QPolygonF polygon;
    QPointF p1, p2, p3;

    QPointF intersectionPoint = findIntersectionPoint(line, item);
    int intersectionSide = findIntersectionSide(line, item);

    switch (intersectionSide) {
    case 1:
        //top
        p1 = QPointF(intersectionPoint.x(), intersectionPoint.y() - 3 * delta);
        p2 = QPointF(intersectionPoint.x() + delta, intersectionPoint.y());
        p3 = QPointF(intersectionPoint.x() - delta, intersectionPoint.y());
        break;
    case 2:
        //right
        p1 = QPointF(intersectionPoint.x() + 3 * delta, intersectionPoint.y());
        p2 = QPointF(intersectionPoint.x(), intersectionPoint.y() + delta);
        p3 = QPointF(intersectionPoint.x(), intersectionPoint.y() - delta);
        break;
    case 3:
        //bot
        p1 = QPointF(intersectionPoint.x(), intersectionPoint.y() + 3 * delta);
        p2 = QPointF(intersectionPoint.x() - delta, intersectionPoint.y());
        p3 = QPointF(intersectionPoint.x() + delta, intersectionPoint.y());
        break;
    case 4:
        //left
        p1 = QPointF(intersectionPoint.x() - 3 * delta, intersectionPoint.y());
        p2 = QPointF(intersectionPoint.x(), intersectionPoint.y() - delta);
        p3 = QPointF(intersectionPoint.x(), intersectionPoint.y() + delta);
        break;
    default:
        break;
    }
    polygon << p1 << p2 << p3;
    return polygon;
}

QPolygonF CRelationshipItem::createAggregatePolygon(const QLineF &line, const CTableItem *item, qreal delta) const
{
    QPolygonF polygon;
    QPointF p1, p2, p3, l1, l2;

    QPointF intersectionPoint = findIntersectionPoint(line, item);
    int intersectionSide = findIntersectionSide(line, item);

    switch (intersectionSide) {
    case 1:
        //top
        p1 = QPointF(intersectionPoint.x(), intersectionPoint.y() - 3 * delta);
        p2 = QPointF(intersectionPoint.x() - delta, intersectionPoint.y() - delta);
        p3 = QPointF(intersectionPoint.x() + delta, intersectionPoint.y() - delta);
        l1 = QPointF(intersectionPoint.x() - delta, intersectionPoint.y());
        l2 = QPointF(intersectionPoint.x() + delta, intersectionPoint.y());
        break;
    case 2:
        //right
        p1 = QPointF(intersectionPoint.x() + 3 * delta, intersectionPoint.y());
        p2 = QPointF(intersectionPoint.x() + delta, intersectionPoint.y() - delta);
        p3 = QPointF(intersectionPoint.x() + delta, intersectionPoint.y() + delta);
        l1 = QPointF(intersectionPoint.x(), intersectionPoint.y() - delta);
        l2 = QPointF(intersectionPoint.x(), intersectionPoint.y() + delta);
        break;
    case 3:
        //bot
        p1 = QPointF(intersectionPoint.x(), intersectionPoint.y() + 3 * delta);
        p2 = QPointF(intersectionPoint.x() + delta, intersectionPoint.y() + delta);
        p3 = QPointF(intersectionPoint.x() - delta, intersectionPoint.y() + delta);
        l1 = QPointF(intersectionPoint.x() + delta, intersectionPoint.y());
        l2 = QPointF(intersectionPoint.x() - delta, intersectionPoint.y());
        break;
    case 4:
        //left
        p1 = QPointF(intersectionPoint.x() - 3 * delta, intersectionPoint.y());
        p2 = QPointF(intersectionPoint.x() - delta, intersectionPoint.y() + delta);
        p3 = QPointF(intersectionPoint.x() - delta, intersectionPoint.y() - delta);
        l1 = QPointF(intersectionPoint.x(), intersectionPoint.y() + delta);
        l2 = QPointF(intersectionPoint.x(), intersectionPoint.y() - delta);
        break;
    default:
        break;
    }
    polygon << p2 << p3 << p1 << p2 << l1 << l2 << p3;
    return polygon;
}

QPolygonF CRelationshipItem::createArrowPolygon(const QLineF &line, const CTableItem *item, qreal delta) const
{
    QPolygonF polygon;
    QPointF p1, p2, p3;

    QPointF intersectionPoint = findIntersectionPoint(line, item);
    int intersectionSide = findIntersectionSide(line, item);

    switch (intersectionSide) {
    case 1:
        //top
        p1 = QPointF(intersectionPoint.x(), intersectionPoint.y());
        p2 = QPointF(intersectionPoint.x() - delta, intersectionPoint.y() - 3 * delta);
        p3 = QPointF(intersectionPoint.x() + delta, intersectionPoint.y() - 3 * delta);
        break;
    case 2:
        //right
        p1 = QPointF(intersectionPoint.x(), intersectionPoint.y());
        p2 = QPointF(intersectionPoint.x() + 3 * delta, intersectionPoint.y() - delta);
        p3 = QPointF(intersectionPoint.x() + 3 * delta, intersectionPoint.y() + delta);
        break;
    case 3:
        //bot
        p1 = QPointF(intersectionPoint.x(), intersectionPoint.y());
        p2 = QPointF(intersectionPoint.x() - delta, intersectionPoint.y() + 3 * delta);
        p3 = QPointF(intersectionPoint.x() + delta, intersectionPoint.y() + 3 * delta);
        break;
    case 4:
        //left
        p1 = QPointF(intersectionPoint.x(), intersectionPoint.y());
        p2 = QPointF(intersectionPoint.x() - 3 * delta, intersectionPoint.y() - delta);
        p3 = QPointF(intersectionPoint.x() - 3 * delta, intersectionPoint.y() + delta);
        break;
    default:
        break;
    }
    polygon << p2 << p1 << p3 << p1 << p2;
    return polygon;
}

QPointF CRelationshipItem::findMinCenterPoint(const QLineF &line, const CTableItem *item, qreal delta) const
{
    QPointF centerPoint;

    QPointF intersectionPoint = findIntersectionPoint(line, item);
    int intersectionSide = findIntersectionSide(line, item);

    switch (intersectionSide) {
    case 1:
        //top
        centerPoint = QPointF(intersectionPoint.x(), intersectionPoint.y() - 4 * delta);
        break;
    case 2:
        //right
        centerPoint = QPointF(intersectionPoint.x() + 4 * delta, intersectionPoint.y());
        break;
    case 3:
        //bot
        centerPoint = QPointF(intersectionPoint.x(), intersectionPoint.y() + 4 * delta);
        break;
    case 4:
        //left
        centerPoint = QPointF(intersectionPoint.x() - 4 * delta, intersectionPoint.y());
        break;
    default:
        break;
    }
    return centerPoint;
}
