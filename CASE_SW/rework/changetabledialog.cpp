#include "CRelationship.h"
#include "CTable.h"
#include "changetabledialog.h"
#include "ui_changetabledialog.h"
#include <QStringListModel>

ChangeTableDialog::ChangeTableDialog(QList<CTable *> tableList, CRelationship *relationship, bool start, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangeTableDialog),
    _model(new QStringListModel(this)),
    _tableList(tableList),
    _relationship(relationship),
    _start(start)
{
    ui->setupUi(this);

    QString selectName = "";
    int selectId, excludeId;
    if(start)
    {
        selectName = _relationship->startTable()->name();
        selectId = _relationship->startTable()->id();
        excludeId = _relationship->endTable()->id();
    }
    else
    {
        selectName = _relationship->endTable()->name();
        selectId = _relationship->endTable()->id();
        excludeId = _relationship->startTable()->id();
    }

    QStringList list;
    foreach (CTable *table, _tableList) {
        if(table->id() != excludeId)
            list << table->name();
    }

    _model->setStringList(list);
    ui->listView->setModel(_model);

    int index = list.indexOf(selectName);
    QModelIndex modelIndex = _model->index(index);
    ui->listView->setCurrentIndex(modelIndex);

    connect(this, SIGNAL(accepted()), this, SLOT(collectData()));
}

ChangeTableDialog::~ChangeTableDialog()
{
    delete ui;
}

void ChangeTableDialog::collectData()
{
    QModelIndex modelIndex = ui->listView->currentIndex();
    QString tableName = _model->data(modelIndex).toString();
    foreach (CTable *table, _tableList) {
        if(table->name() == tableName)
        {
            emit dialogFinished(_relationship->id(), table->id(), _start);
            break;
        }
    }
}
