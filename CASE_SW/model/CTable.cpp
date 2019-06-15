#include "CTable.h"
#include "CRow.h"
#include "CForeignRow.h"
#include "CRelationship.h"
#include "CUniqueGroup.h"

#include <QTextStream>

CTable::CTable(const QString &name) :
    CObject(name)
{
}

CTable::CTable(const CTable *table) :
    CObject(table->name())
{
    foreach (const CRow *row, table->rows()) {
        _rows.append(new CRow(row));
    }
    foreach (const CUniqueGroup *uGroup, table->uniqueGroups()) {
        _uniqueGroups.append(new CUniqueGroup(uGroup));
    }
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

int CTable::type() const
{
    return Type;
}

QString CTable::exportDataToText() const
{
    QString text;
    /*  row Row_name type[(size)] [PK/NN/UQ]
     *  row ...
     *  ugroup uGroup_name Row_name1/Row_name2/...
     *  ugroup ...
     */
    foreach (const CRow *row, this->rows()) {
        QString constraint;
        if(row->primaryKey())
            constraint += "PK";
        if(row->notNull())
        {
            if(constraint.size() > 0)
                constraint += "/NN";
            else
                constraint += "NN";
        }
        if(row->unique())
        {
            if(constraint.size() > 0)
                constraint += "/UQ";
            else
                constraint += "UQ";
        }
        text += QString("row %1 %2 %3\n")
                .arg(row->name())
                .arg(row->typeAsString())
                .arg(constraint);
    }
    foreach (const CUniqueGroup *uGroup, this->uniqueGroups()) {
        QString rows;
        foreach (QString rowName, uGroup->rows()) {
            rows += rowName;
            if(rowName != uGroup->rows().last())
                rows += "/";
        }
        text += QString("ugroup %1 %2\n")
                .arg(uGroup->name())
                .arg(rows);
    }
    return text;
}

void CTable::importFromTextStream(QTextStream &input)
{
    while(!input.atEnd())
    {
        QStringList strList = input.readLine().split(" ");
        /*  row Row_name type[(size)] [PK/NN/UQ]
         *  row ...
         *  ugroup uGroup_name Row_name1/Row_name2/...
         *  ugroup ...
         */
        if(strList.at(0) == "")
            return;
        if(strList.at(0) == "row")
        {
            if(strList.size() >= 3)
            {
                CRow *row = new CRow(strList.at(1));
                row->setTypeFromString(strList.at(2));
                if(strList.size() == 4)
                {
                    QString t = strList.at(3);
                    QStringList list = t.split("/");
                    foreach (const QString &str, list) {
                        if(str == "PK")
                        {
                            row->setPrimaryKey(true);
                            continue;
                        }
                        if(str == "NN")
                        {
                            row->setNotNull(true);
                            continue;
                        }
                        if(str == "UQ")
                        {
                            row->setUnique(true);
                            break;
                        }
                    }
                }
                _rows.append(row);
            }
        }
        if(strList.at(0) == "ugroup")
        {
            if(strList.size() == 3)
            {
                CUniqueGroup *uGroup = new CUniqueGroup(strList.at(1));
                QString t = strList.at(2);
                QStringList rowList = t.split("/");
                uGroup->setRows(rowList);
                _uniqueGroups.append(uGroup);
            }
        }
    }
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
    if(index >= 0 && index < _rows.size())
        _rows.removeAt(index);
}

CRow *CTable::row(int index)
{
    if(index >= 0 && index < _rows.size())
        return _rows.at(index);
    else
        return NULL;
}

QString CTable::changeRowName(int index, const QString &newName)
{
    if(index >= 0 && index < _rows.size())
    {
        foreach (const CRow *row, _rows) {
            if(row->name() == newName)
                return _rows.at(index)->name();
        }
        _rows.at(index)->setName(newName);
        return newName;
    }
    else
        return "";
}

void CTable::addForeignRow(CForeignRow *fRow)
{
    _foreingRows.append(fRow);
}

CForeignRow *CTable::foreignRow(int index)
{
    if(index >= 0 && index < _foreingRows.size())
        return _foreingRows.at(index);
    else
        return NULL;
}

int CTable::rowCount() const
{
    return _rows.size();
}

int CTable::foreignRowCount() const
{
    return _foreingRows.size();
}

int CTable::totalRowCount() const
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

QList<CRow *> CTable::primaryKey() const
{
    QList<CRow *> pkList;
    foreach (CRow *row, _rows) {
        if(row->primaryKey())
            pkList.append(row);
    }
    return pkList;
}

QList<CUniqueGroup *> CTable::uniqueGroups() const
{
    return _uniqueGroups;
}

void CTable::setUniqueGroups(const QList<CUniqueGroup *> &uniqueGroups)
{
    _uniqueGroups = uniqueGroups;
}

QList<CForeignRow *> CTable::foreingRows() const
{
    return _foreingRows;
}
