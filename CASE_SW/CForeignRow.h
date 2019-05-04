#ifndef CFOREIGNROW_H
#define CFOREIGNROW_H

#include "CRow.h"

class CForeignRow: public CRow
{
    friend class CEntity;
    friend class CDataModel;
public:
    CForeignRow() {}
    ~CForeignRow() {}

    int getForeignTable() const
    {
        return foreign_table_id;
    }

    int getForeignRowID() const
    {
        return foreign_row_id;
    }

    void setForeignName(QString str)
    {
        foreign_name = str;
    }

    QString getForeignName() const
    {
        return foreign_name;
    }

    void setForeignRowName(QString str)
    {
        foreign_row_name = str;
    }

    QString getForeignRowName() const
    {
        return foreign_row_name;
    }

private:
    void setForeignTable(int id, int frid); // Задать таблицу, на которыю ссылается строка

    int foreign_table_id;
    int foreign_row_id;

    QString foreign_name;
    QString foreign_row_name;
};

#endif // CFOREIGNROW_H
