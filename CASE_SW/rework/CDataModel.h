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

    CDataModel *convertToPdm();

    bool isPhysical();

private:
    bool _physical;

    QMap<int, CTable *> _tables;
    QMap<int, CRelationship *> _relationships;

    int _tablesCount;
    int _relationshipsCount;

    void removeTable(int id);
    void removeRelationship(int id);

signals:
    void relationshipRemoved(int id);
};

#endif // CDATAMODEL_H
