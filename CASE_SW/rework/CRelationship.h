#ifndef CRELATIONSHIP_H
#define CRELATIONSHIP_H

#include "CObject.h"
class CTable;

class CRelationship : public CObject
{
public:
    enum RELATIONSHIP_MAX_TYPE
    {
        ONE,
        MANY,
        AGGREGATE,
        PHYSICAL
    };

    enum RELATIONSHIP_MIN_TYPE
    {
        OPTIONAL,
        MANDATORY
    };

    CRelationship(int id, CTable *startTable, CTable *endTable);

    RELATIONSHIP_MAX_TYPE startMaxType() const;
    void setStartMaxType(const RELATIONSHIP_MAX_TYPE &startMaxType);

    RELATIONSHIP_MIN_TYPE startMinType() const;
    void setStartMinType(const RELATIONSHIP_MIN_TYPE &startMinType);

    RELATIONSHIP_MAX_TYPE endMaxType() const;
    void setEndMaxType(const RELATIONSHIP_MAX_TYPE &endMaxType);

    RELATIONSHIP_MIN_TYPE endMinType() const;
    void setEndMinType(const RELATIONSHIP_MIN_TYPE &endMinType);

private:
    CTable *_startTable;
    CTable *_endTable;

    RELATIONSHIP_MAX_TYPE _startMaxType;
    RELATIONSHIP_MIN_TYPE _startMinType;
    RELATIONSHIP_MAX_TYPE _endMaxType;
    RELATIONSHIP_MIN_TYPE _endMinType;
};

#endif // CRELATIONSHIP_H
