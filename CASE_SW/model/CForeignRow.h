#ifndef CFOREIGNROW_H
#define CFOREIGNROW_H

#include <QString>


class CRow;

class CForeignRow
{
public:
    CForeignRow(CRow *row, const QString &tableName);

    QString name() const;
    QString typeAsString() const;
    CRow *row() const;
    void setRow(CRow *row);
    QString tableName() const;
    void setTableName(const QString &tableName);
    bool primaryKey() const;
    void setPrimaryKey(bool primaryKey);

private:
    CRow *_row;
    QString _tableName;
    bool _primaryKey;
};

#endif // CFOREIGNROW_H
