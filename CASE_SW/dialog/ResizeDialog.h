#ifndef RESIZEDIALOG_H
#define RESIZEDIALOG_H

#include <QDialog>

namespace Ui {
class ResizeDialog;
}

class ResizeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResizeDialog(int width, int height, QWidget *parent = 0);
    ~ResizeDialog();

private:
    Ui::ResizeDialog *ui;

private slots:
    void collectData();

signals:
    void dialogFinished(int w, int h);
};

#endif // RESIZEDIALOG_H
