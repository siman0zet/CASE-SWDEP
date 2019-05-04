#ifndef CLOADANDSAVEWIDGET_H
#define CLOADANDSAVEWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QFileSystemModel>
#include <QTreeView>
#include <QLineEdit>
#include <QDebug>
#include <QLabel>

class CViewModel;

class CLoadAndSaveWidget : public QWidget
{
    Q_OBJECT
public:
    CLoadAndSaveWidget(CViewModel *_parent, bool isSave);
    ~CLoadAndSaveWidget();

    void saveFSviewConf(QString fsdir);

signals:
    void closing();

protected:
    void closeEvent(QCloseEvent * event);

public slots:

private slots:
    void on_fs_view_clicked(const QModelIndex &index);
    void on_OK_button_clicked();

private:
    CViewModel* parent;
    QLineEdit *new_filename, *new_filedir;
    QPushButton* OK_button;
    QLabel *l_filename, *l_filedir, *l_message;
    QFileSystemModel* fs_model;
    QTreeView* fs_view;
    bool is_save;
};

#endif // CLOADANDSAVEWIDGET_H
