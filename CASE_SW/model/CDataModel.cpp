#include "CDataModel.h"
#include "CTable.h"
#include "CRelationship.h"

CDataModel::CDataModel() :
    _physical(false),
    _countTables(0),
    _countRelationships(0)
{
}

CTable *CDataModel::addTable()
{
    int id = ++_countTables;
    for(int i = 0; i < _tables.size(); i++)
    {
        CTable *table = _tables.values().at(i);
        if(table->name() == QString("Table_%1").arg(id))
        {
            id = ++_countTables;
            i = 0;
        }
    }
    CTable *table = new CTable(id);
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
    CRelationship *relationship = new CRelationship(++_countRelationships,
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

void CDataModel::flipTables(int id)
{
    CTable *table = _relationships.value(id)->startTable();
    _relationships.value(id)->setStartTable(_relationships.value(id)->endTable());
    _relationships.value(id)->setEndTable(table);
}

void CDataModel::changeRelationshipTable(int relationshipId, int tableId, bool start)
{
    if(start)
    {
        _relationships.value(relationshipId)->setStartTable(_tables.value(tableId));
    }
    else
    {
        _relationships.value(relationshipId)->setEndTable(_tables.value(tableId));
    }
}

QString CDataModel::changeTabelName(int tableId, QString name)
{
    foreach(CTable *table, _tables)
    {
        if(table->name() == name)
            return _tables.value(tableId)->name();
    }
    _tables.value(tableId)->setName(name);
    return name;
}

QList<CTable *> CDataModel::listTables()
{
    return _tables.values();
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
