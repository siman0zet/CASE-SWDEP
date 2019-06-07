#include "CRelationship.h"
#include "CRelationshipEditor.h"
#include "CTable.h"
#include "CModelView.h"
#include "ui_crelationshipeditor.h"

#include <QDebug>

CRelationshipEditor::CRelationshipEditor(CRelationship *relationship, CModelView *modelView, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CRelationshipEditor),
    _relationship(relationship),
    _modelView(modelView)
{
    ui->setupUi(this);
    synchronizeData();
}

CRelationshipEditor::~CRelationshipEditor()
{
    delete ui;
    _relationship = NULL;
}

CRelationship *CRelationshipEditor::relationship() const
{
    return _relationship;
}

void CRelationshipEditor::synchronizeData()
{
    if(_relationship->startType() == CRelationship::ONE &&
       _relationship->endType() == CRelationship::ONE)
        ui->radioOneOne->setChecked(true);

    if(_relationship->startType() == CRelationship::ONE &&
       _relationship->endType() == CRelationship::MANY)
        ui->radioOneMany->setChecked(true);

    if(_relationship->startType() == CRelationship::MANY &&
       _relationship->endType() == CRelationship::MANY)
        ui->radioManyMany->setChecked(true);

    if(_relationship->startType() == CRelationship::ONE &&
       _relationship->endType() == CRelationship::AGGREGATE)
        ui->radioAggregate->setChecked(true);

    ui->startTableName->setText(_relationship->startTable()->name());
    ui->endTableName->setText(_relationship->endTable()->name());

    ui->starTableMandatory->setChecked(_relationship->startMandatory());
    ui->endTableMandatory->setChecked(_relationship->endMandatory());
}

void CRelationshipEditor::on_radioOneOne_toggled(bool checked)
{
    if(checked)
    {
        _relationship->setStartType(CRelationship::ONE);
        _relationship->setEndType(CRelationship::ONE);
    }
    emit dataChanged();
}

void CRelationshipEditor::on_radioOneMany_toggled(bool checked)
{
    if(checked)
    {
        _relationship->setStartType(CRelationship::ONE);
        _relationship->setEndType(CRelationship::MANY);
    }
    emit dataChanged();
}

void CRelationshipEditor::on_radioManyMany_toggled(bool checked)
{
    if(checked)
    {
        _relationship->setStartType(CRelationship::MANY);
        _relationship->setEndType(CRelationship::MANY);
    }
    emit dataChanged();
}

void CRelationshipEditor::on_radioAggregate_toggled(bool checked)
{
    if(checked)
    {
        _relationship->setStartType(CRelationship::ONE);
        _relationship->setEndType(CRelationship::AGGREGATE);
    }
    emit dataChanged();
}

void CRelationshipEditor::on_starTableMandatory_toggled(bool checked)
{
    _relationship->setStartMandatory(checked);
    emit dataChanged();
}

void CRelationshipEditor::on_endTableMandatory_toggled(bool checked)
{
    _relationship->setEndMandatory(checked);
    emit dataChanged();
}

void CRelationshipEditor::on_flipTables_clicked()
{
    _modelView->flipTables(_relationship->id());
    synchronizeData();
}

void CRelationshipEditor::on_startTableChange_clicked()
{
    _modelView->showChangeTableDialog(_relationship->id(), true);
    synchronizeData();
}

void CRelationshipEditor::on_endTableChange_clicked()
{
    _modelView->showChangeTableDialog(_relationship->id(), false);
    synchronizeData();
}

void CRelationshipEditor::on_startTableEdit_clicked()
{
    _modelView->showObjectEditor((CObjectItem *)_modelView->tableItem(_relationship->startTable()->id()));
}

void CRelationshipEditor::on_endTableEdit_clicked()
{
    _modelView->showObjectEditor((CObjectItem *)_modelView->tableItem(_relationship->endTable()->id()));
}
