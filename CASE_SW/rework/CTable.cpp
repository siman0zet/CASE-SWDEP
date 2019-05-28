#include "CTable.h"
#include "CRow.h"
#include "CForeignRow.h"

CTable::CTable(int id) :
    CObject(id),
    _name(QString("Table %1").arg(id))
{
}

QString CTable::name() const
{
    return _name;
}

void CTable::setName(const QString &name)
{
    _name = name;
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
