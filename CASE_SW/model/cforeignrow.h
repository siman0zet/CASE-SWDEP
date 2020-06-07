#ifndef CFOREIGNROW_H
#define CFOREIGNROW_H

#include "iforeingrow.h"
#include <QString>

class CRow;

class CForeignRow : public IForeingRow
{
public:
    CForeignRow(CRow *row, const QString &tableName);
    //-----------------------------------------------
    CForeignRow(const CForeignRow *fRow);
    //-----------------------------------------------

    QString name() const;
    QString typeAsString() const override;
    CRow *row() const;
    void setRow(CRow *row);
    QString tableRowName() const override;
    QString tableName() const override;
    void setTableName(const QString &tableName);
    bool primaryKey() const override;
    void setPrimaryKey(bool primaryKey);

private:
    CRow *_row;
    QString _tableName;
    QString _cuurTableName;
    bool _primaryKey;
};

#endif // CFOREIGNROW_H
