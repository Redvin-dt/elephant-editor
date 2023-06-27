#include "../include/MathButtons.h"
#include <QDebug>
#include <QFile>
#include <QScrollArea>

const std::size_t MAX_TABLE_COLUMNS = 4;
const std::size_t BUTTON_NUM = 1435;
const QSize BUTTON_SIZE = QSize(30, 30);
const int COMMAND_COLUMN = 2;
const int NAME_COLUMN = 1;
const int DESCRIPTION_COLUMN = 3;

void MathButtons::init_buttons() {
    Q_INIT_RESOURCE(codeeditor_resources);
    QFile file(":/mathSymbols.txt");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "can not load buttons (file not found)" << '\n';
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList list = line.split("^");
        if (list.size() < 4 || list[DESCRIPTION_COLUMN].size() == 0 ||
            list[DESCRIPTION_COLUMN][0] != '\\') {
            continue;
        }

        if (!list[COMMAND_COLUMN].isEmpty()) {
            buttons_names.append(list[NAME_COLUMN]);
            buttons_functions.append(list[COMMAND_COLUMN]);
            description.append(QStringList(list.back().split(" ")));
        }

        description.back().sort();
        description.back().append(list[COMMAND_COLUMN]);
        description.back().removeDuplicates();
        if (!description.back().isEmpty() && description.back()[0].isEmpty()) {
            description.back().erase(description.back().begin());
        }

        for (auto &item: description.back()) {
            item = item.toLower();
        }
    }

    // initialize buttons
    std::size_t row = 0, column = 0;
    for (std::size_t index = 0; index < buttons_names.size(); index++) {
        QString button_name = buttons_names[index];
        buttons.push_back(new QPushButton(this));
        buttons.back()->setText(button_name);
        buttons.back()->setMinimumSize(BUTTON_SIZE);

        view->addWidget(buttons.back(), row, column);

        column++;
        if (column == MAX_TABLE_COLUMNS) {
            row++;
            column = 0;
        }
    }
    connect_buttons();

    buttons_group->setLayout(view);

    QScrollArea *scroll_area = new QScrollArea(this);
    scroll_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scroll_area->setWidgetResizable(true);

    scroll_area->setWidget(buttons_group);
    layout->addWidget(scroll_area);

    //    qDebug() << buttons.size() << '\n';

    connect(search_box, &QLineEdit::textEdited, this,
            &MathButtons::print_matching_buttons);
}

void MathButtons::connect_buttons() {
    for (std::size_t index = 0; index < buttons.size(); index++) {
        QString function_output = buttons_functions[index];
        connect(
                buttons[index], &QPushButton::clicked,
                [function_output, this]() { button_function(function_output); });
    }
}

void MathButtons::print_matching_buttons(const QString &pattern) {
    QString pattern_lowercase = pattern.toLower();

    while (view->count()) {
        QWidget *widget = view->itemAt(0)->widget();
        if (widget) {
            view->removeWidget(widget);
            delete widget;
        }
    }

    buttons.clear();
    buttons.reserve(BUTTON_NUM);

    std::size_t row = 0, column = 0;
    for (std::size_t index = 0; index < buttons_names.size(); index++) {
        bool is_continued = false;
        if (!pattern_lowercase.isEmpty()) {
            is_continued = true;
            for (const auto &item: description[index]) {
                if (item.contains(pattern_lowercase)) {
                    is_continued = false;
                    break;
                }
            }
        }

        if (is_continued) {
            continue;
        }

        buttons.push_back(new QPushButton(this));
        buttons.back()->setText(buttons_names[index]);
        buttons.back()->setMinimumSize(BUTTON_SIZE);
        connect(buttons.back(), &QPushButton::clicked,
                [function_output = buttons_functions[index], this]() {
                    button_function(function_output);
                });

        view->addWidget(buttons.back(), row, column);

        column++;
        if (column == MAX_TABLE_COLUMNS) {
            row++;
            column = 0;
        }
    }
}