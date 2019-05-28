#include "CDataModel.h"
#include "CTable.h"
#include "CRelationship.h"

CDataModel::CDataModel() :
    _physical(false),
    _tablesCount(0),
    _relationshipsCount(0)
{

}

CTable *CDataModel::addTable()
{
    CTable *table = new CTable(++_tablesCount);
    _tables.insert(table->id(), table);
    return table;
}

CRelationship *CDataModel::addRelationship(int startId, int endId)
{
    // primary key existant in end table (?) check
    CRelationship *relationship = new CRelationship(++_relationshipsCount,
                                                    _tables.value(startId),
                                                    _tables.value(endId));
    _relationships.insert(relationship->id(), relationship);
    return relationship;
}

void CDataModel::removeObjects(const QList<CObject *> &objects)
{
    foreach (CObject *object, objects) {
        if(CRelationship::Type == object->type())
            removeRelationship(object->id());
        if(CTable::Type == object->type())
            removeTable(object->id());
    }
}

CDataModel *CDataModel::convertToPdm()
{

}

bool CDataModel::isPhysical()
{
    return _physical;
}

void CDataModel::removeTable(int id)
{
    CTable *table = _tables.value(id);
    foreach (CRelationship *relationship, table->relationships()) {
        removeRelationship(relationship->id());
    }
    delete _tables.value(id);
    _tables.remove(id);
}

void CDataModel::removeRelationship(int id)
{
    delete _relationships.value(id);
    _relationships.remove(id);
    emit relationshipRemoved(id);
}
