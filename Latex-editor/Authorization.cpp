#include "Authorization.h"
#include "ui_Authorization.h"
#include "SignUp.h"
#include "QMessageBox"

Authorization::Authorization(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::Authorization) {
    ui->setupUi(this);
    ui->password->setEchoMode(QLineEdit::Password);
}

Authorization::~Authorization() {
    delete ui;
}

void Authorization::on_Enter_button_clicked() {
    QString login = ui->login->text();
    QString password = ui->password->text();
    json::value getvalue = json::value::array();
    getvalue[0] = json::value::string(login.toStdString());
    getvalue[1] = json::value::string(password.toStdString());
    json::value is_exist = cpprest_server::get_server().make_request(methods::GET, "check-user", getvalue);
    std::cerr << is_exist << '\n';
    if (is_exist.as_bool()) {
        hide();
        mainwindow = new MainWindow(nullptr, true);
        mainwindow->setOnline();
        mainwindow->show();
    } else {
        QMessageBox::information(this, "Authorization error", "Wrong login or password");
        ui->login->clear();
        ui->password->clear();
    }
}


void Authorization::on_Offline_button_clicked() {
    hide();
    mainwindow = new MainWindow(nullptr, false);
    mainwindow->setOffline();
    mainwindow->show();
}


void Authorization::on_SignUp_button_clicked() {
    auto *w = new SignUp();
    w->show();
    this->hide();
}

