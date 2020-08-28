#ifndef DBMANAGER_H
#define DBMANAGER_H


#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QStandardPaths>

#include <QDebug>

class dbManager
{
public:
    dbManager(QObject *parent);
    ~dbManager();

    bool addIp_port_toDb(QString pp,int port);                      //Metoda dodająca lub aktualizująca ip i port w bazie danych

    QString readIp_fromDb();                                        //Metoda odczytująca ip z bazy danych
    int readPort_fromDb();                                          //Metoda odczytująca port z bazy danych

    void addOrder_toDb(int ord,int state,QString time);             //Metoda dodający numer,stan oraz czas zamowienia do bazy danych
    void deleteOrder_fromDb(int ord);                               //Metoda usuwajaca dane zamowienie razem ze stanem i czasem z bazy danych
    void updateOrder_inDb(int order,int state);                     //Metoda aktualizująca stan danego zamówienia w bazie danych

    void saveNextNumber_toDb(int number);                           //Metoda aktualizująca nastepne zamówienie w bazie danych
    int getNextNumber_fromDb();                                     //Metoda pobierająca następne zamówienie z bazy danych

    QVector<int> getOrders_fromDb();                                //Metoda pobierająca wszystkie zamówienia z bazy danych
    QVector<int> getStates_fromDb();                                //Metoda pobierająca wszystkie stany z bazy danych
    QVector<QString> getTimes_fromDb();                             //Metoda pobierająca wszystkie czasy z bazy danych

private:
    void startDatabase();                                           //Metoda konfigurująca baze danych przy starcie programu

    QSqlQuery *query;
    QSqlDatabase db;

};

#endif // DBMANAGER_H
