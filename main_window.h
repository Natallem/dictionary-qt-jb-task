#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <memory>
#include <vector>
#include <QLabel>
#include "searching_worker.h"
#include "dictionary.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void input_changed();
    static QString format_output(searched_result const& result);

private slots:
    void update_output();

private:
    std::unique_ptr<Ui::MainWindow> ui;
    searching_worker worker;
    QLabel * output_label;
};

#endif // MAIN_WINDOW_H
