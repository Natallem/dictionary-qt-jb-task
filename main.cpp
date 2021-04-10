#include "main_window.h"
#include <QPushButton>
#include <QApplication>
#include "dictionary.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
