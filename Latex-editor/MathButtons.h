#ifndef LATEX_EDITOR_MATHBUTTONS_H
#define LATEX_EDITOR_MATHBUTTONS_H

#include <QWidget>
#include <QVector>
#include <QPushButton>
#include <QGroupBox>
#include <QString>
#include <QLineEdit>
#include <QTableWidget>
#include <functional>
#include <QBoxLayout>
#include <QStringList>
#include <QGridLayout>

class MathButtons : public QWidget{
    Q_OBJECT

    QVector <QString> buttons_names;
    QVector <QString> buttons_functions;
    QVector <QPushButton*> buttons;
    QVector <std::size_t> function_index;
    QVector <QStringList> description;

    QGroupBox *buttons_group;
    QGridLayout *view;
    QLineEdit *search_box;
    QBoxLayout *layout;

    std::function<void(QString)> button_function;

    void print_matching_buttons(const QString &pattern = "");
    void connect_buttons();
    void init_buttons();

public:

    explicit MathButtons(QWidget *parrent, const std::function<void(QString)> &function) : QWidget(parrent),
                                            //view(new QTableWidget(this)),
                                             buttons_group(new QGroupBox(this)),
                                             view(new QGridLayout()),
                                             search_box(new QLineEdit(this)),
                                             button_function(function),
                                             layout(new QBoxLayout(QBoxLayout::Down))
    {
        layout->addWidget(search_box);
        setLayout(layout);
        init_buttons();
    }
};


#endif //LATEX_EDITOR_MATHBUTTONS_H
