#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <memory>
#include <vector>
#include <sstream>
#include <QLabel>
#include <cassert>
#include <QLineEdit>
#include <QTextEdit>


#include "ui_mainwindow.h"
#include "searching_worker.h"
#include "dictionary.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() = default;

    void input_changed();


private slots:

    void update_output();

private:
    std::unique_ptr<Ui::MainWindow> ui;
    searching_worker worker;
    QTextEdit *output_label;

    bool updating = false;

    void check_box_state_changed();
    uint64_t cur_output_version = 0;
    bool is_first_word = true;


    std::atomic<bool> is_checked = false;

    std::pair<QString, QString> format_output(const searched_result &result, uint64_t version, bool &to_append, bool &is_ready);

    QString format_output(const searched_result &result, uint64_t version, bool &to_append);
};

#endif // MAIN_WINDOW_H
