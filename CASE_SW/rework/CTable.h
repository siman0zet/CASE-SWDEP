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
    enum { Type = USER_TYPE + 11 };

    explicit CTable(int id);
    ~CTable();

    int type() override;

    void addRelationship(CRelationship *relationship);
    void removeRelationship(CRelationship *relationship);
    void removeRelationships();

    int addRow();
    void removeRow(int row);

    void addForeignRow();

    int rowCount();
    int foreignRowCount();
    int totalRowCount();

    QList<CRelationship *> relationships() const;

private:
    QList<CRelationship *> _relationships;
    QMap<int, CRow *> _rows;
    QMap<int, CForeignRow *> _foreingRows;
};

#endif // CTABLE_H
