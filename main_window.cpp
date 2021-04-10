#include <iostream>
#include "main_window.h"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    QLineEdit &input_line = *(ui->input_edit);
    input_line.setFixedHeight(input_line.fontMetrics().height() * 2);

    output_label = new QTextEdit();
    output_label->setReadOnly(true);
    output_label->setAcceptRichText(true);
    output_label->setAlignment(Qt::AlignLeft);
    output_label->setText("Loading dictionary...<br>");
    ui->scroll_area->setWidget(output_label);

    connect(ui->input_edit, &QLineEdit::textChanged, this, &MainWindow::input_changed);
    connect(ui->check_box, &QCheckBox::clicked, this, &MainWindow::check_box_state_changed);
    connect(&worker, &searching_worker::output_changed, this, &MainWindow::update_output);
}

void MainWindow::check_box_state_changed() {
    bool cur_checked = ui->check_box->isChecked();
    if (is_checked != cur_checked){
        is_checked = cur_checked;
        input_changed();
    }
}

void MainWindow::input_changed() {
    QString val = ui->input_edit->text();
    if (val.isEmpty()) {
        worker.set_input(std::nullopt, is_checked);
    } else {
        worker.set_input(val.toUtf8().constData(), is_checked);
    }
}

void print(const std::string &str) {
    std::cout << str << "\n";
    std::cout.flush();
}

void MainWindow::update_output() {
    if (updating)
        return;
    updating = true;
    retry:
    auto result_pair = worker.get_output();
    if (!result_pair.first) {
        output_label->setText("");
        updating = false;
        return;
    }
    QString str = format_output(*result_pair.first, result_pair.second);
    if (str.isEmpty() || result_pair.second != worker.output_version) {
        goto retry;
    } else {
        output_label->setText(str);
    }
    updating = false;
}

QString MainWindow::format_output(const searched_result &result, uint64_t version) {
    std::stringstream ss;
    ss << result.input;
    if (result.partial || !result.words.empty())
        ss << " :<br>";
    for (size_t i = 0; i != result.words.size(); ++i) {
        ss << result.words[i];
        if ((i + 1) != result.words.size() || result.partial)
            ss << ", ";
        if (i % 10 == 0) {
            QApplication::processEvents(QEventLoop::AllEvents);
            if (version != worker.output_version)
                return QString();
        }
    }

    if (result.partial)
        ss << "…<br>";
    else {
        ss << "<br><br>Total occurrences number = <b>" << result.words.size() << "</b><br>";
    }
    return QString::fromStdString(ss.str());
}
