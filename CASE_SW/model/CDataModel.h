#ifndef CDATAMODEL_H
#define CDATAMODEL_H

#include <QObject>
#include <QMap>

class CObject;
class CTable;
class CRelationship;

class CDataModel : public QObject
{
    Q_OBJECT

public:
    CDataModel();

    CTable *addTable();
    CRelationship *addRelationship(int startId, int endId);
    void removeObjects(const QList<CObject *> &objects);
    void flipTables(int id);
    void changeRelationshipTable(int relationshipId, int tableId, bool start = false);
    QString changeTabelName(int tableId, const QString &name);

    QList<CTable *> listTables() const;

    bool isPhysical() const;
    void setPhysical(bool physical);

    QMap<int, CTable *> tables() const;
    QMap<int, CRelationship *> relationships() const;

private:
    bool _physical;

    QMap<int, CTable *> _tables;
    QMap<int, CRelationship *> _relationships;

    int _countTables;
    int _countRelationships;

    QList<CObject *> _objectsToRemove;

    void removeTable(int id);
    void removeRelationship(int id);

signals:
    void relationshipRemoved(int id);
};

#endif // CDATAMODEL_H
