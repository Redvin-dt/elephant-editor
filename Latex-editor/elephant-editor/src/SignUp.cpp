#include "../include/SignUp.h"
#include "../include/ui_SignUp.h"
#include <QMessageBox>

SignUp::SignUp(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::SignUp) {
    ui->setupUi(this);
    ui->password1->setEchoMode(QLineEdit::Password);
    ui->password2->setEchoMode(QLineEdit::Password);
}

SignUp::~SignUp() {
    delete ui;
}

void SignUp::on_Enter_button_clicked() {
    QString login = ui->login->text();
    QString password1 = ui->password1->text();
    QString password2 = ui->password2->text();
    if (password1 != password2) {
        QMessageBox::information(this, "Authorization error", "Password mismatch");
        ui->login->clear();
        ui->password1->clear();
        ui->password2->clear();
        return;
    }
    json::value getvalue = json::value::array();
    getvalue[0] = json::value::string(login.toStdString());
    json::value is_exist = cpprest_server::get_server().make_request(methods::GET, "check-user", getvalue);
    if (is_exist == json::value::null()) {
        QMessageBox::information(this, "Authorization error",
                                 "Server is currently offline. Try later or open offline mode");
    } else if (!is_exist.as_bool()) {
        hide();
        getvalue[1] = json::value::string(password1.toStdString());
        cpprest_server::get_server().make_request(methods::POST, "add-user", getvalue);
        mainwindow = new MainWindow(nullptr, true, ui->login->text());
        mainwindow->setOnline();
        mainwindow->show();
    } else {
        QMessageBox::information(this, "Authorization error", "Login already in use");
        ui->login->clear();
        ui->password1->clear();
        ui->password2->clear();
    }
}

