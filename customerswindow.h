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
    void refreshOrdersTables(QVector<int> orders,QVector<int> states);
    void setFullScreen();

public slots:
    void showNext();

private:
    Ui::CustomersWindow *ui;
    QHBoxLayout *titleLay;
    QHBoxLayout *tabWidgLay;
    QVBoxLayout *centralLayout;
    QVBoxLayout *tabWidgLay2;

    Thread *myThread;

    QLabel *titleLabel;

    QFont titleFont;
    QFont ordersFont;
    QFont descrFont;

    QTableWidget *ordersWidget;
    QTableWidgetItem *ordItem;
    QTableWidgetItem *descItem;

    QVector<int> fullReadyOrders;
    QVector<int> partReadyOrders;
    QVector<int> notReadyOrders;
    QVector<int> allOrders;
    QVector<int> allStates;
    int actualOrdersPage = 1;
    int maxOrdersPage = 1;

    void startPreferences();

    void connectSignals();

    void splitOrders(QVector<int> orders,QVector<int> states);

    void dispTable(int a,int b);


};

#endif // CUSTOMERSWINDOW_H
