#include "CDetailedEntityWidget.h"

#include "CViewModel.h"
#include "CEntityView.h"

CDetailedEntityWidget::CDetailedEntityWidget(CViewModel *_parent, CEntityView *_entity, bool is_editable):
    QWidget(NULL), parent(_parent), entity(_entity)
{
    setGeometry(30, 30, 520, 300);

    view = new CDetailedEntityView(this, parent, entity);
    view->setGeometry(5, 5, 510, 200);

    add_button = new QPushButton(this);
    rem_button = new QPushButton(this);
    unique_editor = new QPushButton(this);

    add_button->setGeometry(20, 250, 100, 30);
    rem_button->setGeometry(140, 250, 100, 30);
    unique_editor->setGeometry(20, 215, 100, 30);

    add_button->setText(tr("Add row"));
    rem_button->setText(tr("Remove row"));
    unique_editor->setText(tr("Uniques"));

    connect(add_button, SIGNAL(clicked()), this, SLOT(addRow()));
    connect(rem_button, SIGNAL(clicked()), this, SLOT(remRow()));
    connect(unique_editor, SIGNAL(clicked()), this, SLOT(uEditor()));

    entity_name = new QLabel(tr("name:"), this);
    entity_name->setGeometry(270, 250, 40, 30);

    line_edit = new QLineEdit(this);

    line_edit->setGeometry(320, 250, 160, 30);

    line_edit->setText(entity->getEntity()->getName());

    connect(line_edit, SIGNAL(textChanged(QString)), this, SLOT(entity_name_changed(QString)));

    if (!is_editable)
    {
        //line_edit->setEnabled(false);
        add_button->setEnabled(false);
        rem_button->setEnabled(false);
    }

    view->setItemDelegateForColumn(1, &del);
    view->setItemDelegateForColumn(3, &cbdel);
    view->setItemDelegateForColumn(4, &cbdel);
    view->setItemDelegateForColumn(5, &cbdel);

    view->setColumnWidth(1, 70);
    view->setColumnWidth(2, 70);
    view->setColumnWidth(3, 80);
    view->setColumnWidth(4, 75);
    view->setColumnWidth(5, 75);

    setMaximumSize(520, 300);
    setMinimumSize(520, 300);

    entity_name->setGeometry(270, 250, 50, 30);

    view->setEditTriggers(QAbstractItemView::AllEditTriggers);

    show();
}

CDetailedEntityWidget::~CDetailedEntityWidget()
{
    delete view;
}

CEntityView *CDetailedEntityWidget::getEntity()
{
    return entity;
}

void CDetailedEntityWidget::addRow()
{
    entity->getEntity()->addRow();

    view->setModel(NULL);
    view->setModel(entity->getEntity());

    view->setColumnWidth(1, 70);
    view->setColumnWidth(2, 70);
    view->setColumnWidth(3, 80);
    view->setColumnWidth(4, 75);
    view->setColumnWidth(5, 75);
}

void CDetailedEntityWidget::remRow()
{
    QModelIndexList indexes = view->selectionModel()->selectedIndexes();

    QVector<int> rows_to_remove;

    for (int i = 0; i < indexes.size(); ++i)
    {
        bool new_row = true;
        for (int j = 0; j < rows_to_remove.size(); ++j)
            if (rows_to_remove.at(j) == indexes.at(i).row())
            {
                new_row = false;
                break;
            }

        if (new_row)
        {
            rows_to_remove.append(indexes.at(i).row());
        }
    }

    if (rows_to_remove.size() > 0)
        entity->getEntity()->removeRows(rows_to_remove.first(), rows_to_remove.size());
}

void CDetailedEntityWidget::uEditor()
{
    CUniquesEditorWidget* wid = new CUniquesEditorWidget(entity->getEntity());
    wid->show();
}

void CDetailedEntityWidget::entity_name_changed(QString _name)
{
    if (parent->check_entity_name(_name, entity->getId()))
    {
        entity->getEntity()->setName(_name);
        line_edit->setText(_name);
    }

    entity->update_state();
}

void CDetailedEntityWidget::closeEvent(QCloseEvent *event)
{
    emit closing(this);
}

