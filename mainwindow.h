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
    void approveConnect(QVector<int> orders,QVector<int> states,QVector<QString> times,int ord);

private slots:
    //void sendApprove();
    void serverStartingFailed(bool a);
    void serverStopped();
    void approveConnection();

    void changeOrder(int order,int state);


    void on_actionKonfiguracja_triggered();
    void on_actionInformacje_triggered();


    void on_actionStartServer_triggered();

    void on_actionStopServer_triggered();

    void addOrderToList();

    void recoverDeletedOrder();


private:
    Ui::MainWindow *ui;
    TcpServer *server;
    CustomersWindow *custWind;

    QVector<QString> ordersNotReady;
    QVector<QString> ordersPartReady;
    QVector<QString> ordersReady;

    QVector<int> orders;
    QVector<int> ordersState;
    QVector<QString> ordersTime;
    QVector<int> deletedOrders;
    QVector<int> deletedStates;
    QVector<QString> deletedTimes;

    QFont ordersFont;

    QGridLayout *ordersLayout;

    int order = 0;

    //QVector<QTcpSocket*> clientConnection;
    void connectSignals();
    void refreshOrders();
    void sortOrders();


    //void readClientCommand();
    //void sendAllOrders();

};
#endif // MAINWINDOW_H
