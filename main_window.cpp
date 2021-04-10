#include "main_window.h"
#include "ui_mainwindow.h"
#include <cassert>
#include <sstream>
#include <QLineEdit>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QLineEdit & input_line =  *(ui->input_edit);
    input_line.setFixedHeight(input_line.fontMetrics().height() * 2);

    output_label = new QLabel;
    output_label->setWordWrap(true);
    output_label->setAlignment(Qt::AlignLeft);
    output_label ->setTextFormat(Qt::RichText);
    output_label->setText("Loading dictionary...<br>");
    ui->scroll_area->setWidget(output_label);

    connect(ui->input_edit, &QLineEdit::textChanged, this, &MainWindow::input_changed);
    connect(&worker, &searching_worker::output_changed, this, &MainWindow::update_output);
}

void MainWindow::input_changed()
{
    QString val = ui->input_edit->text();
    if (val.isEmpty()){
        worker.set_input(std::nullopt);
    } else {
        worker.set_input(val.toUtf8().constData());
    }
}

QString MainWindow::format_output(searched_result const& result)
{
    std::stringstream ss;
    ss << result.input;
    if (result.partial || !result.words.empty())
        ss << " :<br>";

    for (size_t i = 0; i != result.words.size(); ++i) {
        ss << result.words[i];
        if ((i + 1) != result.words.size() || result.partial)
            ss << ", ";
    }

    if (result.partial)
        ss << "…<br>";
    else {
        ss << "<br><br>Total occurrences number = " << result.words.size() << "<br>";
    }
    return QString::fromStdString(ss.str());
}

void MainWindow::update_output()
{
    std::optional<searched_result> result = worker.get_output();
    if (!result) {
        output_label->setText("");
        return;
    }
    output_label->setText(format_output(*result));
}

MainWindow::~MainWindow() = default;
