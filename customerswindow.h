#ifndef CUSTOMERSWINDOW_H
#define CUSTOMERSWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QLabel>
#include <QDebug>

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

private:
    Ui::CustomersWindow *ui;
    QHBoxLayout *titleLay;
    QHBoxLayout *tabWidgLay;
    QVBoxLayout *centralLayout;
    QVBoxLayout *tabWidgLay2;

    QLabel *titleLabel;

    QFont titleFont;
    QFont ordersFont;

    QVector<QTableWidget*> ordersTables;

    QVector<int> fullReadyOrders;
    QVector<int> partReadyOrders;
    QVector<int> notReadyOrders;

    void startPreferences();
    void setTablesNumber();

    void splitOrders(QVector<int> orders,QVector<int> states);


};

#endif // CUSTOMERSWINDOW_H
