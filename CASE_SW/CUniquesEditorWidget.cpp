#include "CUniquesEditorWidget.h"

CUniquesEditorWidget::CUniquesEditorWidget(CEntity *en): QWidget(), entity(en)
{
    this->setGeometry(20, 30, 400, 300);
        this->setMaximumSize(400, 300);
        this->setMinimumSize(400, 300);

        lb1 = new QLabel(this);
        lb1->setText(tr("Unique group:"));
        lb1->setGeometry(15, 10, 100, 30);

        lb2 = new QLabel(this);
        lb2->setText(tr("Elements in group:"));
        lb2->setGeometry(15, 50, 160, 30);

        lb3 = new QLabel(this);
        lb3->setText(tr("Other elements:"));
        lb3->setGeometry(230, 50, 160, 30);

        cb = new QComboBox(this);
        cb->setGeometry(130, 10, 120, 30);

        remGr = new QPushButton(this);
        remGr->setText(tr("-"));
        remGr->setGeometry(285, 10, 30, 30);

        addGr = new QPushButton(this);
        addGr->setText(tr("+"));
        addGr->setGeometry(335, 10, 30, 30);

        l1 = new QListView(this);
        l1->setGeometry(10, 80, 165, 210);

        l2 = new QListView(this);
        l2->setGeometry(225, 80, 165, 210);

        remRow = new QPushButton(this);
        remRow->setText(tr("->"));
        remRow->setGeometry(185, 135, 30, 30);

        addRow = new QPushButton(this);
        addRow->setText(tr("<-"));
        addRow->setGeometry(185, 175, 30, 30);

        l1->setModel(&m1);
        l2->setModel(&m2);

        updateUniqueGroups();

        if (uns.size() > 0)
        {
            cb->setCurrentIndex(0);
            setUniqueGroup(0);
        }

        connect(addGr, SIGNAL(clicked()), this, SLOT(addGrAction()));
        connect(remGr, SIGNAL(clicked()), this, SLOT(remGrAction()));

        connect(addRow, SIGNAL(clicked()), this, SLOT(addRowAction()));
        connect(remRow, SIGNAL(clicked()), this, SLOT(remRowAction()));

        connect(cb, SIGNAL(currentIndexChanged(int)), this, SLOT(setUniqueGroup(int)));
}

CUniquesEditorWidget::~CUniquesEditorWidget()
{

}

void CUniquesEditorWidget::updateUniqueGroups()
{
    uns = entity->getComplexUniques();

    QStringList sl;

    for (int i = 0; i < uns.size(); ++i)
    {
        QString num, name;
        name = tr("Group ");

        num.setNum(i + 1);

        sl.append(name + num);
    }

    cb->clear();
    cb->addItems(sl);

    free_rows.clear();

    for (int i = 0; i < entity->getRowCount(); ++i)
    {
        bool free = true;

        for (int j = 0; j < uns.size(); ++j)
        {
            for (int k = 0; k < uns[j].size(); ++k)
            {
                if (uns[j][k] == i)
                {
                    free = false;
                    break;
                }
            }

            if (!free)
                break;
        }

        if (free)
        {
            free_rows.append(entity->getRow(i)->getName());
        }
    }
}

void CUniquesEditorWidget::setUniqueGroup(int i)
{
    if (i >= uns.size() || i < 0)
    {
        QStringList l;
        m1.setStringList(l);
        m2.setStringList(l);
        return;
    }

    QStringList l;

    for (int j = 0; j < uns[i].size(); ++j)
    {
        if (uns[i][j] < entity->getRowCount())
            l.append(entity->getRow(uns[i][j])->getName());
    }

    m1.setStringList(l);
    m2.setStringList(free_rows);
}

void CUniquesEditorWidget::addGrAction()
{
    entity->addCmplxUniquesGroup();

    updateUniqueGroups();

    if (uns.size() == 1)
    {
        cb->setCurrentIndex(0);
        setUniqueGroup(0);
    }
}

void CUniquesEditorWidget::remGrAction()
{
    if (uns.size() <= 0)
        return;

    entity->remCmplxUniquesGroup(cb->currentIndex());

    int ind = cb->currentIndex();

    updateUniqueGroups();

    setUniqueGroup(ind);
    cb->setCurrentIndex(ind);
}

void CUniquesEditorWidget::addRowAction()
{
    if (uns.size() <= 0)
        return;

    QModelIndexList indexes = l2->selectionModel()->selectedIndexes();

    QVector<int> rows_to_remove;
    QVector<QString> rtr;

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
            rtr.append(indexes.at(i).data().toString());
        }
    }

    for (int i = 0; i < rows_to_remove.size(); ++i)
    {
        entity->addToCmplxUniqueGroup(cb->currentIndex(), findRow(rtr.at(i)));
    }

    int ind = cb->currentIndex();

    updateUniqueGroups();
    setUniqueGroup(ind);
    cb->setCurrentIndex(ind);
}

void CUniquesEditorWidget::remRowAction()
{
    if (uns.size() <= 0)
        return;

    QModelIndexList indexes = l1->selectionModel()->selectedIndexes();

    QVector<int> rows_to_remove;
    QVector<QString> rtr;

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
            rtr.append(indexes.at(i).data().toString());
        }
    }

    for (int i = 0; i < rows_to_remove.size(); ++i)
    {
        entity->remFromCmplxUniquesGroup(cb->currentIndex(), findRow(rtr.at(i)));
    }

    int ind = cb->currentIndex();

    updateUniqueGroups();
    setUniqueGroup(ind);
    cb->setCurrentIndex(ind);
}

int CUniquesEditorWidget::findRow(QString name)
{
    for (int i = 0; i < entity->getRowCount(); ++i)
    {
        if (name == entity->getRow(i)->getName())
            return i;
    }

    return -1;
}

