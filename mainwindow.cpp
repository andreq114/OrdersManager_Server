#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    server = new TcpServer(this);
    custWind = new CustomersWindow(this);
    custWind->show();
    custWind->refreshOrdersTables(orders,ordersState);
    connectSignals();
    ui->ordersTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->ordersTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ordersFont.setBold(true);
    ordersFont.setPointSize(20);

    db = new dbManager(this);


    loadSettingsFromDB();


}

MainWindow::~MainWindow()
{
    delete ui;
    delete db;
    delete server;
    delete custWind;
}


void MainWindow::connectSignals(){
    connect(server,SIGNAL(isStartingFailed(bool)),this,SLOT(serverStartingFailed(bool)));
    connect(server, SIGNAL(newConnection()), this, SLOT(approveConnection()));
    //connect(clientConnection, &QIODevice::readyRead, this, &MainWindow::sendAllOrders); NIE ODKOMENTOWAC
    connect(server,SIGNAL(serverStopped()),this,SLOT(serverStopped()));

    connect(server,SIGNAL(onOrderOperation(int,int)),this,SLOT(changeOrder(int,int)));
    connect(server,SIGNAL(addNewOrder()),this,SLOT(addOrderToList()));
    connect(server,SIGNAL(recoverLastOrder()),this,SLOT(recoverDeletedOrder()));

    connect(this,SIGNAL(approveConnect(QVector<int>,QVector<int>,QVector<QString>,int)),server,SLOT(sendApprove(QVector<int>,QVector<int>,QVector<QString>,int)));

    connect(server,SIGNAL(serverFullScreen()),this,SLOT(on_actionFull_screen_triggered()));
    connect(server,SIGNAL(resetOrders()),this,SLOT(ordersReset()));



}

void MainWindow::changeOrder(int order,int state){

    //TU SIE WYKONUJE PROCEDURA OPERACJI NA LISTACH ZAMOWIEN ( ZMIANA , USUNIECIE)
    qDebug()<<"NUMER ZAMOWIENIA TO"<<order;
    qDebug()<<"STAN ZAMOWIENIA TO"<<state;

    for(int i=0;i<orders.length();i++){
        if(orders.at(i) == order){
            if(state == 4){
                deletedOrders.append(orders.at(i));
                deletedStates.append(ordersState.at(i));
                deletedTimes.append(ordersTime.at(i));

                orders.removeAt(i);
                ordersState.removeAt(i);
                ordersTime.removeAt(i);

                db->deleteOrder_fromDb(order);
            }else{
                ordersState.removeAt(i);
                ordersState.insert(i,state);
                db->updateOrder_inDb(order,state);
            }
        }
    }


    server->sendAllOrders(orders,ordersState,ordersTime,db->getNextNumber_fromDb());
    refreshOrders();



}

void MainWindow::addOrderToList(){

    if(order == 99 || order == 0){
        order = 1;
    }else{
        order++;
    }
    orders.append(order);
    ordersState.append(1);
    ordersTime.append(QTime::currentTime().toString("hh:mm:ss"));
    db->addOrder_toDb(order,1,QTime::currentTime().toString("hh:mm:ss"));
    db->saveNextNumber_toDb(order);

    qDebug()<<"Przed refreshem";
    refreshOrders();
    qDebug()<<"Po refreshu";
    server->sendAllOrders(orders,ordersState,ordersTime,order);
}

void MainWindow::recoverDeletedOrder(){
    if(!deletedOrders.isEmpty()){
        orders.append(deletedOrders.last());
        deletedOrders.removeLast();

        ordersState.append(deletedStates.last());
        deletedStates.removeLast();

        ordersTime.append(deletedTimes.last());
        deletedTimes.removeLast();

        db->addOrder_toDb(orders.last(),ordersState.last(),ordersTime.last());

        sortOrders();
        refreshOrders();
        server->sendAllOrders(orders,ordersState,ordersTime,order);
    }
}



void MainWindow::approveConnection(){
    qDebug()<<"Nowe polaczenie, emituje sygnał";
    emit approveConnect(this->orders,this->ordersState,this->ordersTime,this->order);
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
    if(server->isListening()){
        QMessageBox::information(this,"Blad","Pierw zatrzymaj serwer");
    }else{
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

            db->addIp_port_toDb(server->getIpAdress(),server->getPortNumber());
        }




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

void MainWindow::sortOrders(){
    bool change = true;
    int tempOrd = 0;
    int tempState = 0;
    QString tempTime = "";
    qDebug()<<"Dlugosc ordersów "<<orders.length();
    while(change){
        change = false;
        for(int i=0;i<orders.length()-1;i++){
            qDebug()<<i;
            if(orders.at(i+1) < orders.at(i)){
                change = true;

                tempOrd = orders.at(i);
                tempState = ordersState.at(i);
                tempTime = ordersTime.at(i);

                orders.removeAt(i);
                ordersState.removeAt(i);
                ordersTime.removeAt(i);
                if(orders.at(i) == orders.last()){
                    orders.append(tempOrd);
                    ordersState.append(tempState);
                    ordersTime.append(tempTime);
                }else{
                    orders.insert(i+1,tempOrd);
                    ordersState.insert(i+1,tempState);
                    ordersTime.insert(i+1,tempTime);
                }
            }
        }
    }
}

void MainWindow::refreshOrders(){
    qDebug()<<"Poczatek refresha";
    QStringList ordList;
    int row = 0;
    int col = 0;
    QTableWidgetItem *ordItem;
    qDebug()<<orders.length();
    for(int i=0;i<orders.length();i++){
        ordList.append(QString::number(orders.at(i)));
    }
    ui->ordersTable->clear();
    ui->ordersTable->setColumnCount(5);


    ui->ordersTable->setRowCount(static_cast<int>(ceil(static_cast<double>(ordList.length())/5)));

    qDebug()<<ceil(static_cast<int>(static_cast<double>(ordList.length())/5));
    for(int i=0;i<ordList.length();i++){
        ordItem = new QTableWidgetItem(ordList.at(i));
        ordItem->setTextAlignment(Qt::AlignCenter);
        ordItem->setFont(ordersFont);
        if(ordersState.at(i) == 1)
            ordItem->setBackgroundColor(Qt::yellow);
        if(ordersState.at(i) == 2)
            ordItem->setBackgroundColor(QColor(254,127,0,255));
        if(ordersState.at(i) == 3){
            ordItem->setBackgroundColor(Qt::green);
        }


        row = static_cast<int>(ceil(static_cast<double>(i+1)/5))-1;
        qDebug()<<row;
        col = i % 5;
        qDebug()<<"Dodaje item na pozycji "<<row<<" "<<col;

        ui->ordersTable->setItem(row,col,ordItem);


    }
    custWind->refreshOrdersTables(orders,ordersState);



}

void MainWindow::on_actionFull_screen_triggered()
{
    custWind->setFullScreen();
}

void MainWindow::loadSettingsFromDB(){

    server->setIpAdress(db->readIp_fromDb());
    server->setPortNumber(db->readPort_fromDb());
    order = db->getNextNumber_fromDb();
    qDebug()<<"Ostatnie zamowienie z bazy "<<order;
    qDebug()<<"Dlugosc listy zamowien przed: "<<orders.length();
    qDebug()<<"Dlugosc listy stanow przed: "<<ordersState.length();
    qDebug()<<"Dlugosc listy czasow przed: "<<ordersTime.length();

    for(int i=0;i<db->getTimes_fromDb().length();i++){
        orders.append(db->getOrders_fromDb().at(i));
        ordersState.append(db->getStates_fromDb().at(i));
        ordersTime.append(db->getTimes_fromDb().at(i));
    }
    qDebug()<<"Dlugosc listy zamowien po: "<<orders.length();
    qDebug()<<"Dlugosc listy stanow po: "<<ordersState.length();
    qDebug()<<"Dlugosc listy czasow po: "<<ordersTime.length();
    sortOrders();
    refreshOrders();


}

void MainWindow::ordersReset(){
    for(auto ord:orders){
        db->deleteOrder_fromDb(ord);
    }
    orders.clear();
    ordersState.clear();
    ordersTime.clear();

    deletedOrders.clear();
    deletedTimes.clear();
    deletedStates.clear();

    db->saveNextNumber_toDb(1);
    order = 0;
    refreshOrders();
    server->sendAllOrders(orders,ordersState,ordersTime,order);
}
