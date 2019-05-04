#ifndef CCONNECTION_H
#define CCONNECTION_H

/* Класс связи между ребрами. Следует учесть, что верхнее кардинальное число может иметь тип AGREGATE, в этом случае
 * нужно проследить за тем, что AGREGATE не может быть и левым и правым числом одновременно (два класса
 * не могут агрегировать друг друга). */

class CConnection
{
    friend class CDataModel;

public:
    enum CONNECTION_MAX_TYPE {ONE, MANY, AGREGATE, PHYSICAL}; // Верхнее кардинальное число
    enum CONNECTION_MIN_TYPE {NONE, MANDATORY, EMPTY}; // Нижнее кардинальное число

    CConnection(int _id, int entity_id_1, int entity_id_2); // id - код соединения, entity_id_1 и entity_id_2 - коды ребер
    ~CConnection();

    void setLeftMaxType(CONNECTION_MAX_TYPE _type); // Установить верхние кардинальные числа
    void setRightMaxType(CONNECTION_MAX_TYPE _type);

    void setLeftMinType(CONNECTION_MIN_TYPE _type); // Установить нижние кардинальние числа
    void setRightMinType(CONNECTION_MIN_TYPE _type);

    int get_left_entity_id(); // Возвращает id сущностей
    int get_right_entity_id();

    CONNECTION_MAX_TYPE getLeftMaxType();
    CONNECTION_MAX_TYPE getRightMaxType();

    CONNECTION_MIN_TYPE getLeftMinType();
    CONNECTION_MIN_TYPE getRightMinType();

    int getId() const;

private:
    void setConnection(int entity_id_1, int entity_id_2); // private, чтобы только класс DataModel имел доступ к этой функции

    int id, left_id, right_id;

    CONNECTION_MAX_TYPE left_upper_num, right_upper_num;
    CONNECTION_MIN_TYPE left_lower_num, right_lower_num;
};

#endif // CCONNECTION_H
