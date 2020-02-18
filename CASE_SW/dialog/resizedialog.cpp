#include "resizedialog.h"
#include "ui_resizedialog.h"
#include <QRegExpValidator>

ResizeDialog::ResizeDialog(int width, int height, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResizeDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    ui->lineWidth->setValidator(new QRegExpValidator(QRegExp("[0-9]{1,255}")));
    ui->lineHeight->setValidator(new QRegExpValidator(QRegExp("[0-9]{1,255}")));

    ui->lineWidth->setText(QString::number(width));
    ui->lineHeight->setText(QString::number(height));

    connect(this, SIGNAL(accepted()), this, SLOT(collectData()));
}

ResizeDialog::~ResizeDialog()
{
    delete ui;
}

void ResizeDialog::collectData()
{
    emit dialogFinished(ui->lineWidth->text().toInt(), ui->lineHeight->text().toInt());
}
