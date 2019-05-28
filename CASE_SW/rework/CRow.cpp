#include "CRow.h"

CRow::CRow(const QString &name) :
    _name(name),
    _type(CRow::INTEGER),
    _size(-1),
    _primaryKey(false),
    _notNull(false),
    _unique(false)
{

}

CRow::CRow(const QString &name, CRow::DATA_TYPE type, int size, bool primaryKey, bool notNull, bool unique)
{
    _name = name;
    _type = type;
    _size = size;
    _primaryKey = primaryKey;
    _notNull = notNull;
    _unique = unique;
}

QString CRow::name() const
{
    return _name;
}

void CRow::setName(const QString &name)
{
    _name = name;
}

CRow::DATA_TYPE CRow::type() const
{
    return _type;
}

void CRow::setType(const DATA_TYPE &type)
{
    _type = type;
}

int CRow::size() const
{
    return _size;
}

void CRow::setSize(int size)
{
    _size = size;
}

bool CRow::primaryKey() const
{
    return _primaryKey;
}

void CRow::setPrimaryKey(bool primaryKey)
{
    _primaryKey = primaryKey;
}

bool CRow::notNull() const
{
    return _notNull;
}

void CRow::setNotNull(bool notNull)
{
    _notNull = notNull;
}

bool CRow::unique() const
{
    return _unique;
}

void CRow::setUnique(bool unique)
{
    _unique = unique;
}
