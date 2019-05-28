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

CDataModel *CDataModel::convertToPdm()
{

}

bool CDataModel::isPhysical()
{
    return _physical;
}
