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
    void changeTable(int relationshipId, int tableId, bool start = false);
    QList<CTable *> listTables();

    CDataModel *convertToPdm();

    bool isPhysical();

private:
    bool _physical;

    QMap<int, CTable *> _tables;
    QMap<int, CRelationship *> _relationships;

    int _tablesCount;
    int _relationshipsCount;

    QList<CObject *> _objectsToRemove;

    void removeTable(int id);
    void removeRelationship(int id);

signals:
    void relationshipRemoved(int id);
};

#endif // CDATAMODEL_H
