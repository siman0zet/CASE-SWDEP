#ifndef CRELATIONSHIP_H
#define CRELATIONSHIP_H

#include "CObject.h"
class CTable;

class CRelationship : public CObject
{
public:
    enum { Type = USER_TYPE + 12 };

    enum RELATIONSHIP_TYPE
    {
        ONE,
        MANY,
        AGGREGATE,
        PHYSICAL
    };

    CRelationship(int id, CTable *startTable, CTable *endTable);
    ~CRelationship();

    int type() override;

    RELATIONSHIP_TYPE startType() const;
    void setStartType(const RELATIONSHIP_TYPE &startType);

    RELATIONSHIP_TYPE endType() const;
    void setEndType(const RELATIONSHIP_TYPE &endType);

    CTable *startTable() const;
    CTable *endTable() const;

    bool startMandatory() const;
    void setStartMandatory(bool startMandatory);

    bool endMandatory() const;
    void setEndMandatory(bool endMandatory);

private:
    CTable *_startTable;
    CTable *_endTable;

    RELATIONSHIP_TYPE _startType;
    RELATIONSHIP_TYPE _endType;

    bool _startMandatory;
    bool _endMandatory;
};

#endif // CRELATIONSHIP_H
