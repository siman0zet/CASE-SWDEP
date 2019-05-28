#include "CTable.h"
#include "CRow.h"
#include "CForeignRow.h"
#include "CRelationship.h"

CTable::CTable(int id) :
    CObject(id),
    _name(QString("Table %1").arg(id))
{
}

CTable::~CTable()
{
    foreach (CRelationship *relationship, _relationships) {
        delete relationship;
    }
    foreach (CRow *row, _rows.values()) {
        delete row;
    }
    foreach (CForeignRow *foreignRow, _foreingRows.values()) {
        delete foreignRow;
    }
}

int CTable::type()
{
    return Type;
}

QString CTable::name() const
{
    return _name;
}

void CTable::setName(const QString &name)
{
    _name = name;
}

void CTable::addRelationship(CRelationship *relationship)
{
    _relationships.append(relationship);
}

void CTable::removeRelationship(CRelationship *relationship)
{
    int index = _relationships.indexOf(relationship);
    if(index != -1)
        _relationships.removeAt(index);
}

void CTable::removeRelationships()
{
    foreach (CRelationship *relationship, _relationships)
    {
        relationship->startTable()->removeRelationship(relationship);
        relationship->endTable()->removeRelationship(relationship);
        delete relationship;
    }
}

int CTable::addRow()
{
}

int CTable::rowCount()
{
    return _rows.size();
}

int CTable::foreignRowCount()
{
    return _foreingRows.size();
}

int CTable::totalRowCount()
{
    return _rows.size() + _foreingRows.size();
}

QList<CRelationship *> CTable::relationships() const
{
    return _relationships;
}
