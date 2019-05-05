#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "CViewModel.h"
#include "CScrollArea.h"

#include <QDebug>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_countCreatedModels = 0;

    m_groupAction.insert(POINTER, QSharedPointer<QAction>(ui->actionPointer));
    m_groupAction.insert(CREATE, QSharedPointer<QAction>(ui->actionCreate_Table));
    m_groupAction.insert(DELETE, QSharedPointer<QAction>(ui->actionDelete));
    m_groupAction.insert(ONE_TO_ONE, QSharedPointer<QAction>(ui->actionOne_To_One));
    m_groupAction.insert(ONE_TO_MANY, QSharedPointer<QAction>(ui->actionOne_To_Many));
    m_groupAction.insert(MANY_TO_MANY, QSharedPointer<QAction>(ui->actionMany_To_Many));
    m_groupAction.insert(AGGREGATE, QSharedPointer<QAction>(ui->actionAggregate));

    connect(ui->actionPointer, &QAction::triggered,
            this, [this]{triggerGroupAction(POINTER);});
    connect(ui->actionCreate_Table, &QAction::triggered,
            this, [this]{triggerGroupAction(CREATE);});
    connect(ui->actionDelete, &QAction::triggered,
            this, [this]{triggerGroupAction(DELETE);});
    connect(ui->actionOne_To_One, &QAction::triggered,
            this, [this]{triggerGroupAction(ONE_TO_ONE);});
    connect(ui->actionOne_To_Many, &QAction::triggered,
            this, [this]{triggerGroupAction(ONE_TO_MANY);});
    connect(ui->actionMany_To_Many, &QAction::triggered,
            this, [this]{triggerGroupAction(MANY_TO_MANY);});
    connect(ui->actionAggregate, &QAction::triggered,
            this, [this]{triggerGroupAction(AGGREGATE);});

    triggerGroupAction(POINTER);
    this->setWindowTitle("CASE-SWDEP");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_triggered()
{
    QString modelName = "New " + QString::number(++m_countCreatedModels);

    if(!addModelTab(modelName, ""))
        qDebug() << "ERROR: MainWIndow::addModelTab";
}

void MainWindow::on_actionOpen_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(
                this,
                QObject::tr("Open Model"),
                QString(),
                QObject::tr("Model Files (*.cdmod);;All Files (*.*)")
    );
    if(filePath.isEmpty())
        return;

    // no need for file extension in model name
    // because only one extention is possible
    QFileInfo fileInfo(filePath);
    addModelTab(fileInfo.fileName().split(".")[0], filePath);
}

void MainWindow::on_actionSave_triggered()
{
    QString modelName = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    QString modelPath = m_workspaceModels.value(modelName)->getPath();

    // can't save model if no path specified
    if(modelPath.isEmpty())
    {
        on_actionSave_as_triggered();
    }
    else
    {
        CViewModel* modelWidget = (CViewModel*)m_workspaceModels.value(modelName)->getWidget();
        modelWidget->saveInFile(modelPath);
    }
}

void MainWindow::on_actionSave_as_triggered()
{
    QString newModelPath = QFileDialog::getSaveFileName(
        this,
        QObject::tr("Save as CDMOD"),
        QString(),
        QObject::tr("Model Files (*.cdmod)")
    );
    if (newModelPath.isEmpty()) {
        return;
    }

    QFileInfo newFileInfo(newModelPath);
    QString newModelName = newFileInfo.fileName().split(".")[0];

    QString oldModelName = ui->tabWidget->tabText(ui->tabWidget->currentIndex());

    CViewModel* modelWidget = (CViewModel*)m_workspaceModels.value(oldModelName)->getWidget();
    modelWidget->saveInFile(newModelPath);

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

    close();
}

void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    closeTab(index);
}

void MainWindow::triggerGroupAction(groupActionType type)
{
    turnOffGroupActions();
    m_groupAction.value(type)->setChecked(true);
}

bool MainWindow::addModelTab(QString modelName, QString modelPath)
{
    CViewModel *modelWidget = new CViewModel((QWidget *)ui->tabWidget, app);
    if(!modelPath.isEmpty())
        modelWidget->loadFromFile(modelPath); // add return statement to load

    modelWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    modelWidget->setMinimumSize(300, 400);

    QScrollArea* scroll = new QScrollArea();
    scroll->setWidgetResizable(true);
    scroll->setWidget(modelWidget);

    QSharedPointer<ModelInfo> model =
            QSharedPointer<ModelInfo>(new ModelInfo(modelName, modelPath, modelWidget));

    m_workspaceModels.insert(modelName, model);

    ui->tabWidget->addTab(scroll, modelName);
    ui->tabWidget->setCurrentIndex(ui->tabWidget->count() - 1);

    // trigger currently selected action
    /* For user it seems that this action was allegedly triggered before any tab was open.
     * Otherwice, user will have to trigger this action again.
     */
    for(int i = 0; i < m_groupAction.size(); i++)
    {
        if(m_groupAction.value((groupActionType)i)->isChecked())
        {
            m_groupAction.value((groupActionType)i)->trigger();
            break;
        }
    }

    return true;
}

void MainWindow::closeTab(int index)
{
    QString modelName = ui->tabWidget->tabText(index);

    ui->tabWidget->removeTab(index);

    m_workspaceModels.remove(modelName);
}

void MainWindow::turnOffGroupActions()
{
    for(int i = 0; i < m_groupAction.size(); i++)
        m_groupAction.value((groupActionType)i)->setChecked(false);
}

MainWindow::ModelInfo::ModelInfo(QString name, QString path, QWidget* widget)
{
    setName(name);
    setPath(path);
    setWidget(widget);
}

MainWindow::ModelInfo::~ModelInfo()
{
    if(widget != NULL)
        delete widget;
}

void MainWindow::ModelInfo::setName(QString name)
{
    this->name = name;
}

void MainWindow::ModelInfo::setPath(QString path)
{
    this->path = path;
}

void MainWindow::ModelInfo::setWidget(QWidget *widget)
{
    this->widget = widget;
}

QString MainWindow::ModelInfo::getName()
{
    return name;
}

QString MainWindow::ModelInfo::getPath()
{
    return path;
}

QWidget *MainWindow::ModelInfo::getWidget()
{
    return widget;
}

void MainWindow::on_actionPointer_triggered()
{
    QString modelName = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    if(m_workspaceModels.find(modelName) == m_workspaceModels.end())
        return;
    CViewModel* widget = (CViewModel*)m_workspaceModels.value(modelName)->getWidget();
    widget->cursor_tool_activate();
}

void MainWindow::on_actionCreate_Table_triggered()
{
    QString modelName = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    if(m_workspaceModels.find(modelName) == m_workspaceModels.end())
        return;
    CViewModel* widget = (CViewModel*)m_workspaceModels.value(modelName)->getWidget();
    widget->create_entity_tool_activate();
}

void MainWindow::on_actionDelete_triggered()
{
    QString modelName = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    if(m_workspaceModels.find(modelName) == m_workspaceModels.end())
        return;
    CViewModel* widget = (CViewModel*)m_workspaceModels.value(modelName)->getWidget();
    widget->remove_entity_tool_activate();
}

void MainWindow::on_actionOne_To_One_triggered()
{
    QString modelName = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    if(m_workspaceModels.find(modelName) == m_workspaceModels.end())
        return;
    CViewModel* widget = (CViewModel*)m_workspaceModels.value(modelName)->getWidget();
    widget->one_one_tool_activate();
}

void MainWindow::on_actionOne_To_Many_triggered()
{
    QString modelName = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    if(m_workspaceModels.find(modelName) == m_workspaceModels.end())
        return;
    CViewModel* widget = (CViewModel*)m_workspaceModels.value(modelName)->getWidget();
    widget->one_many_tool_activate();
}

void MainWindow::on_actionMany_To_Many_triggered()
{
    QString modelName = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    if(m_workspaceModels.find(modelName) == m_workspaceModels.end())
        return;
    CViewModel* widget = (CViewModel*)m_workspaceModels.value(modelName)->getWidget();
    widget->many_many_tool_activate();
}

void MainWindow::on_actionAggregate_triggered()
{
    QString modelName = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    if(m_workspaceModels.find(modelName) == m_workspaceModels.end())
        return;
    CViewModel* widget = (CViewModel*)m_workspaceModels.value(modelName)->getWidget();
    widget->aggregate_tool_activate();
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    QString modelName = ui->tabWidget->tabText(index);
    this->setWindowTitle((modelName.isNull()) ? "CASE-SWDEP" :  modelName + " - CASE-SWDEP");
}
