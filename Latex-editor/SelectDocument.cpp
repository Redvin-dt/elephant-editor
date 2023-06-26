#include "SelectDocument.h"
#include <QBoxLayout>
#include <QDebug>
#include <QHeaderView>
#include <QMainWindow>
#include <QScrollArea>
#include <QSize>
#include <QTableWidget>
#include <algorithm>

const std::size_t COLUMN_COUNT = 3;
const QSize WINDOW_SIZE = QSize(600, 400);

SelectDocument::SelectDocument(const QStringList &list,
                               QMainWindow *main_window_, QWidget *parrent)
    : QWidget(parrent), table_view(new QGridLayout(this)),
      main_window(main_window_), group(new QGroupBox(this)) {
    main_window->setEnabled(false);
    buttons.resize((list.size() + COLUMN_COUNT - 1) / COLUMN_COUNT);

    {
        std::size_t index = 0;
        for (const auto &item : list) {

            std::size_t buttons_on_line = std::min(
                3UL, list.size() - index / COLUMN_COUNT * COLUMN_COUNT);

            const std::size_t COLUMN_SPAN = 6;

            buttons[index / COLUMN_COUNT].emplace_back(
                new QPushButton(item, this));

            table_view->addWidget(
                buttons[index / COLUMN_COUNT].back(), index / COLUMN_COUNT,
                index % COLUMN_COUNT * COLUMN_SPAN / buttons_on_line, 1,
                COLUMN_SPAN / buttons_on_line, Qt::AlignTop);
            index++;
        }
    }

    group->setLayout(table_view);

    QScrollArea *scroll_area = new QScrollArea(this);
    scroll_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scroll_area->setWidgetResizable(true);
    scroll_area->setWidget(group);

    QBoxLayout *layout = new QBoxLayout(QBoxLayout::Direction::BottomToTop);
    layout->addWidget(scroll_area);

    setLayout(layout);
    setMaximumHeight(
        std::min(group->sizeHint().height() + 20, WINDOW_SIZE.height()));
    setMaximumWidth(WINDOW_SIZE.width());
    setWindowTitle("SelectDocument");
}

void SelectDocument::closeEvent(QCloseEvent *event) {
    main_window->setEnabled(true);
}

SelectDocument::~SelectDocument() { main_window->setEnabled(true); }