#include "mainwindow.h"
#include "pmodelwindow.h"
#include "cpluginwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowState(Qt::WindowMaximized);
    _countCreatedModels = 0;

    _editActions.insert(CModelView::POINTER, QSharedPointer<QAction>(ui->actionPointer));
    _editActions.insert(CModelView::CREATE, QSharedPointer<QAction>(ui->actionCreate_Table));
    _editActions.insert(CModelView::DELETE, QSharedPointer<QAction>(ui->actionDelete));
    _editActions.insert(CModelView::ONE_ONE, QSharedPointer<QAction>(ui->actionOne_One));
    _editActions.insert(CModelView::ONE_MANY, QSharedPointer<QAction>(ui->actionOne_Many));
    _editActions.insert(CModelView::MANY_MANY, QSharedPointer<QAction>(ui->actionMany_Many));
    _editActions.insert(CModelView::AGGREGATE, QSharedPointer<QAction>(ui->actionAggregate));

    connect(ui->actionPointer, &QAction::triggered,
            this, [this]{activateEditAction(CModelView::POINTER);});
    connect(ui->actionCreate_Table, &QAction::triggered,
            this, [this]{activateEditAction(CModelView::CREATE);});
    connect(ui->actionDelete, &QAction::triggered,
            this, [this]{activateEditAction(CModelView::DELETE);});
    connect(ui->actionOne_One, &QAction::triggered,
            this, [this]{activateEditAction(CModelView::ONE_ONE);});
    connect(ui->actionOne_Many, &QAction::triggered,
            this, [this]{activateEditAction(CModelView::ONE_MANY);});
    connect(ui->actionMany_Many, &QAction::triggered,
            this, [this]{activateEditAction(CModelView::MANY_MANY);});
    connect(ui->actionAggregate, &QAction::triggered,
            this, [this]{activateEditAction(CModelView::AGGREGATE);});

    activateEditAction(CModelView::POINTER);
    this->setWindowTitle("CASE-SWDEP");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_triggered()
{
    QString modelName = QString("New_%1").arg(QString::number(++_countCreatedModels));

    if(!addModelTab(modelName, ""))
        qDebug() << "ERROR: MainWIndow::addModelTab";
}

void MainWindow::on_actionOpen_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(
                this,
                QObject::tr("Open Model"),
                QString(),
                QObject::tr("Model Files (*.cdmod);;All Files (*.*)"),
                nullptr,
                QFileDialog::DontUseNativeDialog
    );
    if(filePath.isEmpty())
        return;

    QFileInfo fileInfo(filePath);
    QString modelName = fileInfo.fileName().split(".").at(0);
    if(_workspaceModels.contains(modelName))
    {
        for(int i = 0; i < ui->tabWidget->count(); i++)
        {
            QString tabName = ui->tabWidget->tabText(i);
            if (modelName == tabName)
            {
                ui->tabWidget->setCurrentIndex(i);
                break;
            }
        }
    }
    else
        addModelTab(modelName, filePath);
}

void MainWindow::on_actionSave_triggered()
{
    QString modelName = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    QString modelPath = _workspaceModels.value(modelName)->path();

    // can't save model if no path specified
    if(modelPath.isEmpty())
    {
        on_actionSave_as_triggered();
    }
    else
    {
        if(!_workspaceModels.value(modelName)->saveToFile(modelPath))
            return;
    }
}

void MainWindow::on_actionSave_as_triggered()
{
    QString newModelPath = QFileDialog::getSaveFileName(
        this,
        QObject::tr("Save as CDMOD"),
        QString(),
        QObject::tr("Model Files (*.cdmod)"),
        nullptr,
        QFileDialog::DontUseNativeDialog
    );
    if (newModelPath.isEmpty()) {
        return;
    }

    QFileInfo newFileInfo(newModelPath);
    QString newModelName = newFileInfo.fileName().split(".")[0];

    QString oldModelName = ui->tabWidget->tabText(ui->tabWidget->currentIndex());

    _workspaceModels.value(oldModelName)->setName(newModelName);
    _workspaceModels.insert(newModelName, _workspaceModels.value(oldModelName));
    _workspaceModels.remove(oldModelName);

    if(!_workspaceModels.value(newModelName)->saveToFile(newModelPath))
        return;

    ui->tabWidget->removeTab(ui->tabWidget->currentIndex());
    addModelTab(newModelName, newModelPath);
}

void MainWindow::on_actionClose_triggered()
{
    closeTab(ui->tabWidget->currentIndex());
}

void MainWindow::on_actionQuit_triggered()
{
    for(int i = 0; i < ui->tabWidget->count(); i++)
        closeTab(i);

    QApplication::closeAllWindows();
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    closeTab(index);
}

void MainWindow::activateEditAction(const CModelView::TOOL_TYPE &type)
{
    deactivateEditActions();
    _editActions.value(type)->setChecked(true);
}

void MainWindow::closeEvent(QCloseEvent *)
{
    on_actionQuit_triggered();
}

bool MainWindow::addModelTab(const QString &modelName, const QString &modelPath)
{
    CModelView *modelView = new CModelView(modelName, modelPath, this);
    if(!modelPath.isEmpty())
        if(!modelView->loadFromFile(modelPath))
            return false;

    QScrollArea* scroll = new QScrollArea();
    scroll->setWidgetResizable(true);
    scroll->setWidget(modelView);
    modelView->changeSize(1280, 720);

    _workspaceModels.insert(modelName, modelView);

    ui->tabWidget->addTab(scroll, modelName);
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);

    // trigger currently selected action
    /* For user it seems that this action was allegedly triggered before any tab was open.
     * Otherwice, user will have to trigger this action again.
     */
    for(int i = 0; i < _editActions.size(); i++)
    {
        if(_editActions.value((CModelView::TOOL_TYPE)i)->isChecked())
        {
            _editActions.value((CModelView::TOOL_TYPE)i)->trigger();
            break;
        }
    }

    return true;
}

void MainWindow::closeTab(int index)
{
    QString modelName = ui->tabWidget->tabText(index);

    ui->tabWidget->removeTab(index);

    _workspaceModels.remove(modelName);
}

void MainWindow::deactivateEditActions()
{
    for(int i = 0; i < _editActions.size(); i++)
        _editActions.value((CModelView::TOOL_TYPE)i)->setChecked(false);
}

void MainWindow::on_actionPointer_triggered()
{
    QString modelName = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    if(_workspaceModels.find(modelName) == _workspaceModels.end())
        return;
    _workspaceModels.value(modelName)->activateTool(CModelView::POINTER);
}

void MainWindow::on_actionCreate_Table_triggered()
{
    QString modelName = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    if(_workspaceModels.find(modelName) == _workspaceModels.end())
        return;
    _workspaceModels.value(modelName)->activateTool(CModelView::CREATE);
}

void MainWindow::on_actionDelete_triggered()
{
    QString modelName = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    if(_workspaceModels.find(modelName) == _workspaceModels.end())
        return;
    _workspaceModels.value(modelName)->activateTool(CModelView::DELETE);
}

void MainWindow::on_actionOne_One_triggered()
{
    QString modelName = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    if(_workspaceModels.find(modelName) == _workspaceModels.end())
        return;
    _workspaceModels.value(modelName)->activateTool(CModelView::ONE_ONE);
}

void MainWindow::on_actionOne_Many_triggered()
{
    QString modelName = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    if(_workspaceModels.find(modelName) == _workspaceModels.end())
        return;
    _workspaceModels.value(modelName)->activateTool(CModelView::ONE_MANY);
}

void MainWindow::on_actionMany_Many_triggered()
{
    QString modelName = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    if(_workspaceModels.find(modelName) == _workspaceModels.end())
        return;
    _workspaceModels.value(modelName)->activateTool(CModelView::MANY_MANY);
}

void MainWindow::on_actionAggregate_triggered()
{
    QString modelName = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    if(_workspaceModels.find(modelName) == _workspaceModels.end())
        return;
    _workspaceModels.value(modelName)->activateTool(CModelView::AGGREGATE);
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    QString modelName = ui->tabWidget->tabText(index);
    this->setWindowTitle((modelName.isNull()) ? "CASE-SWDEP" :  modelName + " - CASE-SWDEP");
}

void MainWindow::on_actionTo_PDM_triggered()
{
    QString modelName = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    if(_workspaceModels.find(modelName) == _workspaceModels.end())
        return;

    PModelWindow *pWindow = new PModelWindow(_workspaceModels.value(modelName), this);
    pWindow->show();
}

void MainWindow::on_actionScript_triggered()
{
    QString modelName = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    if(_workspaceModels.find(modelName) == _workspaceModels.end())
        return;

    CPluginWindow *sWindow = new CPluginWindow(_workspaceModels.value(modelName));
    sWindow->show();
}

void MainWindow::on_actionChange_Size_triggered()
{
    QString modelName = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    if(_workspaceModels.find(modelName) == _workspaceModels.end())
        return;
    _workspaceModels.value(modelName)->showResizeDialog();
}
