#ifndef CFOREIGNROW_H
#define CFOREIGNROW_H

#include <QString>


class CRow;

class CForeignRow
{
public:
    CForeignRow();

    QString name() const;
    void setName(const QString &name);
    CRow *row() const;
    void setRow(CRow *row);
    int tableId() const;
    void setTableId(int tableId);

private:
    QString _name;
    CRow *_row;
    int _tableId;
};

#endif // CFOREIGNROW_H
