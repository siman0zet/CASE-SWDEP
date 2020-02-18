#include "crow.h"

#include <QRegExpValidator>

CRow::CRow(const QString &name) :
    _name(name),
    _type(CRow::INTEGER),
    _size(10),
    _primaryKey(false),
    _notNull(false),
    _unique(false)
{

}

CRow::CRow(const CRow *row) :
    _name(row->name()),
    _type(row->type()),
    _size(row->size()),
    _primaryKey(row->primaryKey()),
    _notNull(row->notNull()),
    _unique(row->unique())
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

QString CRow::typeAsString() const
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

void CRow::setTypeFromString(QString type)
{
    QRegExpValidator validator;
    int size = type.size();

    validator.setRegExp(QRegExp("INTEGER", Qt::CaseInsensitive));
    if(validator.validate(type, size) == QValidator::Acceptable)
    {
        this->setType(INTEGER);
        this->setSize(10);
    }
    validator.setRegExp(QRegExp("FLOAT", Qt::CaseInsensitive));
    if(validator.validate(type, size) == QValidator::Acceptable)
    {
        this->setType(FLOAT);
        this->setSize(10);
    }
    validator.setRegExp(QRegExp("BOOLEAN", Qt::CaseInsensitive));
    if(validator.validate(type, size) == QValidator::Acceptable)
    {
        this->setType(BOOLEAN);
        this->setSize(10);
    }
    validator.setRegExp(QRegExp("DATE", Qt::CaseInsensitive));
    if(validator.validate(type, size) == QValidator::Acceptable)
    {
        this->setType(DATE);
        this->setSize(10);
    }
    validator.setRegExp(QRegExp("BLOB", Qt::CaseInsensitive));
    if(validator.validate(type, size) == QValidator::Acceptable)
    {
        this->setType(BLOB);
        this->setSize(10);
    }
    validator.setRegExp(QRegExp("CHAR\\([0-9]+\\)", Qt::CaseInsensitive));
    if(validator.validate(type, size) == QValidator::Acceptable)
    {
        QRegExp regExp("\\((.+)\\)", Qt::CaseInsensitive);
        if(regExp.indexIn(type))
        {
            int _size = regExp.cap(1).toInt();
            this->setType(CHAR);
            this->setSize(_size);
        }
    }
    validator.setRegExp(QRegExp("VARCHAR\\([0-9]+\\)", Qt::CaseInsensitive));
    if(validator.validate(type, size) == QValidator::Acceptable)
    {
        QRegExp regExp("\\((.+)\\)", Qt::CaseInsensitive);
        if(regExp.indexIn(type))
        {
            int _size = regExp.cap(1).toInt();
            this->setType(VARCHAR);
            this->setSize(_size);
        }
    }
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
