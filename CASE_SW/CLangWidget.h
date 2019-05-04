#ifndef CLANGWIDGET_H
#define CLANGWIDGET_H

#include <QWidget>
#include <QStringList>
#include <QComboBox>
#include <QLabel>
#include <QTranslator>

class CLangWidget : public QWidget
{
    Q_OBJECT
public:
    CLangWidget(QStringList list, QString selected_lang);
    ~CLangWidget();

public slots:

    void reset_text();

signals:

    void lang_selected(QString _lang);

private:

    QComboBox cb;
    QLabel l;
};

#endif // CLANGWIDGET_H
