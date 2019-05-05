#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QSharedPointer>

namespace Ui {
class MainWindow;
}

class CViewModel;
class QScrollArea;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    enum groupActionType {
        POINTER = 0,
        CREATE,
        DELETE,
        ONE_TO_ONE,
        ONE_TO_MANY,
        MANY_TO_MANY,
        AGGREGATE
    };

private slots:
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionQuit_triggered();
    void on_actionSave_as_triggered();
    void on_actionClose_triggered();
    void on_tabWidget_tabCloseRequested(int index);
    void triggerGroupAction(groupActionType type);
    void on_actionPointer_triggered();
    void on_actionCreate_Table_triggered();
    void on_actionDelete_triggered();
    void on_actionOne_To_One_triggered();
    void on_actionOne_To_Many_triggered();
    void on_actionMany_To_Many_triggered();
    void on_actionAggregate_triggered();
    void on_tabWidget_currentChanged(int index);

    void on_actionTo_PDM_triggered();

    void on_actionScript_triggered();

private:
    Ui::MainWindow *ui;

    QApplication* app;

    class ModelInfo
    {
    private:
        QString name;
        QString path;
        QWidget* widget;

    public:
        ModelInfo(QString name = "", QString path = "", QWidget* widget = NULL);
        ~ModelInfo();
        void        setName(QString name);
        void        setPath(QString path);
        void        setWidget(QWidget* widget);
        QString     getName();
        QString     getPath();
        QWidget*    getWidget();
    };

    //! \brief Track down models/files currently open in application workspace.
    QMap<QString, QSharedPointer<ModelInfo>> m_workspaceModels;

    QMap<groupActionType, QSharedPointer<QAction>> m_groupAction;

    //! \brief Simple counter for newly created models.
    //! Value increases only when new model is created.
    uint m_countCreatedModels;

    bool addModelTab(QString modelName, QString modelPath);
    void closeTab(int index);
    void turnOffGroupActions();
};

#endif // MAINWINDOW_H
