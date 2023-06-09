#include "TableWidget.h"
#include <QCloseEvent>
#include <QIntValidator>
#include <QLabel>
#include <QStringList>

const int LAYOUT_SPACING = 25;

void TableWidget::closeEvent(QCloseEvent *event) {
    main_window->setEnabled(true);
}

void TableWidget::on_create_button_click() {
    /* TODO */
    main_window->setEnabled(true);
    close();
}

TableWidget::TableWidget(QMainWindow *main_window)
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
      create_button(new QPushButton(this)) {
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

    /* TODO add validator for border and separator */

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