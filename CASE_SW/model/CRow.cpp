#include "CRow.h"

CRow::CRow(const QString &name) :
    _name(name),
    _type(CRow::INTEGER),
    _size(10),
    _primaryKey(false),
    _notNull(false),
    _unique(false)
{

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

QString CRow::typeAsString()
{
    switch (_type) {
    case INTEGER:
        return "INTEGER";
        break;
    case FLOAT:
        return "FLOAT";
        break;
    case CHAR:
        return QString("CHAR(%1)").arg(_size);
        break;
    case VARCHAR:
        return QString("VARCHAR(%1)").arg(_size);
        break;
    case BOOLEAN:
        return "BOOLEAN";
        break;
    case DATE:
        return "DATE";
        break;
    case BLOB:
        return "BLOB";
        break;
    }
    return "";
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
