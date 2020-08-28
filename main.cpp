#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    a.setWindowIcon(QIcon(":/img/serverIcon.ico"));
    w.setWindowTitle("Orders Server");
    w.show();
    return a.exec();
}
