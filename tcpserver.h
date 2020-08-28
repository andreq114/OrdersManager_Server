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

    void startServer();                                                                             //Włączenie serwera
    void stopServer();                                                                              //Zatrzymanie serwera
    void sendAllOrders(QVector<int> orders,QVector<int> states,QVector<QString> times,int order);   //Wysłanie do wszystkich podłączonych klientów aktualnej listy zamowien/stanów/czasów/nastepnego numeru zamowienia

    void setIpAdress(QString ip);                                                                   //Setter do ustawienia adresu ip serwera
    void setPortNumber(int port);                                                                   //Setter do ustawienia portu serwera
    QString getIpAdress();                                                                          //Getter do pobrania ip serwera
    int getPortNumber();                                                                            //Getter do pobrania portu serwera

signals:
    void isStartingFailed(bool a);                                                                  //Sygnał czy udało się wystartować serwer
    void serverStopped();                                                                           //Sygnał o zatrzymaniu serwera

    void onOrderOperation(int order,int state);                                                     //Sygnał po otrzymaniu komendy od klienta o zmiany stanu danego zamówienia
    void makeSoundSignal();                                                                         //Sygnał po otrzymaniu komendy od klienta o uzycie sygnału dźwiękowego
    void recoverLastOrder();                                                                        //Sygnał po otrzymaniu komendy od klienta o przywrocenie jednego zamowienia
    void addNewOrder();                                                                             //Sygnał po otrzymaniu komendy od klienta o dodanie nowego zamowienia do listy
    void serverFullScreen();                                                                        //Sygnal po otrzymaniu komendy od klienta o wlaczenie/wylaczenie trybu pelnoekranowego okna widocznego dla klientóW
    void resetOrders();                                                                             //Sygnał po otrzymaniu komendy od klienta o zresetowanie listy zamowien/stanów/czasów i numeru nastepnego zamowienia

private slots:
    void readClientCommand();                                                                       //Slot odczytujacy komende otrzymaną od klienta
    void sendApprove(QVector<int> orders,QVector<int> states,QVector<QString> times,int order);     //Slot przy pierwszym podłączeniu się każdego klienta wysylajacy dodatkowo potwierdzenie polaczenia do klienta
    void disconnectClient();                                                                        //Slot przyjmujacy sygnał o rozłączeniu się klienta od serwera

private:
    QString ipAdr = "172.16.108.177";                                                               //Ip serwera
    quint16 port = 51268;                                                                           //Port serwera
    QVector<QTcpSocket*> clientConnection;                                                          //Lista socketów tj. podłączonych klientów


    int ordNumber(QString command);                                                                 //Metoda wyłuskający numer zamówienia z łańcucha znaków
    int stateNumber(QString command);                                                               //Metoda wyłuskający numer stanu zamówienia z łańcucha znaków
    QString makeDatagram(QVector<int> orders,QVector<int> states,QVector<QString> times,int order); //Metoda tworząca datagram tj. łańcuch znaków z listy zamówień,stanów,czasów i numeru następnego zamówienia

};

#endif // TCPSERVER_H
