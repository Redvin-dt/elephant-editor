#include "../include/TableWidget.h"
#include "poppler-qt5.h"
#include <QCloseEvent>
#include <QDebug>
#include <QIntValidator>
#include <QLabel>
#include <QStringList>
#include <map>
#include <QProcess>

const int LAYOUT_SPACING = 25;

void TableWidget::closeEvent(QCloseEvent *event) {
    main_window->setEnabled(true);
}

void TableWidget::on_cancel_button_click() {
    main_window->setEnabled(true);
    close();
}

void TableWidget::on_preview_button_click() {
    QFile latex_template("latex_template_imageinsert.tex");
    latex_template.open(QIODevice::WriteOnly | QIODevice::Append);

    static const QString table_open =
            "%Please add following package to your "
            "document:\n%\\usepackage{tabularray}\n\\begin{tblr}";
    static const QString table_close = "\\end{tblr}\n\\\\\n";
    static const std::map<QString, QString> convert_separator = {
            {"line",   "|"},
            {"empty",  ""},
            {"dotted", "|[dotted]"},
            {"dashed", "|[dashed]"}};

    static const std::map<QString, QString> convert_alignment = {
            {"center", "c"},
            {"left",   "l"},
            {"right",  "r"}};

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
    QString latex_code =
            "\\documentclass[a4paper,12pt]{article}\n"
            "\\usepackage{tabularray}\n"
            "\\begin{document}\n"
            + table + "\n" +
            "\\end{document}";
    latex_template.write(latex_code.toStdString().c_str());
    latex_template.close();
    const QString COMPILE_OPTIONS =
            "pdflatex --file-line-error -halt-on-error -interaction=nonstopmode ";
    QProcess compiling(this);
    compiling.start(COMPILE_OPTIONS + "latex_template_imageinsert.tex");
    compiling.waitForFinished(-1);
    QString extrafiles = "latex_template_imageinsert.log";
    QFile::remove(extrafiles);
    extrafiles.replace(".log", ".aux");
    QFile::remove(extrafiles);
    extrafiles.replace(".aux", ".out");
    QFile::remove(extrafiles);
    extrafiles.replace(".out", ".synctex.gz");
    QFile::remove(extrafiles);
    std::unique_ptr<Poppler::Document> document(
            Poppler::Document::load("latex_template_imageinsert.pdf"));
    QFile::remove("latex_template_imageinsert.tex");
    if (!document || document->isLocked()) {
        return;
    }
    std::unique_ptr<Poppler::Page> pdfFirstPage(document->page(0));
    if (pdfFirstPage == 0) {
        return;
    }

    const int X_ZOOM = 55;
    const int Y_ZOOM = 50;
    const int PDF_X = 15;
    const int PDF_Y = 25;
    const int WIDTH = 358;
    const int HEIGHT = 450;

    QImage first_image =
            pdfFirstPage->renderToImage(X_ZOOM, Y_ZOOM, PDF_X, PDF_Y, WIDTH, HEIGHT);
    QFile::remove("latex_template_imageinsert.pdf");
    image_widget->setImage(first_image);
}

void TableWidget::on_create_button_click() {

    static const QString table_open =
            "%Please add following package to your "
            "document:\n%\\usepackage{tabularray}\n\\begin{tblr}";
    static const QString table_close = "\\end{tblr}\n\\\\\n";
    static const std::map<QString, QString> convert_separator = {
            {"line",   "|"},
            {"empty",  ""},
            {"dotted", "|[dotted]"},
            {"dashed", "|[dashed]"}};

    static const std::map<QString, QString> convert_alignment = {
            {"center", "c"},
            {"left",   "l"},
            {"right",  "r"}};

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
        : QWidget(), main_window(main_window), main_layout(new QHBoxLayout), layout(new QGridLayout(this)),
          row_line_edit(new QLineEdit(this)), column_line_edit(new QLineEdit(this)),
          separator_combo_box(new QComboBox(this)),
          border_combo_box(new QComboBox(this)),
          default_text_line_edit(new QLineEdit(this)),
          bold_check_box(new QCheckBox(this)),
          italic_check_box(new QCheckBox(this)),
          underline_check_box(new QCheckBox(this)),
          alignment_combo_box(new QComboBox(this)),
          cancel_button(new QPushButton(this)),
          preview_button(new QPushButton(this)),
          create_button(new QPushButton(this)), insert_function(insert_function_), image_widget(new ImageWidget(this)) {
    main_window->setEnabled(false);

    this->setFixedSize(QSize(900, 500));
    image_widget->loadImage(":image_template.png");
    image_widget->setMinimumSize(QSize(358, 450));
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

    cancel_button->setText("cancel");
    create_button->setText("create");
    preview_button->setText("preview");
    QObject::connect(create_button, &QPushButton::clicked, this,
                     &TableWidget::on_create_button_click);
    QObject::connect(cancel_button, &QPushButton::clicked, this,
                     &TableWidget::on_cancel_button_click);
    QObject::connect(preview_button, &QPushButton::clicked, this,
                     &TableWidget::on_preview_button_click);
    layout->setSpacing(LAYOUT_SPACING);

    // first row
    layout->addWidget(new QLabel("row:", this), 0, 0);
    layout->addWidget(row_line_edit, 0, 1);
    layout->addWidget(new QLabel("column:"), 0, 3);
    layout->addWidget(column_line_edit, 0, 4);
    layout->addWidget(image_widget, 0, 5);
    // second row
    layout->addWidget(new QLabel("cell separator:"), 1, 0);
    layout->addWidget(separator_combo_box, 1, 1);
    layout->addWidget(new QLabel("border:"), 1, 3);
    layout->addWidget(border_combo_box, 1, 4);

    // third row
    layout->addWidget(new QLabel("default text"), 2, 0);
    layout->addWidget(default_text_line_edit, 2, 1);

    // fourth row
    layout->addWidget(new QLabel("Bold:"), 3, 0);
    layout->addWidget(bold_check_box, 3, 1);
    layout->addWidget(new QLabel("Italic:", this), 3, 2);
    layout->addWidget(italic_check_box, 3, 3);
    layout->addWidget(new QLabel("Underline"), 3, 4);
    layout->addWidget(underline_check_box, 3, 5);

    // fifth row
    layout->addWidget(new QLabel("alignment:"), 4, 0);
    layout->addWidget(alignment_combo_box, 4, 1);

    // six row
    layout->addWidget(cancel_button, 5, 0, 1, 2);
    layout->addWidget(create_button, 5, 5, 1, 2);
    layout->addWidget(preview_button, 5, 2, 1, 3);
    this->setLayout(layout);
    this->setWindowTitle("Create Table");
}

TableWidget::~TableWidget() { main_window->setEnabled(true); }
