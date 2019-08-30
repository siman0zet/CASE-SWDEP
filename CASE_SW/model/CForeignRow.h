#ifndef CFOREIGNROW_H
#define CFOREIGNROW_H

#include "IForeingRow.h"
#include <QString>

class CRow;

class CForeignRow : public IForeingRow
{
public:
    CForeignRow(CRow *row, const QString &tableName);

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
    bool _primaryKey;
};

#endif // CFOREIGNROW_H
