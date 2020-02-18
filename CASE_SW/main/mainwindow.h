#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QSharedPointer>

#include "../view/cmodelview.h"

namespace Ui {
class MainWindow;
}

class CViewModel;
class QScrollArea;
class CModelView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void activateEditAction(const CModelView::TOOL_TYPE &type);

protected:
    void closeEvent(QCloseEvent *) override;

private slots:
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionQuit_triggered();
    void on_actionSave_as_triggered();
    void on_actionClose_triggered();
    void on_tabWidget_tabCloseRequested(int index);
    void on_actionPointer_triggered();
    void on_actionCreate_Table_triggered();
    void on_actionDelete_triggered();
    void on_actionOne_One_triggered();
    void on_actionOne_Many_triggered();
    void on_actionMany_Many_triggered();
    void on_actionAggregate_triggered();
    void on_tabWidget_currentChanged(int index);
    void on_actionTo_PDM_triggered();
    void on_actionScript_triggered();
    void on_actionChange_Size_triggered();

private:
    Ui::MainWindow *ui;

    //! \brief Track down models/files currently open in application workspace.
    QMap<QString, CModelView *> _workspaceModels;

    QMap<CModelView::TOOL_TYPE, QSharedPointer<QAction>> _editActions;

    //! \brief Simple counter for newly created models.
    //! Value increases only when new model is created.
    uint _countCreatedModels;

    bool addModelTab(const QString &modelName, const QString &modelPath);
    void closeTab(int index);
    void deactivateEditActions();
};

#endif // MAINWINDOW_H
