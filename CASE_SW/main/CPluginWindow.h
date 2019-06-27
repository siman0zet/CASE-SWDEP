#ifndef CPLUGINWINDOW_H
#define CPLUGINWINDOW_H

#include <QMainWindow>
#include <QMap>

namespace Ui {
class CPluginWindow;
}

class CModelView;
class CDataModel;
class CPluginInterface;

class CPluginWindow : public QMainWindow
{
    Q_OBJECT

public:
    CPluginWindow(CModelView *model, QWidget *parent = 0);
    ~CPluginWindow();

private slots:
    void on_pushRefresh_clicked();

private:
    Ui::CPluginWindow *ui;

    CDataModel *_dataModel;
    QMap<QString, CPluginInterface *> _plugins;
    void loadPlugins();
};

#endif // CPLUGINWINDOW_H
