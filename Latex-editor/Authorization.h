#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include <QDialog>
#include "mainwindow.h"
namespace Ui {
class Authorization;
}

class Authorization : public QDialog
{
    Q_OBJECT

public:
    explicit Authorization(QWidget *parent = nullptr);
    ~Authorization();

private slots:
    void on_Enter_button_clicked();

    void on_Offline_button_clicked();

private:
    Ui::Authorization *ui;
    MainWindow *mainwindow;
};

#endif // AUTHORIZATION_H
