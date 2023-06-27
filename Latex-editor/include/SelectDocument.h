#ifndef SELECT_DOCUMENT_H
#define SELECT_DOCUMENT_H

#include <QGridLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QStringList>
#include <QWidget>
#include <vector>

class QMainWindow;

class SelectDocument : public QWidget {
Q_OBJECT

    QGridLayout *table_view;
    QMainWindow *main_window;
    QGroupBox *group;
    std::vector<std::vector<QPushButton *>> buttons;

public:
    void closeEvent(QCloseEvent *event);

    SelectDocument(const QStringList &list, QMainWindow *main_window_,
                   QWidget *parrent = nullptr);

    template<typename F>
    void connect_button(std::size_t row, std::size_t column, F function) {
        connect(buttons[row][column], &QPushButton::clicked, function);
    }

    template<typename F>
    void connect_button(std::size_t index, F function) {
        const static std::size_t COLUMN_COUNT = 3;

        connect_button(index / COLUMN_COUNT, index % COLUMN_COUNT, [function = function, select_window = this]() {
            function();
            select_window->close();
        });
    }

    ~SelectDocument();
};

#endif // SELECT_DOCUMENT_H
