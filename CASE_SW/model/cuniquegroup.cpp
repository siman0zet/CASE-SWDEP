#include "cuniquegroup.h"
#include <QString>

CUniqueGroup::CUniqueGroup(const QString &name) :
    _name(name)
{

}

CUniqueGroup::CUniqueGroup(const CUniqueGroup *uGroup) :
    _name(uGroup->name()),
    _rows(uGroup->rows())
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

void CUniqueGroup::setRows(const QList<QString> &rows)
{
    _rows = rows;
}
