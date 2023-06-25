#include "Authorization.h"
#include "ui_Authorization.h"
#include "QMessageBox"
Authorization::Authorization(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Authorization)
{
    ui->setupUi(this);
}

Authorization::~Authorization()
{
    delete ui;
}

void Authorization::on_Enter_button_clicked()
{
    QString login = ui->login->text();
    QString password = ui->password->text();
    //add check server and work offline if connection error
    if (login == "admin" && password == "admin"){
        hide();
        mainwindow = new MainWindow();
        mainwindow->show();
    }
    else {
        QMessageBox::information(this, "Authorization error", "everething wrong idi nahui");
    }
}


void Authorization::on_Offline_button_clicked()
{
    hide();
    mainwindow = new MainWindow();
    mainwindow->show();
}
