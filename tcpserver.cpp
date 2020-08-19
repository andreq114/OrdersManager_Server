#include "tcpserver.h"

TcpServer::TcpServer(QObject *parent) : QTcpServer(parent)
{
    connectSignals();
}

TcpServer::~TcpServer()
{

}
void TcpServer::connectSignals(){
        //connect(this, &QTcpServer::newConnection, this, &TcpServer::sendApprove);
}
void TcpServer::startServer(){
    if (!this->listen(QHostAddress(ipAdr),port)) {
        emit isStartingFailed(true);
    }else{
        emit isStartingFailed(false);
        qDebug()<<"Stworzono serwer";
    }
}
void TcpServer::stopServer(){
    if(this->isListening()){
        this->close();

        emit serverStopped();
    }
}


void TcpServer::setIpAdress(QString ip){
    ipAdr = ip;
}
void TcpServer::setPortNumber(int port){
    this->port = QString::number(port).toUShort();
}
QString TcpServer::getIpAdress(){
    return ipAdr;}
int TcpServer::getPortNumber(){
    return port;
}


void TcpServer::readClientCommand(){
    qDebug()<<"Odebrałem ramke";
    QByteArray data = "999";
    qDebug()<<"liczba podlaczonych klientow "<<clientConnection.length();
    for (auto socket:clientConnection){
        if(socket->bytesAvailable() > 0){
            data = socket->readAll();
        }
    }
    QString stringData = data;
    qDebug()<<stringData;
    if(data == "SOUND")
        emit makeSoundSignal();
    else if(data == "RECOVER")
        emit recoverLastOrder();
    else if(data == "ADD"){
        emit addNewOrder();
    }
    else
        emit onOrderOperation(ordNumber(stringData),stateNumber(stringData));

    qDebug()<<data;
}


void TcpServer::sendApprove(QVector<int> orders,QVector<int> states)  //Po połączeniu wysylamy potwierdzenie o nawiazaniu polaczenia do klienta
{
        qDebug()<<"Wysylam cos";
        QString tekst = "$";
        QByteArray block;
        block.append(tekst);

        qDebug()<<block;
        clientConnection .append(this->nextPendingConnection());
        //connect(clientConnection.la, &QAbstractSocket::disconnected,clientConnection, &QObject::deleteLater);
        connect(clientConnection.last(), &QIODevice::readyRead, this, &TcpServer::readClientCommand);

        clientConnection.last()->write(block);

        sendAllOrders(orders,states);
        //clientConnection->disconnectFromHost();

}

void TcpServer::sendAllOrders(QVector<int> orders,QVector<int> states){
    QString tekst = makeDatagram(orders,states);
    QByteArray block;
    block.append(tekst);
    for(auto socket:clientConnection){
        socket->write(block);
        socket->waitForBytesWritten(1000);
    }

    /*block.append(tekst);
    clientConnection = server->nextPendingConnection();
    connect(clientConnection, &QAbstractSocket::disconnected,clientConnection, &QObject::deleteLater);

    clientConnection->write(block);*/
    //clientConnection->disconnectFromHost();
}




int TcpServer::ordNumber(QString command)       //Wyłuskanie numeru zamówienia
{
    QString ord;
    ord = command.mid(command.indexOf("/")+1,command.lastIndexOf("/")-command.indexOf("/")-1);
    qDebug()<<"Zamowienie"<<ord;
    return ord.toInt();
}

int TcpServer::stateNumber(QString command)     //Wyłuskanie stanu danego zamówienia
{
    QString state;
    state = command.mid(command.indexOf("#")+1,command.lastIndexOf("#")-command.indexOf("#")-1);
    qDebug()<<"Stan"<<state;
    return state.toInt();
}

QString TcpServer::makeDatagram(QVector<int> orders,QVector<int> states){
    QString ordersTemp = "";
    QString statesTemp = "";

    for(auto ord:orders){
        ordersTemp.append("*");
        ordersTemp.append(QString::number(ord));
    }
    ordersTemp.append("*");

    for(auto state:states){
        statesTemp.append("^");
        statesTemp.append(QString::number(state));
    }
    statesTemp.append("^");

    return ordersTemp+statesTemp;

}
