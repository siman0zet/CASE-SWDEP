#ifndef CVIEWMODEL_H
#define CVIEWMODEL_H

#include <QWidget>
#include <QVector>
#include <QPainter>
#include <QPoint>
#include <QMouseEvent>
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QKeyEvent>
#include <QMessageBox>
#include <QToolBar>
#include <QScrollBar>
#include <QStatusBar>
#include <QResizeEvent>
#include <QScrollArea>
#include <CScrollArea.h>

#include "CDataModel.h"

#include "CEntityView.h"
#include "CConnectionView.h"
#include "CDetailedEntityWidget.h"
#include "CConnectionViewWidget.h"
#include "CScriptGenerator.h"

/* CViewModel - это виджет, представляющий собой внешнее отображение модели данных, он способен принимать внешние сигналы и
 * обрабатывать их, а также, он способен сохранять и загружать данные о себе из файла (в файл). */

#include <QTextStream>
#include <QFile>

#include "CLoadAndSaveWidget.h"

class MainWindow;

class CViewModel : public QWidget
{
    Q_OBJECT // Нужно для работы сигналов и слотов
public:
    CViewModel(QWidget *w, QApplication* _app);
    ~CViewModel();

    void loadFromFile(QString file_name); // Загрузить из файла
    void saveInFile(QString file_name); // Сохранить в файл

    void resizeEvent(QResizeEvent* re);

    void pre_saveInFile(QString _name);
    bool pre_loadFromFile(QString _name);

    CViewModel *PDMViewConstructor(CDataModel *model);

    int addEntity(); // Добавить сущность (возвращает id новой сущности)
    void remEntity(int id); // Удалить сущность
    void remEntityCascade(int id); // Удаляет сущность и все связи с ней

    int addConnection(int entity1, int entity2); // Добавить связь (возвращает id новой связи)
    void remConnection(int id); // Удалить связь
    void reconnect(int id, int entity1, int entity2);

    void addViewEntity(int id); // Добавить только представление сущности
    void addViewConnection(int id, int entity1, int entity2); // Добавить только представление связи

    void remViewEntity(int id); // Удалить только представление сущности
    void remViewConnection(int id); // Удалить только представление связи

    void reset_view(); // Удалить все визуальные объекты

    void move_entity(int id, QPoint point); // Перемещает визуальный объект-сущность

    void mousePressEvent(QMouseEvent* me);
    void mouseDoubleClickEvent(QMouseEvent* me);
    void mouseMoveEvent(QMouseEvent* me);
    void mouseReleaseEvent(QMouseEvent* me);

    void keyPressEvent(QKeyEvent* ke);
    void keyReleaseEvent(QKeyEvent* ke);

    void entity_pos_updated(CEntityView* entity); // Изменено положение сущности

    void context_menu_request(QPoint pos);
    void entity_context_menu_request(QPoint pos);
    void connection_context_menu_request(QPoint pos);

    void timerEvent(QTimerEvent* te);

    void clear_entity_selection();
    void unselect_entity(CEntityView* entity);
    void select_entity(CEntityView* entity);

    void move_entity(QPoint point, int id);

    int is_entityes_x_intersected(int entity1, int entity2);

    QVector<CEntityView *> getEntityes();

    bool check_entity_name(QString _name, int id);

    void closeEvent(QCloseEvent *event);

    void cursor_tool_activate();
    void create_entity_tool_activate();
    void remove_entity_tool_activate();
    void one_one_tool_activate();
    void one_many_tool_activate();
    void many_many_tool_activate();
    void aggregate_tool_activate();

    void setMultiplySelection(bool flag);

    MainWindow *mw;

    QVector<CEntityView*> getSelectedEnts() {return selected_entityes;}

    bool isMultipySelEnabled() {return multiply_selection;}

public slots:

    void reset_text();

    void show_entity_detail();
    void remove_selected_entityes();
    void connect_selected_entityes();
    void remove_selected_connection();
    void show_connection_detail();

private slots:

    void detailed_view_destroyed(CDetailedEntityWidget* wid);
    void add_new_entity_event();
    void convert_to_query_event();

    void load_and_save_view_destroyed();
    void conn_view_destroyed(CConnectionViewWidget* wid);
    void save_CDM_event();
    void save_CDM_as_event();
    void load_CDM_event();

    void to_script_event();

signals:

    void closing();

protected:

    void paintEvent(QPaintEvent * event);

    void checkSize();

private:
    bool rect_selection, clear_selection;

    QPoint rect_point, rect_curr_point;

    QVector<CEntityView*> rect_selected;

    QApplication *app;

    QAction *save_CDM, *save_CDM_as, *load_CDM, *to_script;
    QString currFileName;
    CLoadAndSaveWidget* load_and_save_view;
    bool w_opened;

    void disable_all_tools();
    void set_entityes_selectable(bool flag);

    void draw_connection_line(QPoint p1, QPoint p2, CConnection::CONNECTION_MAX_TYPE, CConnection::CONNECTION_MIN_TYPE, QPainter &painter);

    bool check_entity(int id); // Возвращает true, если сущность можно удалить
    bool check_connection(int id); // Возвращает true, если связь можно удалить

    CConnectionView* make_connection(CEntityView* entity1, CEntityView* entity2, CConnection* conn); // Создает соединение

    void setCurrentModel(CDataModel* model); // Установить модель как текущую

    void linesIntersectionCulc();
    void entityesLinesCheck();
    void linesIntersectionCheck(CConnectionView *con1, CConnectionView *con2);

    QVector<CEntityView*> entity_vec;
    QVector<CConnectionView*> connection_vec;

    QVector<CEntityView*> selected_entityes;

    int last_entity_id, last_connection_id;

    CDataModel* data_model;

    QMenu *entity_context_menu;
    QAction *detailed_entity_view, *remove_entity, *connect_entityes;

    QMenu *context_menu;
    QAction *add_new_entity, *convert_to_query;

    QMenu *connection_context_menu;
    QAction *rem_con, *detail_con;

    CViewModel *parent;

    int timerId;

    bool multiply_selection;

    QVector<CDetailedEntityWidget*> detailed_views;
    QVector<CConnectionViewWidget*> connection_views;

    CConnectionView *selected_connection;

    bool cursor_tool, create_entity_tool, remove_entity_tool, one_one_tool, one_many_tool, many_many_tool, agregate_tool;

    CEntityView *tmp_conn_entity;
    QPoint tmp_conn_point;

    CScriptGenerator* ScriptGen;

    QScrollArea *parent_area;
};


#endif // CVIEWMODEL_H
