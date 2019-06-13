#ifndef PMODELWINDOW_H
#define PMODELWINDOW_H

#include <QMainWindow>

namespace Ui {
class PModelWindow;
}

class CModelView;
class PModelView;

class PModelWindow : public QMainWindow
{
    Q_OBJECT

public:
    PModelWindow(CModelView *cModel, QWidget *parent = 0);
    ~PModelWindow();

private:
    Ui::PModelWindow *ui;

    PModelView *_model;
};

#endif // PMODELWINDOW_H
