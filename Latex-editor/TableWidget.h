#ifndef TABLE_WIDGET_H
#define TABLE_WIDGET_H

#include <QCheckBox>
#include <QComboBox>
#include <QGridLayout>
#include <QLineEdit>
#include <QMainWindow>
#include <QPushButton>
#include <QString>
#include <QWidget>
#include <functional>

class TableWidget : public QWidget {
    Q_OBJECT

    QMainWindow *main_window;
    QGridLayout *layout;
    QLineEdit *row_line_edit, *column_line_edit;
    QComboBox *separator_combo_box, *border_combo_box;
    QLineEdit *default_text_line_edit;
    QCheckBox *bold_check_box, *italic_check_box, *underline_check_box;
    QComboBox *alignment_combo_box;
    QPushButton *cancel_button, *create_button;

    std::function<void(const QString &)> insert_function;

    /**
     * @brief
     * on X button presed
     * @param event
     */
    void closeEvent(QCloseEvent *event);

  public slots:
    /**
     * @brief Construct table
     *
     */
    void on_create_button_click();
    /**
     * @brief Cancle table construct
     *
     */
    void on_cancel_button_click();

  public:
    /**
     * @brief Construct a new Table Widget object
     *
     * @param main_window
     */
    TableWidget(const std::function<void(const QString &)> &insert_function_,
                QMainWindow *main_window = nullptr);

    ~TableWidget();
};

#endif // TABLE_WIDGET_H