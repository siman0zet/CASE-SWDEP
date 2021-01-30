#include "cforeignrow.h"
#include "crow.h"

CForeignRow::CForeignRow(CRow *row, const QString &tableName) :
    _row(row),
    _tableName(tableName),
    _primaryKey(false)
{
}

//-------------------------------
CForeignRow::CForeignRow(const CForeignRow *fRow) :
    _row(fRow->row()),
    _tableName(fRow->tableName()),
    _primaryKey(fRow->primaryKey())
{
}
//-------------------------------
QString CForeignRow::name() const
{
    return QString("%1_%2").arg(_tableName).arg(_row->name());
}

QString CForeignRow::typeAsString() const
{
    return _row->typeAsString();
}

CRow *CForeignRow::row() const
{
    return _row;
}

void CForeignRow::setRow(CRow *row)
{
    _row = row;
}

QString CForeignRow::tableRowName() const
{
    return _row->name();
}

QString CForeignRow::tableName() const
{
    return _tableName;
}

void CForeignRow::setTableName(const QString &tableName)
{
    _tableName = tableName;
}

bool CForeignRow::primaryKey() const
{
    return _primaryKey;
}

void CForeignRow::setPrimaryKey(bool primaryKey)
{
    _primaryKey = primaryKey;
}
