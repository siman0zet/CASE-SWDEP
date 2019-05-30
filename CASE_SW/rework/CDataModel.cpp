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
    foreach (CRelationship *relationship, _relationships.values()) {
        if((relationship->startTable()->id() == startId ||
            relationship->endTable()->id() == startId) &&
           (relationship->startTable()->id() == endId ||
            relationship->endTable()->id() == endId))
        {
            return NULL;    //relationship between this tables already exist
        }
    }
    CRelationship *relationship = new CRelationship(++_relationshipsCount,
                                                    _tables.value(startId),
                                                    _tables.value(endId));
    _relationships.insert(relationship->id(), relationship);
    return relationship;
}

void CDataModel::removeObjects(const QList<CObject *> &objects)
{
    // Cycle counter gets reset after removing an object
    // since when table is removed, all relationships to it are removed in cascade
    // leading to pointer to deleted object appearing in the list
    _objectsToRemove = objects;
    for(int i = 0; i < _objectsToRemove.size(); i++)
    {
        if(CRelationship::Type == _objectsToRemove[i]->type())
        {
            removeRelationship(_objectsToRemove[i]->id());
            i = 0;
            continue;
        }
        if(CTable::Type == _objectsToRemove[i]->type())
        {
            removeTable(_objectsToRemove[i]->id());
            i = 0;
            continue;
        }
    }
    _objectsToRemove.clear();
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
    int index = _objectsToRemove.indexOf(_tables.value(id));
    if(index != -1)
        _objectsToRemove.removeAt(index);

    CTable *table = _tables.value(id);
    foreach (CRelationship *relationship, table->relationships()) {
        removeRelationship(relationship->id());
    }
    delete _tables.value(id);
    _tables.remove(id);
}

void CDataModel::removeRelationship(int id)
{
    int index = _objectsToRemove.indexOf(_relationships.value(id));
    if(index != -1)
        _objectsToRemove.removeAt(index);

    delete _relationships.value(id);
    _relationships.remove(id);
    emit relationshipRemoved(id);
}
