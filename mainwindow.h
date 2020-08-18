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
    void approveConnect(QVector<int> orders,QVector<int> states);

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

private:
    Ui::MainWindow *ui;
    TcpServer *server;

    QVector<QString> ordersNotReady;
    QVector<QString> ordersPartReady;
    QVector<QString> ordersReady;

    QVector<int> orders;
    QVector<int> ordersState;

    QGridLayout *ordersLayout;

    //QVector<QTcpSocket*> clientConnection;
    void connectSignals();
    void refreshOrders();

    //void readClientCommand();
    //void sendAllOrders();

};
#endif // MAINWINDOW_H
