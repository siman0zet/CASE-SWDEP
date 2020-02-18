#include "../model/crelationship.h"
#include "../model/ctable.h"
#include "changetabledialog.h"
#include "ui_changetabledialog.h"
#include <QStringListModel>

ChangeTableDialog::ChangeTableDialog(const QList<CTable *> &tableList, CRelationship *relationship, bool start, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangeTableDialog),
    _model(new QStringListModel(this)),
    _tableList(tableList),
    _relationship(relationship),
    _start(start)
{
    ui->setupUi(this);

    QString selectName;
    QString excludeName;
    if(start)
    {
        selectName = _relationship->startTable()->name();
        excludeName = _relationship->endTable()->name();
    }
    else
    {
        selectName = _relationship->endTable()->name();
        excludeName = _relationship->startTable()->name();
    }

    QStringList list;
    foreach (CTable *table, _tableList) {
        if(table->name() != excludeName)
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
            emit dialogFinished(_relationship->name(), table->name(), _start);
            break;
        }
    }
}
