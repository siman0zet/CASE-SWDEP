#include "CRelationship.h"

CRelationship::CRelationship(int id, CTable *startTable, CTable *endTable) :
    CObject(id),
    _startTable(startTable),
    _endTable(endTable),
    _startMaxType(ONE),
    _startMinType(MANDATORY),
    _endMaxType(ONE),
    _endMinType(MANDATORY)
{
}

CRelationship::RELATIONSHIP_MAX_TYPE CRelationship::startMaxType() const
{
    return _startMaxType;
}

void CRelationship::setStartMaxType(const RELATIONSHIP_MAX_TYPE &startMaxType)
{
    _startMaxType = startMaxType;
}

CRelationship::RELATIONSHIP_MIN_TYPE CRelationship::startMinType() const
{
    return _startMinType;
}

void CRelationship::setStartMinType(const RELATIONSHIP_MIN_TYPE &startMinType)
{
    _startMinType = startMinType;
}

CRelationship::RELATIONSHIP_MAX_TYPE CRelationship::endMaxType() const
{
    return _endMaxType;
}

void CRelationship::setEndMaxType(const RELATIONSHIP_MAX_TYPE &endMaxType)
{
    _endMaxType = endMaxType;
}

CRelationship::RELATIONSHIP_MIN_TYPE CRelationship::endMinType() const
{
    return _endMinType;
}

void CRelationship::setEndMinType(const RELATIONSHIP_MIN_TYPE &endMinType)
{
    _endMinType = endMinType;
}
