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

    void addRow(CRow *row);
    void removeRow(int index);
    CRow *row(int index);
    QString changeRowName(int index, QString name);

    void addForeignRow();

    int rowCount();
    int foreignRowCount();
    int totalRowCount();

    QList<CRelationship *> relationships() const;
    QList<CRow *> rows() const;

private:
    QList<CRelationship *> _relationships;
    QList<CRow *> _rows;
    QList<CForeignRow *> _foreingRows;
};

#endif // CTABLE_H
