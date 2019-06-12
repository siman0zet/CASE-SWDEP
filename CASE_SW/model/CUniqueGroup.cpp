#include "CUniqueGroup.h"
#include <QString>

CUniqueGroup::CUniqueGroup(QString name) :
    _name(name)
{

}

QString CUniqueGroup::name() const
{
    return _name;
}

void CUniqueGroup::setName(const QString &name)
{
    _name = name;
}

QList<QString> CUniqueGroup::rows() const
{
    return _rows;
}

void CUniqueGroup::addRow(const QString &row)
{
    _rows.append(row);
}

void CUniqueGroup::removeRow(const QString &row)
{
    int index = _rows.indexOf(row);
    if(index != -1)
        _rows.removeAt(index);
}
