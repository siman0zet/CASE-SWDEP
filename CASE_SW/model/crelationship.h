#ifndef CRELATIONSHIP_H
#define CRELATIONSHIP_H

#include "cobject.h"
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
        ARROW
    };

    CRelationship(CTable *startTable, CTable *endTable);
    ~CRelationship();

    int type() const override;
    QString exportDataToText() const override;
    void importFromTextStream(QTextStream &input) override;

    RELATIONSHIP_TYPE startType() const;
    void setStartType(const RELATIONSHIP_TYPE &startType);

    RELATIONSHIP_TYPE endType() const;
    void setEndType(const RELATIONSHIP_TYPE &endType);

    CTable *startTable() const;
    void setStartTable(CTable *startTable);
    CTable *endTable() const;
    void setEndTable(CTable *endTable);

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
