#include "CTable.h"
#include "CRow.h"
#include "CForeignRow.h"
#include "CRelationship.h"
#include "CUniqueGroup.h"

CTable::CTable(int id) :
    CObject(id)
{
    this->setName(QString("Table %1").arg(id));
}

CTable::~CTable()
{
    foreach (CRelationship *relationship, _relationships) {
        delete relationship;
    }
    foreach (CRow *row, _rows) {
        delete row;
    }
    foreach (CForeignRow *foreignRow, _foreingRows) {
        delete foreignRow;
    }
}

int CTable::type()
{
    return Type;
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

void CTable::addRow(CRow *row)
{
    _rows.append(row);
}

void CTable::removeRow(int index)
{
    _rows.removeAt(index);
}

CRow *CTable::row(int index)
{
    return _rows.at(index);
}

QString CTable::changeRowName(int index, QString name)
{
    foreach(CRow *row, _rows)
    {
        if(row->name() == name)
            return _rows.at(index)->name();
    }
    _rows.at(index)->setName(name);
    return name;
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

QList<CRow *> CTable::rows() const
{
    return _rows;
}

QList<CUniqueGroup *> CTable::uniqueGroups() const
{
    return _uniqueGroups;
}

void CTable::setUniqueGroups(const QList<CUniqueGroup *> &uniqueGroups)
{
    _uniqueGroups = uniqueGroups;
    foreach (CUniqueGroup *ugroup, _uniqueGroups) {
        foreach (QString name, ugroup->rows()) {
            foreach (CRow *row, _rows) {
                if(name == row->name())
                    row->setUnique(true);
            }
        }
    }
}

