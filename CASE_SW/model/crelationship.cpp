#include "crelationship.h"
#include "ctable.h"

#include <QTextStream>

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

QString CRelationship::exportDataToText() const
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
        default:
            break;
        }
        switch (endType()) {
        case ONE:
            type += "one";
            break;
        case MANY:
            type += "many";
            break;
        default:
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

void CRelationship::importFromTextStream(QTextStream &input)
{
    QStringList strList = input.readLine().split(" ");
    /*  type type (e.g. one-one one-many many-many aggregate)
     *  m   start_mandatory end_mandatory (e.g. mandatory optional)
     */
    if(strList.at(0) == "type")
    {
        if(strList.size() == 2)
        {
            QString t = strList.at(1);
            QStringList list = t.split("-");
            if(list.at(0) == "one")
                this->setStartType(ONE);
            if(list.at(0) == "many")
                this->setStartType(MANY);
            if(list.at(0) == "aggregate")
            {
                this->setStartType(ONE);
                this->setEndType(AGGREGATE);
            }
            if(list.size() == 2)
            {
                if(list.at(1) == "one")
                    this->setEndType(ONE);
                if(list.at(1) == "many")
                    this->setEndType(MANY);
            }
        }
    }
    strList = input.readLine().split(" ");
    if(strList.at(0) == "m")
    {
        if(strList.size() == 3)
        {
            if(strList.at(1) == "mandatory")
                this->setStartMandatory(true);
            else
                this->setStartMandatory(false);
            if(strList.at(2) == "mandatory")
                this->setEndMandatory(true);
            else
                this->setEndMandatory(false);
        }
    }
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
