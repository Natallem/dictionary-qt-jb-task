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
#include "constants.h"

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
    uint64_t input_version = constants::INPUT_VERSION_QUIT + 1;
    uint64_t cur_output_version = 0;
    uint64_t last_printed_version = 0;
    bool is_first_word = true;
    bool is_seq_checkbox = false;

    void check_box_state_changed();

    QString format_output(const searching_result &result, uint64_t input_v, uint64_t output_v, bool &to_append);
};

#endif // MAIN_WINDOW_H
