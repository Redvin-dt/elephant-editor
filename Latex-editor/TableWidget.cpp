#include "TableWidget.h"
#include <QCloseEvent>

void TableWidget::closeEvent(QCloseEvent *event) {
    main_window->setEnabled(true);
}

void TableWidget::on_create_button_click() {
    /* TODO */
    main_window->setEnabled(true);
    close();
}

TableWidget::TableWidget(QMainWindow *main_window)
    : QWidget(), create_button(new QPushButton("Close", this)),
      main_window(main_window) {
    main_window->setEnabled(false);
    create_button->show();
    connect(create_button, SIGNAL(clicked()), this,
            SLOT(on_exit_button_click()));
}

TableWidget::~TableWidget() { main_window->setEnabled(true); }