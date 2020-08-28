#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QMessageBox>
#include <QNetworkInterface>
#include <tcpserver.h>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QTableWidgetItem>
#include <QGridLayout>
#include <math.h>
#include <QTime>
#include <customerswindow.h>
#include <QCloseEvent>
#include <QSound>



#include <dbmanager.h>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void sendUpdatedOrders(QStringList orders);
    void approveConnect(QVector<int> orders,QVector<int> states,QVector<QString> times,int ord);        //Sygnał z potwierdzeniem nawiązania połączenia

private slots:

    void serverStartingFailed(bool a);                                                                  //Slot wyswietlajacy informacje ze nie udalo sie wystartowac serwera
    void serverStopped();                                                                               //Slot wyswietlajacy informacje ze serwer zostal zatrzymany
    void approveConnection();                                                                           //Slot wysylajacy sygnal do klienta poprzez serwer ze polaczenie zostalo nawiazane

    void changeOrder(int order,int state);                                                              //Slot wykonujacy akcje na zamowienia (zmiana stanu,usuniecie)

    void on_actionKonfiguracja_triggered();                                                             //Slot do konfiguracji ip/portu
    void on_actionInformacje_triggered();                                                               //Slot do wyswietlania aktualnego ip/portu

    void on_actionStartServer_triggered();                                                              //Slot do włączenia serwera

    void on_actionStopServer_triggered();                                                               //Slot do zatrzymania serwera

    void addOrderToList();                                                                              //Slot do dodania nowego zamówienia do listy

    void recoverDeletedOrder();                                                                         //Slot do przywrócenia jednego zamówienia


    void on_actionFull_screen_triggered();                                                              //Slot do włączenia trybu pełnoekranowego tego okna

    void ordersReset();                                                                                 //Slot do zresetowania wszystkich zamówień i numeru nastepnego zamówienia

    void soundSignal();                                                                                 //Slot wywolujacy sygnał dźwiękowy

private:
    Ui::MainWindow *ui;
    TcpServer *server;                                                                                  //Klasa do operowania na serwerze i polaczeniem z klientami
    CustomersWindow *custWind;                                                                          //Klasa z oknem wyswietlanym dla klientów
    dbManager *db;                                                                                      //Klasa do operowania na bazie danych

    QVector<QString> ordersNotReady;                                                                    //Wektory z zamowieniami rozlozonymi na 3 listy
    QVector<QString> ordersPartReady;
    QVector<QString> ordersReady;


    QVector<int> orders;
    QVector<int> ordersState;
    QVector<QString> ordersTime;

    QVector<int> deletedOrders;
    QVector<int> deletedStates;
    QVector<QString> deletedTimes;

    QSound *bell;




    QFont ordersFont;

    QGridLayout *ordersLayout;

    int order = 0;

    void connectSignals();                                                                              //Polączenie sygnałow ze slotami
    void refreshOrders();                                                                               //Odswiezenie zamówień w oknie
    void sortOrders();                                                                                  //Sortowanie zamówień w kolejnosci rosnacej
    void loadSettingsFromDB();                                                                          //Zaladowanie danych z bazy danych

    void closeEvent(QCloseEvent *event);



};
#endif // MAINWINDOW_H
