#ifndef CDATAMODEL_H
#define CDATAMODEL_H

#include "idatamodel.h"
#include <QMap>

class CObject;
class CTable;
class CRelationship;

class CDataModel : public IDataModel
{
    Q_OBJECT

public:
    CDataModel();

    CDataModel *convertToPhysical();

    CTable *addTable();
    void addTable(CTable *table);
    CRelationship *addRelationship(const QString &startName, const QString &endName);
    void addRelationship(CRelationship *relationship);
    void removeObjects(const QList<CObject *> &objects);
    void flipTables(const QString &relationshipName);
    void changeRelationshipTable(const QString &relationshipName,
                                 const QString &tableName,
                                 bool start = false);
    QString changeTabelName(const QString &oldName, const QString &newName);
    /*void addForeingTableConnection(const CTable *startTable,
                                   const CTable *endTable,
                                   bool identifying = false);*/
    void addConduitTableConnection(const CTable *startTable,
                                   const CTable *endTable,
                                   bool identifying = false);

    QList<CTable *> listTables() const;

    bool isPhysical() const;
    void setPhysical(bool physical);

    QMap<QString, CTable *> tables() const override;
    QMap<QString, CRelationship *> relationships() const;
    QList<CTable *> tablesSortedByReference() const override;
    void sortTablesByReference(CTable *table, QList<CTable *> &list) const;

private:
    bool _physical;

    QMap<QString, CTable *> _tables;
    QMap<QString, CRelationship *> _relationships;

    int _countTables;

    QList<CObject *> _objectsToRemove;

    void removeTable(const QString &name);
    void removeRelationship(const QString &name);

signals:
    void tableNameChanged(const QString &oldName, const QString &newName);
};

#endif // CDATAMODEL_H
