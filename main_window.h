#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <memory>
#include <vector>
#include <sstream>
#include <QLabel>
#include <cassert>
#include <QLineEdit>

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

    static QString format_output(searched_result const &result);

private slots:

    void update_output();

private:
    std::unique_ptr<Ui::MainWindow> ui;
    searching_worker worker;
    QLabel *output_label;

    QString format_output(const searched_result &result, uint64_t version);
};

#endif // MAIN_WINDOW_H
