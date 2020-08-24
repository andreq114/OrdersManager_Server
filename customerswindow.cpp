#include "customerswindow.h"
#include "ui_customerswindow.h"

CustomersWindow::CustomersWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CustomersWindow)
{
    ui->setupUi(this);
    myThread = new Thread(this);
    myThread->start();

    connectSignals();
    startPreferences();
    //allOrders.append(1);
    //allOrders.append(2);
    //allOrders.append(3);
    //allOrders.append(4);
}

CustomersWindow::~CustomersWindow()
{
    delete ui;
    delete titleLabel;
    delete titleLay;


    delete ordersWidget;

    delete tabWidgLay2;
    delete tabWidgLay;
    delete centralLayout;

    if(myThread->isRunning()){
        myThread->terminate();
        myThread->wait();
    }
    delete myThread;

}

void CustomersWindow::connectSignals(){
    connect(myThread,SIGNAL(refreshOrders()),this,SLOT(showNext()));
}

void CustomersWindow::startPreferences(){

    //this->setWindowFlag(Qt::WindowStaysOnTopHint);



    setWindowFlags(Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);


    titleFont.setBold(true);
    titleFont.setPointSize(40);
    descrFont.setBold(true);
    descrFont.setPointSize(35);
    ordersFont.setBold(true);
    ordersFont.setPointSize(50);

    ordersWidget = new QTableWidget();
    ordersWidget->setStyleSheet("background-color:transparent;");

    centralLayout = new QVBoxLayout();
    titleLay = new QHBoxLayout();
    tabWidgLay = new QHBoxLayout();
    tabWidgLay2 = new QVBoxLayout();




    titleLabel = new QLabel();
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setFont(titleFont);

    titleLabel->setText("Zamówienia");

    titleLay->addWidget(titleLabel);

    tabWidgLay->addLayout(tabWidgLay2);

    centralLayout->addLayout(titleLay);
    centralLayout->addLayout(tabWidgLay);
    tabWidgLay2->addWidget(ordersWidget);

    ui->centralwidget->setLayout(centralLayout);




}


void CustomersWindow::refreshOrdersTables(QVector<int> order,QVector<int> states){
    splitOrders(order,states);
    actualOrdersPage = 1;
    maxOrdersPage =static_cast<int>(ceil(static_cast<double>(order.length())/5));
    myThread->quit();          //Nie mam pewnosci czy zadziala
    showNext();
    myThread->start();



}

void CustomersWindow::splitOrders(QVector<int> orders,QVector<int> states){
    notReadyOrders.clear();
    partReadyOrders.clear();
    fullReadyOrders.clear();
    allOrders.clear();
    allStates.clear();
    for(int i=0;i<orders.length();i++){
        if(states.at(i) == 1){
            notReadyOrders.append(orders.at(i));
        }else if(states.at(i) == 2){
            partReadyOrders.append(orders.at(i));
        }else if(states.at(i) == 3){
            fullReadyOrders.append(orders.at(i));
        }
    }
    for(int i=0;i<fullReadyOrders.length();i++){
        allOrders.append(fullReadyOrders.at(i));
        allStates.append(3);
    }
    for(int i=0;i<partReadyOrders.length();i++){
        allOrders.append(partReadyOrders.at(i));
        allStates.append(2);
    }
    for(int i=0;i<notReadyOrders.length();i++){
        allOrders.append(notReadyOrders.at(i));
        allStates.append(1);
    }
}


void CustomersWindow::showNext(){
    qDebug()<<"Druga strona";

    delete ordersWidget;
    ordersWidget = new QTableWidget();

    tabWidgLay2->addWidget(ordersWidget);
    ordersWidget->setStyleSheet("background-color:transparent;");
    ordersWidget->setSelectionMode(QTableWidget::NoSelection);


    ordersWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);


    ordersWidget->horizontalHeader()->setVisible(false);
    ordersWidget->verticalHeader()->setVisible(false);



    if(actualOrdersPage == 1){
        dispTable(0,5);
    }
    else if(actualOrdersPage == 2){
        dispTable(5,10);
    }
    else if(actualOrdersPage == 3){
        dispTable(10,15);
    }
    else if(actualOrdersPage == 4){
        dispTable(15,20);
    }
    else if(actualOrdersPage == 5){
        dispTable(20,25);
    }
    else if(actualOrdersPage == 6){
        dispTable(25,30);
    }
    else if(actualOrdersPage == 7){
        dispTable(30,35);
    }
    else if(actualOrdersPage == 8){
        dispTable(35,40);
    }
    else if(actualOrdersPage == 9){
        dispTable(40,45);
    }
    else if(actualOrdersPage == 10){
        dispTable(45,50);
    }
}

void CustomersWindow::dispTable(int a,int b){
    int b_temp;
    if(allOrders.length() < b){
        b_temp = b-(b-allOrders.length());
    }else{
        b_temp = b;
    }

    ordersWidget->setRowCount(b_temp-a);
    ordersWidget->setColumnCount(2);
    for(int i=a;i<b_temp;i++){
        ordersWidget->setRowHeight(i-a,175);
        qDebug()<<"Wykonuje sie";
        ordItem = new QTableWidgetItem();
        descItem = new QTableWidgetItem();


        ordItem->setText(QString::number(allOrders.at(i)));
        ordItem->setFont(ordersFont);
        ordItem->setTextAlignment(Qt::AlignCenter);

        descItem->setFont(descrFont);
        descItem->setTextAlignment(Qt::AlignCenter);

        if(allStates.at(i) == 1){
            ordItem->setBackgroundColor(Qt::yellow);
            descItem->setBackgroundColor(Qt::yellow);
            descItem->setText("W trakcie realizacji");
        }else if(allStates.at(i) == 2){
            ordItem->setBackgroundColor(Qt::green);
            descItem->setBackgroundColor(Qt::green);
            descItem->setText("Częściowo do odbioru");
        }else if(allStates.at(i) == 3){
            ordItem->setBackgroundColor(Qt::green);
            descItem->setBackgroundColor(Qt::green);
            descItem->setText("Gotowe do odbioru");
        }

        ordersWidget->setItem(i-a,0,ordItem);
        ordersWidget->setItem(i-a,1,descItem);

    }
    if(actualOrdersPage == maxOrdersPage)
        actualOrdersPage = 1;
    else
        actualOrdersPage++;
}

void CustomersWindow::setFullScreen(){
    if(this->isFullScreen()){
        this->setWindowState(Qt::WindowMaximized);
    }else{
        this->setWindowState(Qt::WindowFullScreen);
    }
}
