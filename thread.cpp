#include "thread.h"

Thread::Thread(QObject *parent) : QThread(parent)
{

}

void Thread::run()
{
    while(this->isRunning()){
        qDebug()<<"Dzialam";
        emit this->refreshOrders();
        this->msleep(3000);


    }
}
