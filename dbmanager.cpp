#include "dbmanager.h"

dbManager::dbManager(QObject *parent)
{
    startDatabase();
}

void dbManager::startDatabase(){
    const QString DRIVER("QSQLITE");
    if(QSqlDatabase::isDriverAvailable(DRIVER))
        qDebug()<<"Jest driver";

    db = QSqlDatabase::addDatabase(DRIVER,"database");

    db.setDatabaseName("F:/OrdersManager_Server/OrdersManager_Server/myDB");


    if(!db.open())
        qWarning() << "ERROR: " << db.lastError();
    query = new QSqlQuery(db);

    //Tabele juz stworzone, komentujemy
    //query->exec("CREATE TABLE orders (id INTEGER PRIMARY KEY, orderNumber INTEGER, orderState INTEGER,orderTime VARCHAR(10))");

    // query->exec("CREATE TABLE config (id INTEGER PRIMARy KEY, ipAdress VARCHAR(20), port INTEGER)");

    if(!query->isActive())
        qWarning() << "ERROR: " << query->lastError().text();


}

bool dbManager::addIp_port_toDb(QString ip,int port){

    query->prepare("UPDATE config SET ipAdress = :ip WHERE id = 1");
    query->bindValue(":ip", ip);
    if(!query -> exec())
        qWarning() << "ERROR: " << query->lastError().text();
    query->clear();



    query->prepare("UPDATE config SET port = :por WHERE id = 1");
    query->bindValue(":por", port);
    if(!query -> exec())
        qWarning() << "ERROR: " << query->lastError().text();
    query->clear();

    return true;
}


QString dbManager::readIp_fromDb(){
    query->clear();
    if(!query -> exec("SELECT ipAdress FROM config WHERE id = 1"))
        qWarning() << "ERROR: " << query->lastError().text();

    query->first();
    qDebug()<<"IP z bazy "<<query->value(0).toString();

    return query->value(0).toString();
};

int dbManager::readPort_fromDb(){
    query->clear();
    if(!query -> exec("SELECT port FROM config WHERE id = 1"))
        qWarning() << "ERROR: " << query->lastError().text();

    query->first();
    qDebug()<<"port z bazy "<<query->value(0).toString();

    return query->value(0).toInt();
};



void dbManager::addOrder_toDb(int ord, int state, QString time){

    query->prepare("INSERT INTO orders(orderNumber,orderState,orderTime) VALUES (?,?,?)");
    query->addBindValue(ord);
    query->addBindValue(state);
    query->addBindValue(time);
    if(!query -> exec())
        qWarning() << "ERROR: " << query->lastError().text();
    query->clear();

}

void dbManager::deleteOrder_fromDb(int ord){
    query->prepare("DELETE FROM orders WHERE orderNumber = :order");
    query->bindValue(":order",ord);
    if(!query -> exec())
        qWarning() << "ERROR: " << query->lastError().text();
    query->clear();

}

void dbManager::updateOrder_inDb(int order, int state){
    query->prepare("UPDATE orders SET orderState = :state WHERE orderNumber = :order");
    query->bindValue(":state", state);
    query->bindValue(":order",order);
    if(!query -> exec())
        qWarning() << "ERROR: " << query->lastError().text();
    query->clear();
}




void dbManager::saveNextNumber_toDb(int number){
    query->prepare("UPDATE next_number SET nextNumber = :number");
    query->bindValue(":number", number);
    if(!query -> exec())
        qWarning() << "ERROR: " << query->lastError().text();
    query->clear();
}

int dbManager::getNextNumber_fromDb(){
    int value = 0;
    query->prepare("SELECT nextNumber FROM next_number WHERE id = 1");
    if(!query -> exec())
        qWarning() << "ERROR: " << query->lastError().text();
    if(query->next())
        value = query->value(0).toInt();
    query->clear();

    return value;

}




QVector<int> dbManager::getOrders_fromDb(){
    QVector<int> orders;

    query->prepare("SELECT COUNT(*) FROM orders");
    if(!query -> exec())
        qWarning() << "ERROR: " << query->lastError().text();
    int numbOfOrders = 0;
    if (query->next()) {
        numbOfOrders= query->value(0).toInt();
    }
    query->clear();



    if(numbOfOrders == 0){

    }else{
        query->prepare("SELECT orderNumber from orders");
        if(!query -> exec())
            qWarning() << "ERROR: " << query->lastError().text();

        for(int i=0;i<numbOfOrders;i++){
            if (query->next()) {
                orders.append(query->value(0).toInt());
            }
        }


    }
    query->clear();

    return orders;

}

QVector<int> dbManager::getStates_fromDb(){
    QVector<int> states;

    query->prepare("SELECT COUNT(*) FROM orders");
    if(!query -> exec())
        qWarning() << "ERROR: " << query->lastError().text();
    int numbOfStates = 0;
    if (query->next()) {
        numbOfStates= query->value(0).toInt();
    }
    query->clear();


    if(numbOfStates == 0){

    }else{
        query->prepare("SELECT orderState from orders");
        if(!query -> exec())
            qWarning() << "ERROR: " << query->lastError().text();

        for(int i=0;i<numbOfStates;i++){
            if (query->next()) {
                states.append(query->value(0).toInt());

            }
        }


    }
    query->clear();
    return states;


}

QVector<QString> dbManager::getTimes_fromDb(){
    QVector<QString> times;

    query->prepare("SELECT COUNT(*) FROM orders");
    if(!query -> exec())
        qWarning() << "ERROR: " << query->lastError().text();
    int numbOfTimes = 0;
    if (query->next()) {
        numbOfTimes= query->value(0).toInt();
    }
    query->clear();



    if(numbOfTimes == 0){

    }else{
        query->prepare("SELECT orderTime from orders");
        if(!query -> exec())
            qWarning() << "ERROR: " << query->lastError().text();

        for(int i=0;i<numbOfTimes;i++){
            if (query->next()) {
                times.append(query->value(0).toString());

            }
        }


    }
    query->clear();
    return times;
}
