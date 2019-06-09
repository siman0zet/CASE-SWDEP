#include "CForeignRow.h"

CForeignRow::CForeignRow()
{

}

QString CForeignRow::name() const
{
    return _name;
}

void CForeignRow::setName(const QString &name)
{
    _name = name;
}

CRow *CForeignRow::row() const
{
    return _row;
}

void CForeignRow::setRow(CRow *row)
{
    _row = row;
}

int CForeignRow::tableId() const
{
    return _tableId;
}

void CForeignRow::setTableId(int tableId)
{
    _tableId = tableId;
}
