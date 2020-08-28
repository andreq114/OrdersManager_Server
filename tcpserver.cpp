#include "tcpserver.h"

TcpServer::TcpServer(QObject *parent) : QTcpServer(parent)
{
}

TcpServer::~TcpServer()
{
    for(auto client:clientConnection)
        delete client;
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
        for(auto client:clientConnection)
            client->disconnectFromHost();
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
    else if(data == "FULLSCREEN"){
        emit serverFullScreen();
    }
    else if(data == "RESET"){
        emit resetOrders();
    }
    else
        emit onOrderOperation(ordNumber(stringData),stateNumber(stringData));

}

void TcpServer::sendApprove(QVector<int> orders,QVector<int> states,QVector<QString> times,int order)  //Po połączeniu wysylamy potwierdzenie o nawiazaniu polaczenia do klienta
{
        qDebug()<<"Wysylam cos";
        //Pierwsze polaczenie, dodajemy dolara na poczatku łańcucha znaków przed listami zamówień itd.
        QString tekst = "$";
        QByteArray block;
        block.append(tekst);

        clientConnection.append(this->nextPendingConnection());                                             //Dodajemy ostatnio podlaczonego klienta do listy
        connect(clientConnection.last(), &QIODevice::readyRead, this, &TcpServer::readClientCommand);       //Polaczenie sygnalu otrzymania danych od klienta ze slotem
        connect(clientConnection.last(),SIGNAL(disconnected()),this,SLOT(disconnectClient()));              //Polaczenie sygnalu rozlaczenie klienta ze slotem

        clientConnection.last()->write(block);                  //Dodajemy dolara do bloku ostatnio polaczonego klienta

        sendAllOrders(orders,states,times,order);               //Wysyłamy aktualna liste zamówień do wszystkich klientów


}

void TcpServer::sendAllOrders(QVector<int> orders,QVector<int> states,QVector<QString> times,int order){
    //Tworzymy lancuch znakow z zamowien,stanow,czasow oraz nastepnego numeru zamowienia i wysyłamy do wszystkich podłączonych klientów
    QString tekst = makeDatagram(orders,states,times,order);
    QByteArray block;
    block.append(tekst);
    for(auto socket:clientConnection){
        socket->write(block);
        socket->waitForBytesWritten(1000);
    }

}




int TcpServer::ordNumber(QString command)
{
    //Wyłuskanie numeru zamówienia
    QString ord;
    ord = command.mid(command.indexOf("/")+1,command.lastIndexOf("/")-command.indexOf("/")-1);
    return ord.toInt();
}

int TcpServer::stateNumber(QString command)
{
    //Wyłuskanie stanu danego zamówienia
    QString state;
    state = command.mid(command.indexOf("#")+1,command.lastIndexOf("#")-command.indexOf("#")-1);
    return state.toInt();
}

QString TcpServer::makeDatagram(QVector<int> orders,QVector<int> states,QVector<QString> times,int order){
    QString ordersTemp = "";
    QString statesTemp = "";
    QString timesTemp = "";
    QString orderTemp;

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

    for(auto time:times){
        timesTemp.append("#");
        timesTemp.append(time);
    }
    timesTemp.append("#");

    orderTemp.append("@");
    if(order == 99)
        orderTemp.append(QString::number(1));
    else
        orderTemp.append(QString::number(order+1));
    orderTemp.append("@");



    qDebug()<<ordersTemp+statesTemp+timesTemp;

    return ordersTemp+statesTemp+timesTemp+orderTemp;

}


void TcpServer::disconnectClient(){

    for(auto sock:clientConnection)
        if(sender() == sock)
            clientConnection.removeOne(sock);

    disconnect(sender(),SIGNAL(disconnected()),this,SLOT(disconnectClient()));
    sender()->deleteLater();

}
