#ifndef CTABLE_H
#define CTABLE_H

#include "CObject.h"

#include <QMap>
#include <QString>

class CRelationship;
class CRow;
class CForeignRow;

class CTable : public CObject
{
public:
    CTable(int id);

    QString name() const;
    void setName(const QString &name);

    int addRow();
    void removeRow(int row);

    void addForeignRow();

    int rowCount();
    int foreignRowCount();
    int totalRowCount();

private:
    QString _name;

    QMap<int, CRelationship *> _relationships;
    QMap<int, CRow *> _rows;
    QMap<int, CForeignRow *> _foreingRows;
};

#endif // CTABLE_H
