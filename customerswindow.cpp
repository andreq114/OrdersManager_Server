#include "customerswindow.h"
#include "ui_customerswindow.h"

CustomersWindow::CustomersWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CustomersWindow)
{
    ui->setupUi(this);
    startPreferences();
}

CustomersWindow::~CustomersWindow()
{
    delete ui;
    delete titleLabel;
    delete titleLay;
    for(auto table:ordersTables)
        delete table;
    delete tabWidgLay2;
    delete tabWidgLay;
    delete centralLayout;


}

void CustomersWindow::startPreferences(){
    titleFont.setBold(true);
    titleFont.setPointSize(25);
    ordersFont.setBold(true);
    ordersFont.setPointSize(30);


    centralLayout = new QVBoxLayout();
    titleLay = new QHBoxLayout();
    tabWidgLay = new QHBoxLayout();
    tabWidgLay2 = new QVBoxLayout();



    titleLabel = new QLabel();
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setFont(titleFont);
    titleLabel->setMinimumSize(500,200);
    titleLabel->setText("Zamówienia");

    titleLay->addWidget(titleLabel);

    tabWidgLay->addLayout(tabWidgLay2);

    centralLayout->addLayout(titleLay);
    centralLayout->addLayout(tabWidgLay);
    ui->centralwidget->setLayout(centralLayout);

    setTablesNumber();


}

void CustomersWindow::setTablesNumber(){
    for(int i=0;i<10;i++){
        ordersTables.append(new QTableWidget());
        tabWidgLay2->addWidget(ordersTables.at(i));
        ordersTables.at(i)->setVisible(false);
        ordersTables.at(i)->setStyleSheet("background-color:transparent;");
        ordersTables.at(i)->setMinimumSize(400,500);
        ordersTables.at(i)->
    }
    ordersTables.at(0)->setVisible(true);
}

//Sprowac zamiast kilku widgetów zrobić jeden widget table i na nim na watku zmieniac itemy
//Zrobic Itemy na stałe
void CustomersWindow::refreshOrdersTables(QVector<int> order,QVector<int> states){

    splitOrders(order,states);
    ordersTables.at(0)->setRowCount(5);
    ordersTables.at(0)->setColumnCount(2);
    ordersTables.at(0)->setItem(0,0,new QTableWidgetItem("Elo1"));
    ordersTables.at(0)->setItem(1,0,new QTableWidgetItem("Elo2"));
    ordersTables.at(0)->setItem(2,0,new QTableWidgetItem("Elo3"));
    ordersTables.at(0)->setItem(3,0,new QTableWidgetItem("Elo4"));
    ordersTables.at(0)->setItem(4,0,new QTableWidgetItem("Elo5"));

}

void CustomersWindow::splitOrders(QVector<int> orders,QVector<int> states){
    notReadyOrders.clear();
    partReadyOrders.clear();
    fullReadyOrders.clear();
    for(int i=0;i<orders.length();i++){
        if(states.at(i) == 1){
            notReadyOrders.append(orders.at(i));
        }else if(states.at(i) == 2){
            partReadyOrders.append(orders.at(i));
        }else if(states.at(i) == 3){
            fullReadyOrders.append(orders.at(i));
        }
    }
}
