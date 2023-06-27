#include "CreateDocument.h"
#include "ui_CreateDocument.h"
#include <QDebug>
#include <iostream>
CreateDocument::CreateDocument(QString* d_name, QString* colab, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateDocument)
{
    ui->setupUi(this);
    doc_name = d_name;
    collaborators = colab;
}

CreateDocument::~CreateDocument()
{
    delete ui;
}

void CreateDocument::on_Add_clicked()
{
    if (!ui->Colaborators->text().isEmpty()){
        *collaborators += ui->Colaborators->text() + " ";
        ui->Colaborators->clear();
    }
}

void CreateDocument::on_Create_doc_clicked()
{
    if (!ui->Doc_name->text().isEmpty()){
        on_Add_clicked();
        *doc_name = ui->Doc_name->text();
        hide();
    }
}

