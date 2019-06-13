#ifndef CTABLE_H
#define CTABLE_H

#include "CObject.h"

#include <QMap>
#include <QString>

class CRelationship;
class CRow;
class CForeignRow;
class CUniqueGroup;

class CTable : public CObject
{
public:
    enum { Type = USER_TYPE + 11 };

    explicit CTable(const QString &name);
    explicit CTable(const CTable *table);
    ~CTable();

    int type() const override;

    void addRelationship(CRelationship *relationship);
    void removeRelationship(CRelationship *relationship);
    void removeRelationships();

    void addRow(CRow *row);
    void removeRow(int index);
    CRow *row(int index);
    QString changeRowName(int index, const QString &newName);

    void addForeignRow(CForeignRow *fRow);
    CForeignRow *foreignRow(int index);

    int rowCount() const;
    int foreignRowCount() const;
    int totalRowCount() const;

    QList<CRelationship *> relationships() const;
    QList<CRow *> rows() const;
    QList<CRow *> primaryKey() const;

    QList<CUniqueGroup *> uniqueGroups() const;
    void setUniqueGroups(const QList<CUniqueGroup *> &uniqueGroups);

private:
    QList<CRelationship *> _relationships;
    QList<CRow *> _rows;
    QList<CForeignRow *> _foreingRows;
    QList<CUniqueGroup *> _uniqueGroups;
};

#endif // CTABLE_H
