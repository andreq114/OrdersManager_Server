#ifndef DBMANAGER_H
#define DBMANAGER_H


#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>

#include <QDebug>

class dbManager
{
public:
    dbManager(QObject *parent);

    bool addIp_port_toDb(QString pp,int port);

    QString readIp_fromDb();
    int readPort_fromDb();

    void addOrder_toDb(int ord,int state,QString time);
    void deleteOrder_fromDb(int ord);
    void updateOrder_inDb(int order,int state);

    void saveNextNumber_toDb(int number);
    int getNextNumber_fromDb();

    QVector<int> getOrders_fromDb();
    QVector<int> getStates_fromDb();
    QVector<QString> getTimes_fromDb();

private:
    void startDatabase();


    QString dbPath = "";
    QSqlQuery *query;
    QSqlDatabase db;

};

#endif // DBMANAGER_H
