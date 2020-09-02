#ifndef THREAD_H
#define THREAD_H

#include <QThread>
#include <QDebug>

class Thread : public QThread
{
    Q_OBJECT
public:
    explicit Thread(QObject *parent = nullptr);

private:
    void run();
signals:
    void refreshOrders(bool setNextPagSig);

};

#endif // THREAD_H
