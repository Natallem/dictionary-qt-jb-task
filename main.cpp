#include "main_window.h"
#include <QPushButton>
#include <QApplication>
#include <iostream>
#include "dictionary_util.h"
int main(int argc, char *argv[])
{
//    std::string pattern = "od";
//    std::string str = "helloodfios";
//    str.insert(4, "hello");
//    dictionary_util::sub_string_search(std::move(str), pattern,dictionary_util::p_array(pattern));
//    std::cout << dictionary_util::sub_string_search(std::move(str), pattern,dictionary_util::p_array(pattern));
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
