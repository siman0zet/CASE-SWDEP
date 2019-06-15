#include "CTable.h"
#include "CRow.h"
#include "CForeignRow.h"
#include "CRelationship.h"
#include "CUniqueGroup.h"

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

QString CTable::getDataAsText() const
{
    QString text;
    /*  r Row_name type[(size)] [PK/NN/UQ]
     *  row ...
     *  frow Row_name Table_Name [PK]
     *  frow ...
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
    foreach (const CForeignRow *fRow, this->foreingRows()) {
        text += QString("frow %1 %2")
                .arg(fRow->row()->name())
                .arg(fRow->tableName());
        if(fRow->primaryKey())
            text += " PK";
        text += "\n";
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
