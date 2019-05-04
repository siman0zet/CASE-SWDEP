#ifndef CSCRIPTGENERATOR_H
#define CSCRIPTGENERATOR_H

#include <QWidget>

#include <QComboBox>
#include <QString>
#include <QStringList>
#include <QDir>
#include <QPluginLoader>
#include <QMessageBox>
#include <QPushButton>
#include <QTextEdit>

#include <QResizeEvent>

#include <QDebug>

#include "CPluginInterface.h"

class CDataModel;

class CScriptGenerator : public QWidget
{
    Q_OBJECT
public:
    CScriptGenerator(CDataModel* _dm, QApplication *_app);
    ~CScriptGenerator();

    void loadPlugins(QString path = "../");

public slots:

    void pbutton_clicked();

protected:

    void resizeEvent(QResizeEvent* re);

public:

    QVector <CPluginInterface*> plugins;

    QComboBox cb;
    QPushButton pb;
    QTextEdit te;

    CDataModel *dm;

    QApplication *app;
};

#endif // CSCRIPTGENERATOR_H
