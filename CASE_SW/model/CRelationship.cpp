#include "CRelationship.h"
#include "CTable.h"

CRelationship::CRelationship(CTable *startTable, CTable *endTable) :
    CObject(QString("%1_%2").arg(startTable->name()).arg(endTable->name())),
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

int CRelationship::type() const
{
    return Type;
}

QString CRelationship::getDataAsText() const
{
    QString text;
    /*  relationship startTable_name endTable_name
     *  type type (e.g. one-one one-many many-many aggregate)
     *  m   start_mandatory end_mandatory (e.g. mandatory optional)
     */
    text += QString("\nrelationship %1 %2\n")
            .arg(startTable()->name())
            .arg(endTable()->name());
    QString type;
    if(endType() == AGGREGATE)
        type += "aggregate";
    else
    {
        switch (startType()) {
        case ONE:
            type += "one-";
            break;
        case MANY:
            type += "many-";
            break;
        }
        switch (endType()) {
        case ONE:
            type += "one";
            break;
        case MANY:
            type += "many";
            break;
        }
    }
    text += QString("type %1\n")
            .arg(type);
    text += QString("m %1 %2\n")
            .arg((this->startMandatory()) ? "mandatory" : "optional")
            .arg((this->endMandatory()) ? "mandatory" : "optional");
    return text;
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
void CRelationship::setStartTable(CTable *startTable)
{
    if(_startTable)
    {
        _startTable->removeRelationship(this);
    }
    _startTable = startTable;
    _startTable->addRelationship(this);
}

CTable *CRelationship::endTable() const
{
    return _endTable;
}

void CRelationship::setEndTable(CTable *endTable)
{
    if(_endTable)
    {
        _endTable->removeRelationship(this);
    }
    _endTable = endTable;
    _endTable->addRelationship(this);
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
