#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "CodeEditor.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    CodeEditor *editor = new CodeEditor();
    // ui->gridLayout->addWidget(editor, 0, 1);
    ui->ViewAndCode->addWidget(editor);
    QWidget* tab_widget = ui->ViewAndCode->widget(0);
    ui->ViewAndCode->replaceWidget(0, editor);
    ui->ViewAndCode->addWidget(tab_widget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

