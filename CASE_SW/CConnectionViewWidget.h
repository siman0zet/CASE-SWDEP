#ifndef CCONNECTIONVIEWWIDGET_H
#define CCONNECTIONVIEWWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QVector>
#include <QString>
#include <QStringList>
#include <QLabel>

class CViewModel;
class CConnectionView;
class CEntityView;

class CConnectionViewWidget : public QWidget
{
    Q_OBJECT
public:
    CConnectionViewWidget(CViewModel *_parent, CConnectionView *con_view);

    void update_state();

public slots:

    void left_entity_changed(QString _name);
    void right_entity_changed(QString _name);

    void left_lower_num_changed(int ind);
    void right_lower_num_changed(int ind);

    void left_upper_num_changed(int ind);
    void right_upper_num_changed(int ind);

    CConnectionView* getConnection();

signals:

    void closing(CConnectionViewWidget* ptr);

protected:

    void closeEvent(QCloseEvent * event);

private:
    void update_left_lower_nums();
    void update_right_lower_nums();

    void update_left_upper_nums();
    void update_right_upper_nums();

    bool inserted;

    CViewModel *parent;
    CConnectionView *connection_view;

    QComboBox *left_entity, *right_entity;
    QComboBox *left_lower_num, *right_lower_num;
    QComboBox *left_upper_num, *right_upper_num;
    QLabel *left_entity_text, *right_entity_text;
    QLabel *left_lower_num_text, *right_lower_num_text;
    QLabel *left_upper_num_text, *right_upper_num_text;

    QVector<CEntityView*> entity_vec;
};

#endif // CCONNECTIONVIEWWIDGET_H
