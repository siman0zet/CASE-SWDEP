#include "CPluginWindow.h"
#include "ui_CPluginWindow.h"

#include <QDir>
#include <QDebug>
#include <QPluginLoader>
#include <CPluginInterface.h>

#include <view/CModelView.h>

CPluginWindow::CPluginWindow(CModelView *model, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CPluginWindow),
    _dataModel(model->dataModel())
{
    ui->setupUi(this);
    this->setWindowTitle(QString("%1 - Generate SQL CREATE script")
                         .arg(model->name()));
    this->loadPlugins();
}

CPluginWindow::~CPluginWindow()
{
    delete ui;
}

void CPluginWindow::loadPlugins()
{
    ui->comboBox->clear();
    QString extension = "";
    QString prefix = "";

#ifdef Q_OS_WIN
    extension = "dll";
    prefix = "";
#endif

#ifdef Q_OS_LINUX
    extension = "so";
    prefix = "lib";
#endif

    QStringList nameFilter;
    nameFilter << QString("%1*.%2")
                  .arg(prefix)
                  .arg(extension);

    QDir libDir(LIB_PATH);
    QStringList files = libDir.entryList(nameFilter);
    foreach (QString file, files) {
        qDebug() << file;
        QPluginLoader loader(QString("%1/%2")
                             .arg(LIB_PATH)
                             .arg(file));
        loader.load();
        if(!loader.isLoaded())
        {
            qDebug() << "Can't load plugin!";
            qDebug() << "Error @ " + file + ": " + loader.errorString();
            continue;
        }
        QObject *obj = loader.instance();
        CPluginInterface *plugin = qobject_cast<CPluginInterface *>(obj);
        if(plugin)
        {
            QString str = QString("%1 (%2)")
                    .arg(plugin->name())
                    .arg(plugin->version());
            ui->comboBox->addItem(str);
            _plugins.insert(str, plugin);
        }
    }
}

void CPluginWindow::on_pushRefresh_clicked()
{
    this->loadPlugins();
}
