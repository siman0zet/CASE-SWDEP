#include "PModelWindow.h"
#include "ui_PModelWindow.h"

#include <view/CModelView.h>
#include <view/CTableItem.h>
#include <view/PModelView.h>

#include <model/CTable.h>

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
