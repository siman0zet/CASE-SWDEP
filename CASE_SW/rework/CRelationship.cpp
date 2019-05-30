#include "CRelationship.h"
#include "CTable.h"

CRelationship::CRelationship(int id, CTable *startTable, CTable *endTable) :
    CObject(id),
    _startTable(startTable),
    _endTable(endTable),
    _startType(ONE),
    _endType(ONE),
    _startMandatory(true),
    _endMandatory(true)
{
    _startTable->addRelationship(this);
    _endTable->addRelationship(this);
}

CRelationship::~CRelationship()
{
    _startTable->removeRelationship(this);
    _endTable->removeRelationship(this);
    _startTable = nullptr;
    _endTable = nullptr;
}

int CRelationship::type()
{
    return Type;
}

CRelationship::RELATIONSHIP_TYPE CRelationship::startType() const
{
    return _startType;
}

void CRelationship::setStartType(const RELATIONSHIP_TYPE &startMaxType)
{
    _startType = startMaxType;
}

CRelationship::RELATIONSHIP_TYPE CRelationship::endType() const
{
    return _endType;
}

void CRelationship::setEndType(const RELATIONSHIP_TYPE &endMaxType)
{
    _endType = endMaxType;
}

CTable *CRelationship::startTable() const
{
    return _startTable;
}

CTable *CRelationship::endTable() const
{
    return _endTable;
}

bool CRelationship::startMandatory() const
{
    return _startMandatory;
}

void CRelationship::setStartMandatory(bool startMandatory)
{
    _startMandatory = startMandatory;
}

bool CRelationship::endMandatory() const
{
    return _endMandatory;
}

void CRelationship::setEndMandatory(bool endMandatory)
{
    _endMandatory = endMandatory;
}
