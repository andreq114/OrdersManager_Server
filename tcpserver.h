#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QMessageBox>
#include <QTranslator>


// ORDER/1/STATE/1
//ORDER - NUMER ZAMÓWIENIA
//STANY:

//2 - zamówienie jest na którejś liście, dodać do listy częściowo zrealizowanych
//3 - zamówienie jest na którejś liście, dodać do listy zrealizowanych
//4 - zamówienie jest na którejś liście, dodać do listy w trakcie realizacji
//5 - usuń zamówienie, nie ważne na której jest liście




// DATAGRAM WYSYŁANY DO KLIENTA
// $ na początku datagramu - pierwsze połączenie i pierwsza wiadomość
// *5*4*3* - numery zamówień
// ^2^3^2^ - stany zamówień

//PRZYKŁAD
// $*5*4*3*^2^3^4^  -- pierwsza wiadomosc dla klienta z potwierdzeniem polaczenia oraz numerami zamowien i stanów
// *4*3*5*^2^3^1^   -- wiadomosc z aktualizacja dla klienta z numerami zamowien i stanów

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent);
    ~TcpServer();

    void startServer();
    void stopServer();
    void sendAllOrders(QVector<int> orders,QVector<int> states,QVector<QString> times,int order);

    void setIpAdress(QString ip);
    void setPortNumber(int port);
    QString getIpAdress();
    int getPortNumber();

signals:
    void isStartingFailed(bool a);
    void serverStopped();

    void onOrderOperation(int order,int state);
    void makeSoundSignal();
    void recoverLastOrder();
    void addNewOrder();
    void serverFullScreen();
    void resetOrders();

private slots:
    void readClientCommand();
    void sendApprove(QVector<int> orders,QVector<int> states,QVector<QString> times,int order);
    void disconnectClient();

private:
    QString ipAdr = "172.16.108.177";
    quint16 port = 51268;
    QVector<QTcpSocket*> clientConnection;

    void connectSignals();
    int ordNumber(QString command);
    int stateNumber(QString command);
    QString makeDatagram(QVector<int> orders,QVector<int> states,QVector<QString> times,int order);

};

#endif // TCPSERVER_H
