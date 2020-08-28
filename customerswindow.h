#ifndef CUSTOMERSWINDOW_H
#define CUSTOMERSWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QLabel>
#include <QDebug>
#include <thread.h>
#include <QHeaderView>
#include <QtMath>

namespace Ui {
class CustomersWindow;
}

class CustomersWindow : public QMainWindow
{

    Q_OBJECT
public:
    explicit CustomersWindow(QWidget *parent = nullptr);
    ~CustomersWindow();
    void refreshOrdersTables(QVector<int> orders,QVector<int> states);          //Odwiezenie listy zamowień
    void setFullScreen();                                                       //Ustawianie trybu pelnego ekranu dla tego okna

public slots:
    void showNext();                                                            //Jesli zamowien jest wiecej niz 5, przeskakiwanie miedzy ekranami

private:
    Ui::CustomersWindow *ui;
    QHBoxLayout *titleLay;                                                      //Layouty do rozmieszcenia gui
    QHBoxLayout *tabWidgLay;
    QVBoxLayout *centralLayout;
    QVBoxLayout *tabWidgLay2;

    Thread *myThread;

    QLabel *titleLabel;                                                         //Label z napisem "Zamówienia"

    QFont titleFont;                                                            //Czcionki dla tytułu,numerów zamówień i opisu zamówień
    QFont ordersFont;
    QFont descrFont;

    QTableWidget *ordersWidget;                                                 //Główny widget dla zamówień w którym są wyświetlane zamówienia
    QTableWidgetItem *ordItem;                                                  //Itemy z numerem zamówień
    QTableWidgetItem *descItem;                                                 //Itemy z opisem danego zamówienia

    QVector<int> fullReadyOrders;                                               //Wektory z zamówieniami podzielone na zrealizowane,czesciowo zreazlizowane i w trakcie realizacji
    QVector<int> partReadyOrders;
    QVector<int> notReadyOrders;

    QVector<int> allOrders;                                                     //Listy wszystkich zamówień i stanów
    QVector<int> allStates;

    int actualOrdersPage = 1;                                                   //Aktualna strona z zamówieniami
    int maxOrdersPage = 1;                                                      //Liczba wszystkich stron z zamówieniami

    void startPreferences();                                                    //Początkowe ustawienia okna (rysowanie gui)

    void connectSignals();                                                      //Polaczenie sygnałow ze slotami

    void splitOrders(QVector<int> orders,QVector<int> states);                  //Podział zamówień na osobne listy zrealizowanych/czesciowo zrealizowanych/w trakcie realizacji

    void dispTable(int a,int b);                                                //Wyswietlanie tabeli na ekranie


};

#endif // CUSTOMERSWINDOW_H
