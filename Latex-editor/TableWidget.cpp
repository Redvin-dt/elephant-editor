#include "TableWidget.h"
#include <QCloseEvent>
#include <QDebug>
#include <QIntValidator>
#include <QLabel>
#include <QStringList>
#include <map>

const int LAYOUT_SPACING = 25;

void TableWidget::closeEvent(QCloseEvent *event) {
    main_window->setEnabled(true);
}

void TableWidget::on_cancel_button_click() {
    main_window->setEnabled(true);
    close();
}

void TableWidget::on_create_button_click() {

    static const QString table_open =
        "%Please add following package to your "
        "document:\n%\\usepackage{tabularray}\n\\begin{tblr}";
    static const QString table_close = "\\end{tblr}\n\\\\\n";
    static const std::map<QString, QString> convert_separator = {
        {"line", "|"},
        {"empty", ""},
        {"dotted", "|[dotted]"},
        {"dashed", "|[dashed]"}};

    static const std::map<QString, QString> convert_alignment = {
        {"center", "c"}, {"left", "l"}, {"right", "r"}};

    QString alignment =
        convert_alignment.at(alignment_combo_box->currentText());
    QString separator =
        convert_separator.at(separator_combo_box->currentText());
    QString border = convert_separator.at(border_combo_box->currentText());
    QString columns_type = "{" + border;
    for (int i = 0; i < column_line_edit->text().toInt(); i++) {
        if (i != 0) {
            columns_type += separator;
        }
        columns_type += alignment;
    }
    columns_type += border + "}";

    if (!separator.isEmpty()) {
        separator.remove(0, 1);
    }

    if (!border.isEmpty()) {
        border.remove(0, 1);
    }

    QString item;

    int closed_bracket_counter = 0;

    if (underline_check_box->isChecked()) {
        item += "\\underline{";
        closed_bracket_counter++;
    }

    if (bold_check_box->isChecked()) {
        item += "\\textbf{";
        closed_bracket_counter++;
    }

    if (italic_check_box->isChecked()) {
        item += "\\textit{";
        closed_bracket_counter++;
    }

    item += default_text_line_edit->text();

    for (int i = 0; i < closed_bracket_counter; i++) {
        item += "}";
    }

    QString line;
    for (int i = 0; i < column_line_edit->text().toInt(); i++) {
        if (i != 0) {
            line += " & ";
        }
        line += item;
    }
    line += "\\\\\n";

    QString table = table_open + columns_type;
    if (!(border_combo_box->currentText() == "empty")) {
        table += "\n\\hline" + border + "\n";
    }
    for (int i = 0; i < row_line_edit->text().toInt(); i++) {
        if (i != 0 && !(separator_combo_box->currentText() == "empty")) {
            table += "\\hline" + separator + "\n";
        }
        table += line;
    }
    if (!(border_combo_box->currentText() == "empty")) {
        table += "\\hline" + border + "\n";
    }
    table += table_close;

    insert_function(table);

    main_window->setEnabled(true);
    close();
}

TableWidget::TableWidget(
    const std::function<void(const QString &)> &insert_function_,
    QMainWindow *main_window)
    : QWidget(), main_window(main_window), layout(new QGridLayout(this)),
      row_line_edit(new QLineEdit(this)), column_line_edit(new QLineEdit(this)),
      separator_combo_box(new QComboBox(this)),
      border_combo_box(new QComboBox(this)),
      default_text_line_edit(new QLineEdit(this)),
      bold_check_box(new QCheckBox(this)),
      italic_check_box(new QCheckBox(this)),
      underline_check_box(new QCheckBox(this)),
      alignment_combo_box(new QComboBox(this)),
      cancel_button(new QPushButton(this)),
      create_button(new QPushButton(this)), insert_function(insert_function_) {
    main_window->setEnabled(false);

    this->setFixedSize(QSize(500, 400));

    QIntValidator *int_validator = new QIntValidator(1, 1000, this);
    row_line_edit->setValidator(int_validator);
    column_line_edit->setValidator(int_validator);

    border_combo_box->addItem("line");
    border_combo_box->addItem("dotted");
    border_combo_box->addItem("dashed");
    border_combo_box->addItem("empty");

    separator_combo_box->addItem("line");
    separator_combo_box->addItem("dotted");
    separator_combo_box->addItem("dashed");
    separator_combo_box->addItem("empty");

    alignment_combo_box->addItem("center");
    alignment_combo_box->addItem("left");
    alignment_combo_box->addItem("right");

    cancel_button->setText("cancle");
    create_button->setText("create");

    QObject::connect(create_button, &QPushButton::clicked, this,
                     &TableWidget::on_create_button_click);

    QObject::connect(cancel_button, &QPushButton::clicked, this,
                     &TableWidget::on_create_button_click);

    layout->setSpacing(LAYOUT_SPACING);

    // first row
    layout->addWidget(new QLabel("row:", this), 0, 0);
    layout->addWidget(row_line_edit, 0, 1, 1, 2);
    layout->addWidget(new QLabel("column:"), 0, 3);
    layout->addWidget(column_line_edit, 0, 4, 1, 2);

    // second row
    layout->addWidget(new QLabel("cell separator:"), 1, 0);
    layout->addWidget(separator_combo_box, 1, 1, 1, 2);
    layout->addWidget(new QLabel("border:"), 1, 3);
    layout->addWidget(border_combo_box, 1, 4, 1, 2);

    // third row
    layout->addWidget(new QLabel("default text"), 2, 0);
    layout->addWidget(default_text_line_edit, 2, 1, 1, 5);

    // fourth row
    layout->addWidget(new QLabel("Bold:"), 3, 0);
    layout->addWidget(bold_check_box, 3, 1);
    layout->addWidget(new QLabel("Italic:", this), 3, 2);
    layout->addWidget(italic_check_box, 3, 3);
    layout->addWidget(new QLabel("Underline"), 3, 4);
    layout->addWidget(underline_check_box, 3, 5);

    // fifth row
    layout->addWidget(new QLabel("alignment:"), 4, 0);
    layout->addWidget(alignment_combo_box, 4, 1, 1, 3);

    // six row
    layout->addWidget(cancel_button, 5, 0, 1, 2);
    layout->addWidget(create_button, 5, 4, 1, 2);

    this->setLayout(layout);
    this->setWindowTitle("Create Table");
}

TableWidget::~TableWidget() { main_window->setEnabled(true); }
