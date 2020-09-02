#include "thread.h"

Thread::Thread(QObject *parent) : QThread(parent)
{

}

void Thread::run()
{
    while(this->isRunning()){

        emit this->refreshOrders(true);
        this->msleep(3000);                             //Zmiana ekranu widocznego dla klientów co 4 sekundy

    }
}
