#ifndef CHANGETABLEDIALOG_H
#define CHANGETABLEDIALOG_H

#include <QDialog>

class CTable;
class QStringListModel;
class CRelationship;

namespace Ui {
class ChangeTableDialog;
}

class ChangeTableDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChangeTableDialog(const QList<CTable *> &tableList, CRelationship *relationship, bool start, QWidget *parent = 0);
    ~ChangeTableDialog();

private:
    Ui::ChangeTableDialog *ui;

    QStringListModel *_model;

    QList<CTable *> _tableList;
    CRelationship *_relationship;
    bool _start;

private slots:
    void collectData();

signals:
    void dialogFinished(const QString &relationshipName, const QString &tableName, bool start);
};

#endif // CHANGETABLEDIALOG_H
