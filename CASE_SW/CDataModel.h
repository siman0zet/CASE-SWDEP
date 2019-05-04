#ifndef CDATAMODEL_H
#define CDATAMODEL_H

#include "CEntity.h"
#include "CConnection.h"

#include "CDataModelException.h"

#include <QVector>
#include <QString>

class QApplication;

/* CDataModel - класс модели данных (PDM или CDM), представляет собой набор сущностей и связей между ними,
 * когда речь идет о  PDM, связи между сущностями носят фиктивный характер: связь осуществляется при
 * помощи внешних ключей. */

class CViewModel;

class CDataModel
{
    friend class CViewModel;
public:
    CDataModel(QApplication *_app);
    CDataModel(CDataModel& rhs);
    ~CDataModel();

    int addEntity(); // Добавить сущность (возвращает id добавленной сущности)
    void remEntity(int id); // Удаляет сущность с кодом id
    void remEntityCascade(int id); // Удаляет сущность и все связи с ней

    CEntity *getEntity(int id); // Возвращает указатель на сущность с кодом id

    int addConnection(int entity_id_1, int entity_id_2); // Добавляет связь между двумя сущностями, возвращает id связи
    void remConnection(int id); // Удаляет связь с кодом id

    CConnection *getConnection(int id); // Возвращает указатель на связь с кодом id

    void reset(); // Очищает модель

    void loadFromFile(QString file_name); // Загрузить из файла (старые данные модели теряются)
    void saveInFile(QString file_name); // Сохранить в файл

    CDataModel *convertToPDM(); // Возвращает указатель на PDM
    bool PrimaryKeysCheck();
    QString makeScript(); // Создает скрипт на создание БД (работает только с PDM)

    void reconnect(int conn_id, int entity_id_1, int entity_id_2);

    int getEntityLasrId();
    int getConnectionLastId();

    void setViewModel(CViewModel* _view);
    CViewModel* getViewModel();

    void setValid(bool flag);

    bool isValid();
    bool is_PDM();

    int getErrorCode();

    void removeAll();
    void setLastEntityId(int _id);

    QVector<CEntity*> getTables()
    {
        return entity_vec;
    }

private:
    bool tearing_check(QVector<CConnection*> &vec);
    bool loop_check(QVector<CConnection*> &vec);

    bool looping();

    void addKeys(CEntity* to, CEntity* from, bool isPK = true, bool isUNIQ = true, bool isNNULL = true);
    void addRows(CEntity* to, CEntity* from);

    bool isPDM;

    QVector<CEntity*> entity_vec;
    QVector<CConnection*> connection_vec;

    int last_entity_id, last_connection_id;

    bool is_valid;
    int error_code;

    CViewModel* view;

    QApplication *app;

    QVector<CRow::loaded_type> loaded_types;
};

#endif // CDATAMODEL_H
