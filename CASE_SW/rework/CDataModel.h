#ifndef CDATAMODEL_H
#define CDATAMODEL_H

#include <QMap>

class CTable;
class CRelationship;

class CDataModel
{
public:
    CDataModel();

    CTable *addTable();
    CRelationship *addRelationship(int startId, int endId);
//    void removeAllThisShite();

    CDataModel *convertToPdm();

    bool isPhysical();

private:
    bool _physical;

    QMap<int, CTable *> _tables;
    QMap<int, CRelationship *> _relationships;

    int _tablesCount;
    int _relationshipsCount;
};

#endif // CDATAMODEL_H
