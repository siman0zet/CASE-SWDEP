#ifndef CTABLE_H
#define CTABLE_H

#include "itable.h"

#include <QMap>
#include <QString>

class CRelationship;
class CRow;
class CForeignRow;
class CUniqueGroup;
class QTextStream;

class CTable : public ITable
{
public:
    enum { Type = USER_TYPE + 11 };

    explicit CTable(const QString &name);
    explicit CTable(const CTable *table);
    ~CTable();

    int type() const override;

    QString exportDataToText() const override;
    void importFromTextStream(QTextStream &input) override;

    void addRelationship(CRelationship *relationship);
    void removeRelationship(CRelationship *relationship);
    void removeRelationships();

    void addRow(CRow *row);
    void removeRow(int index);
    CRow *row(int index);
    QString changeRowName(int index, const QString &newName);

    void addForeignRow(CForeignRow *fRow);
    CForeignRow *foreignRow(int index);
    //---------------------------------------------------
    void removeForeginRow(CForeignRow *fRow);
    void removeForeginRow(int indexFRow);
    QString exportForeginKeysToText();
    void importForeginKeysFromTS(QStringList strList, CTable *fgTable);
    bool SearchFROnName(QString nameFR);
    //---------------------------------------------------

    int rowCount() const;
    int foreignRowCount() const;
    int totalRowCount() const;

    QList<CRelationship *> relationships() const;
    QList<CRow *> rows() const override;
    QList<CForeignRow *> foreingRows() const;
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
