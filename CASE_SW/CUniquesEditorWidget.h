#ifndef CUNIQUESEDITORWIDGET_H
#define CUNIQUESEDITORWIDGET_H

#include <QWidget>

#include <QLabel>
#include <QListView>
#include <QComboBox>
#include <QPushButton>
#include <QStringListModel>
#include <QVector>

#include "CEntity.h"

class CUniquesEditorWidget : public QWidget
{
    Q_OBJECT
public:
    CUniquesEditorWidget(CEntity* en);
    ~CUniquesEditorWidget();

    void updateUniqueGroups();

public slots:

    void setUniqueGroup(int i);

    void addGrAction();
    void remGrAction();

    void addRowAction();
    void remRowAction();

protected:



    QLabel *lb2, *lb3;
    int findRow(QString name);

    QComboBox *cb;
    QPushButton *addGr, *remGr, *addRow, *remRow;

    QListView *l1, *l2;

    CEntity *entity;

    QLabel *lb1;

    QStringListModel m1, m2;

    QVector<QVector<int> > uns;

    QStringList free_rows;
};

#endif // CUNIQUESEDITORWIDGET_H
