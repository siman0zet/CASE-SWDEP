#include "CScriptGenerator.h"

#include "CDataModel.h"

#include <QApplication>
#include <QDebug>

CScriptGenerator::CScriptGenerator(CDataModel *_dm, QApplication *_app): QWidget(NULL), cb(this), pb(this), te(this), dm(_dm), app(_app)
{
    this->setGeometry(10, 10, 700, 500);

    cb.setGeometry(10, 10, 100, 30);
    pb.setGeometry(140, 10, 100, 30);
    te.setGeometry(10, 50, 680, 440);

    if (app)
        pb.setText(app->translate("Convert", "ScriptGen"));
    else
        pb.setText(tr("Convert"));
    pb.show();

    connect(&pb, SIGNAL(clicked()), this, SLOT(pbutton_clicked()));
}

CScriptGenerator::~CScriptGenerator()
{

}

void CScriptGenerator::loadPlugins(QString path)
{
#ifdef Q_OS_WIN32
    QString enlargement = ".dll";   // Для Windows
    QString prefix = "";
#endif

#ifdef Q_OS_LINUX
    QString enlargement = ".so";    // Для Linux
    QString prefix = "lib";         // В Linux добовляется приставка lib...
#endif



    QStringList combo;

    QDir findPlugin(path);
    //qDebug()<<findPlugin.entryList().contains(prefix + readPluginsName.at(i) + enlargement);
    QStringList filesList = findPlugin.entryList();
    for(int i = 2;i < filesList.size(); i++)
    {
        QPluginLoader loader(path + "/" + filesList[i]);
        loader.load();
        if(loader.isLoaded() == false)
        {
            qDebug() << QString("Can't load a plugin");
            /*QMessageBox msgBoxError;
            msgBoxError.setWindowTitle("Ошибка");
            msgBoxError.setIcon(QMessageBox::Critical);
            msgBoxError.setText("Ошибка загрузки плагина "+ filesList[i] + loader.errorString());
            msgBoxError.exec();*/
            qDebug() <<"Error! " + filesList[i] + ": " + loader.errorString();
            continue;
        }
        QObject * obj = loader.instance();
        CPluginInterface* plugin = qobject_cast<CPluginInterface *>(obj);
        if(plugin)
        {
            combo<<plugin->getName() + " (" + plugin->getVersion() + ")";
            plugins.push_back(plugin);
        }
    }

    cb.clear();
    cb.addItems(combo);
}



void CScriptGenerator::pbutton_clicked()
{
    QString resscript;
    for(int i=0;i<plugins.size();i++)
    {
        if (plugins[i]->getName() + " (" + plugins[i]->getVersion() + ")" == cb.currentText())
        {
            resscript = plugins[i]->getQuery(dm);
            break;
        }
    }
    te.setText(resscript);
}

void CScriptGenerator::resizeEvent(QResizeEvent *re)
{
    te.setGeometry(10, 50, re->size().width() - 20, re->size().height() - 60);
}

