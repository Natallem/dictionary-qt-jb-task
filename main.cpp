#include "main_window.h"
#include <QApplication>
#include "dictionary_util.h"

int main(int argc, char *argv[])
{
    bool gen = false;
    if (gen){
        dictionary_util::generate_dictionary(constants::input_dictionary_file_name, constants::processed_dictionary_file_name);
    } else {
        QApplication a(argc, argv);
        MainWindow w;
        w.show();
        return a.exec();
    }
}
