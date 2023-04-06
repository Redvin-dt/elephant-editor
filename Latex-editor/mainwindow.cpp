#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "CodeEditor.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    CodeEditor *Editor = new CodeEditor();
    ui->gridLayout->addWidget(Editor, 0, 1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

