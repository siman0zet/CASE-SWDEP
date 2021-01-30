#include "cpluginwindow.h"
#include "pmodelwindow.h"
#include "ui_pmodelwindow.h"

#include <view/cmodelview.h>
#include <view/ctableitem.h>
#include <view/pmodelview.h>

#include <model/ctable.h>

PModelWindow::PModelWindow(CModelView *cModel, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PModelWindow),
    _model(new PModelView(cModel))
{
    ui->setupUi(this);
    this->setWindowTitle(QString("%1 - Physical Model").arg(cModel->name()));

    QScrollArea* scroll = new QScrollArea();
    scroll->setWidgetResizable(true);
    scroll->setWidget(_model);
    this->setCentralWidget(scroll);
}

PModelWindow::~PModelWindow()
{
    delete ui;
}

void PModelWindow::on_actionScript_triggered()
{
    CPluginWindow *sWindow = new CPluginWindow(_model);
    sWindow->show();
}

void PModelWindow::on_actionChange_Size_triggered()
{
    _model->showResizeDialog();
}
