#include "CObject.h"

CObject::CObject(const QString &name) :
    _name(name)
{
    
}

int CObject::type() const
{
    return Type;
}

QString CObject::exportDataToText() const
{
    return "";
}

QString CObject::name() const
{
    return _name;
}

void CObject::setName(const QString &name)
{
    _name = name;
}
