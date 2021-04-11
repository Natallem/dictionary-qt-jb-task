#include <QColorDialog>
#include <iostream>
#include "main_window.h"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    QLineEdit &input_line = *(ui->input_edit);
    input_line.setFixedHeight(input_line.fontMetrics().height() * 2);

    output_label = new QTextEdit(ui->scroll_area);
    output_label->setReadOnly(true);
    output_label->setAcceptRichText(true);
    output_label->setAlignment(Qt::AlignLeft);
    ui->scroll_area->setWidget(output_label);
    if (worker.dict.is_open()){
        output_label->setText("Loading dictionary...<br>");
        connect(ui->input_edit, &QLineEdit::textChanged, this, &MainWindow::input_changed);
        connect(ui->check_box, &QCheckBox::clicked, this, &MainWindow::check_box_state_changed);
        connect(&worker, &searching_worker::output_changed, this, &MainWindow::update_output);
    } else {
        QPalette palette;
        palette.setColor(QPalette::Text,Qt::darkRed);
        output_label->setPalette(palette);
        output_label->setText(QString::fromStdString(constants::error_open_file_message));
    }
}

void MainWindow::check_box_state_changed() {
    bool cur_checked = ui->check_box->isChecked();
    if (is_seq_checkbox != cur_checked) {
        is_seq_checkbox = cur_checked;
        input_changed();
    }
}

void MainWindow::input_changed() {
    QString val = ui->input_edit->text();
    ++input_version;
    if (val.isEmpty()) {
        worker.set_input(std::nullopt, is_seq_checkbox, input_version);
    } else {
        worker.set_input(val.toUtf8().constData(), is_seq_checkbox, input_version);
    }
}
void print(const std::string & str){
    std::cout << str << '\n';
    std::cout.flush();
}
void MainWindow::update_output() {
    if (updating)
        return;
    updating = true;
    retry:
    auto[res, input_v, output_v] = worker.get_output();

    if (last_printed_version == output_v) {
        updating = false;
        return;
    } else {
        last_printed_version = output_v;
    }
    if (res.input.empty()) {
        output_label->setText("");
        updating = false;
        return;
    }
    bool input_changed_while_printing = format_output(res, input_v);
    if (input_changed_while_printing || output_v != worker.output_version) {
        goto retry;
    }
    updating = false;
}

bool MainWindow::format_output(const searching_result &result, uint64_t input_v) {
    std::stringstream ss;
    if (cur_output_version != result.input_version) {
        cur_output_version = result.input_version;
        is_first_word = true;
        ss << "<b>" << result.input << "</b>";
        if (result.partial || !result.words.empty())
            ss << " :<br>";
        output_label->setText(QString::fromStdString(ss.str()));
        ss.str(std::string());
    }
    for (size_t i = 0; i != result.words.size(); ++i) {
        if (!is_first_word) {
            ss << ", ";
        } else {
            is_first_word = false;
        }
        ss << result.words[i];
        QApplication::processEvents(QEventLoop::AllEvents);
        if (input_v != input_version)
            return true;
        if (i % 10000 == 0){
            flush_output(ss);
        }
    }
    if (!result.partial) {
        ss << "<br><br>Total occurrences number = <b>" << result.total_occurrences_number << "</b><br>";
    }
    flush_output(ss);
    return false;
}

void MainWindow::flush_output(std::stringstream & ss){
    auto cursorPos = output_label->textCursor().position();
    output_label->moveCursor(QTextCursor::End);
    output_label->insertHtml(QString::fromStdString(ss.str()));
    QTextCursor cursor = output_label->textCursor();
    cursor.setPosition(cursorPos, QTextCursor::MoveAnchor);
    output_label->setTextCursor(cursor);
    ss.str(std::string());
}
