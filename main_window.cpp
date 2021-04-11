#include <QColorDialog>
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
    output_label->setText("Loading dictionary...<br>");
    ui->scroll_area->setWidget(output_label);

    connect(ui->input_edit, &QLineEdit::textChanged, this, &MainWindow::input_changed);
    connect(ui->check_box, &QCheckBox::clicked, this, &MainWindow::check_box_state_changed);
    connect(&worker, &searching_worker::output_changed, this, &MainWindow::update_output);
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
    bool to_append = false;
    QString str = format_output(res, input_v, output_v, to_append);
    if (str.isEmpty()) {
        goto retry;
    } else {
        if (to_append) {
            auto cursorPos = output_label->textCursor().position();
            output_label->moveCursor(QTextCursor::End);
            output_label->insertHtml(str);
            QTextCursor cursor = output_label->textCursor();
            cursor.setPosition(cursorPos, QTextCursor::MoveAnchor);
            output_label->setTextCursor(cursor);
        } else {
            output_label->setText(str);
        }
        if (output_v != worker.output_version) {
            goto retry;
        }
    }
    updating = false;
}

QString MainWindow::format_output(const searching_result &result, uint64_t input_v, uint64_t output_v, bool &to_append) {
    std::stringstream ss;
    if (cur_output_version != result.input_version) {
        cur_output_version = result.input_version;
        is_first_word = true;
        ss << "<b>" << result.input << "</b>";
        if (result.partial || !result.words.empty())
            ss << " :<br>";
    } else {
        to_append = true;
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
            return QString();
    }
    if (!result.partial) {
        ss << "<br><br>Total occurrences number = <b>" << result.total_occurrences_number << "</b><br>";
    }
    return QString::fromStdString(ss.str());
}
