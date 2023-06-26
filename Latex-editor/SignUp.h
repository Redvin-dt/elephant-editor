#ifndef SIGNUP_H
#define SIGNUP_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
    class SignUp;
}

class SignUp : public QDialog {
Q_OBJECT

public:

    MainWindow *mainwindow = nullptr;

    explicit SignUp(QWidget *parent = nullptr);

    ~SignUp();

private slots:

    void on_Enter_button_clicked();

private:
    Ui::SignUp *ui;
};

#endif // SIGNUP_H
