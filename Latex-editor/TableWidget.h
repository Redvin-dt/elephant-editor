#ifndef TABLE_WIDGET_H
#define TABLE_WIDGET_H

#include <QMainWindow>
#include <QPushButton>
#include <QWidget>

class TableWidget : public QWidget {
    Q_OBJECT

    QPushButton *create_button;
    QMainWindow *main_window;

    ~TableWidget();
    /**
     * @brief
     * on X button presed
     * @param event
     */
    void closeEvent(QCloseEvent *event);

  public slots:
    void on_create_button_click();

  public:
    /**
     * @brief Construct a new Table Widget object
     *
     * @param main_window
     */
    TableWidget(QMainWindow *main_window);
};

#endif // TABLE_WIDGET_H