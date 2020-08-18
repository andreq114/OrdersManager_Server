#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    server = new TcpServer(this);
    connectSignals();
    ui->ordersTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->ordersTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::connectSignals(){
    connect(server,SIGNAL(isStartingFailed(bool)),this,SLOT(serverStartingFailed(bool)));
    connect(server, SIGNAL(newConnection()), this, SLOT(approveConnection()));
    //connect(clientConnection, &QIODevice::readyRead, this, &MainWindow::sendAllOrders); NIE ODKOMENTOWAC
    connect(server,SIGNAL(serverStopped()),this,SLOT(serverStopped()));
    connect(server,SIGNAL(onOrderOperation(int,int)),this,SLOT(changeOrder(int,int)));
    connect(this,SIGNAL(approveConnect(QVector<int>,QVector<int>)),server,SLOT(sendApprove(QVector<int>,QVector<int>)));
}

void MainWindow::changeOrder(int order,int state){

    //TU SIE WYKONUJE PROCEDURA OPERACJI NA LISTACH ZAMOWIEN ( DODANIE , ZMIANA , USUNIECIE)
    qDebug()<<"NUMER ZAMOWIENIA TO"<<order;
    qDebug()<<"STAN ZAMOWIENIA TO"<<state;

    orders.append(order);
    ordersState.append(state);
    server->sendAllOrders(orders,ordersState);
    refreshOrders();

    //METODA DO WSTAWIANIA ZAMOWIENIA NA PANELU DLA KLIENTÓW

    //


    //PO ZAKTUALIZOWANIU LIST ZAMOWIEN
    //emit sendUpdatedOrders(QStringList orders);
}

void MainWindow::approveConnection(){
    emit approveConnect(this->orders,this->ordersState);
}








void MainWindow::serverStartingFailed(bool a){
    if(a == true){
        QMessageBox::information(this, tr("Orders System Server"),
                          tr("Unable to start the server: %1.")
                          .arg(server->errorString()));
    }else{
        QMessageBox::information(this, tr("Orders System Server"),
                          tr("The server was created successfully"));

    }
}

void MainWindow::serverStopped(){
    QMessageBox::information(this,"Informacja","Server został zatrzymany",QMessageBox::Ok);
}

void MainWindow::on_actionKonfiguracja_triggered() //Okienko do zmiany adresu ip i portu serwera
{
    QDialog dialog(this);
    QFormLayout lay(&dialog);

    lay.addRow(new QLabel("Adres IP serwera:"));
    QLineEdit *ipAdress = new QLineEdit (&dialog);
    ipAdress->setInputMask("000.000.000.000;_");
    lay.addRow (ipAdress);

    lay.addRow(new QLabel("Port serwera:"));
    QLineEdit *portNumber = new QLineEdit (&dialog);
    portNumber->setInputMask("00000;_");
    lay.addRow(portNumber);

    // Dodaj przycisk Anuluj i OK
    QDialogButtonBox buttonBox (QDialogButtonBox :: Ok | QDialogButtonBox :: Cancel                                ,
        Qt :: Horizontal, & dialog);
    lay.addRow ( & buttonBox);
    QObject :: connect ( & buttonBox, SIGNAL (rejected ()), & dialog, SLOT (reject ()));
    QObject :: connect ( & buttonBox, SIGNAL (accepted ()), & dialog, SLOT (accept ()));
    if (dialog.exec() == QDialog::Accepted) {

        server->setIpAdress(ipAdress->text());

        if(portNumber->text().isEmpty())
            server->setPortNumber(0);
        else
         server->setPortNumber(portNumber->text().toInt());
    }

}


void MainWindow::on_actionInformacje_triggered()
{
    QDialog dialog(this);
    QFormLayout lay(&dialog);

    lay.addRow(new QLabel("Adres IP serwera:"));
    lay.addRow (new QLabel(server->getIpAdress()));
    lay.addRow(new QLabel("Port serwera:"));
    lay.addRow(new QLabel(QString::number(server->getPortNumber())));

    // Dodaj przycisk Anuluj i OK
    QDialogButtonBox buttonBox (QDialogButtonBox :: Ok,
        Qt :: Horizontal, & dialog);
    lay.addRow ( & buttonBox);
    QObject :: connect ( & buttonBox, SIGNAL (accepted ()), & dialog, SLOT (reject ()));
    dialog.exec();

}


void MainWindow::on_actionStartServer_triggered()
{
    if(server->isListening())
        QMessageBox::information(this,"Błąd","Serwer jest już w trakcie pracy");
    else
    server->startServer();

    qDebug()<<"IP serwera: "<<server->serverAddress().toString();
    qDebug()<<"\nPort serwera: "<<server->serverPort();
}

void MainWindow::on_actionStopServer_triggered()
{
    if(!server->isListening())
        QMessageBox::information(this,"Błąd","Serwer jestj już zatrzymany");
    else
        server->stopServer();
}


void MainWindow::refreshOrders(){
    QStringList ordList;
    int row = 0;
    int col = 0;
    QTableWidgetItem *ordItem;

    for(int i=0;i<orders.length();i++){
        ordList.append(QString::number(orders.at(i)));
    }
    ui->ordersTable->clear();
    ui->ordersTable->setColumnCount(5);

    /*if(ordList.length() < 5)
        ui->ordersTable->setRowCount(1);
    if(ordList.length() >= 5 && ordList.length() < 10)
        ui->ordersTable->setRowCount(2);
    if(ordList.length() >= 10 && ordList.length() < 15)
        ui->ordersTable->setRowCount(3);*/

    ui->ordersTable->setRowCount(static_cast<int>(ceil(static_cast<double>(ordList.length())/5)));

    qDebug()<<ceil(static_cast<int>(static_cast<double>(ordList.length())/5));
    for(int i=0;i<ordList.length();i++){
        ordItem = new QTableWidgetItem(ordList.at(i));
        if(ordersState.at(i) == 1)
            ordItem->setBackgroundColor(Qt::yellow);
        if(ordersState.at(i) == 2)
            ordItem->setBackgroundColor(Qt::green);
        if(ordersState.at(i) == 3){
            ordItem->setBackgroundColor(Qt::green);
        }

        /*if(i < 5)
            row = 0;
        if(i >= 5 && i < 10)
            row = 1;*/

        row = static_cast<int>(ceil(static_cast<double>(i+1)/5))-1;
        qDebug()<<row;
        col = i % 5;
        qDebug()<<"Dodaje item na pozycji "<<row<<" "<<col;

        ui->ordersTable->setItem(row,col,ordItem);


    }









}
