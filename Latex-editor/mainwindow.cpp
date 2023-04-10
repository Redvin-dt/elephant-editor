#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPushButton>
#include <QVector>
#include <cstddef>
#include <QScrollArea>
#include <QTableWidget>
#include <QHeaderView>
#include <QScrollArea>
#include <QImage>
#include <QDebug>
#include <QFormLayout>
#include <QtGui>
#include <QSize>
#include <poppler/qt5/poppler-qt5.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    
    setCodeEditor();
    setButton();
    setMinimumSize(1300, 1000);
    initImage();
}

void MainWindow::setButton(){

    static QVector <QString> buttons_names = {"int", "frac", "cdot", "sqrt", "textbf"};
    static QVector <QString> buttons_functions = {"\\int", "\\frac{}{}", "\\cdot", "\\sqrt", "\\textbf"};
    static QVector <QPushButton*> buttons;

    //create table for buttons
    QTableWidget *table_view = new QTableWidget(this);
    const std::size_t max_columns = 4;
    table_view->setRowCount(1 + buttons_names.size() / max_columns + (buttons.size() % max_columns != 0));
    table_view->setColumnCount(max_columns);
    table_view->horizontalHeader()->hide();
    table_view->verticalHeader()->hide();
    table_view->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //initialize buttons
    std::size_t row = 0, column = 0;
    for (std::size_t index = 0; index < buttons_names.size(); index++){
        QString button_name = buttons_names[index];
        QString function_input = buttons_functions[index];
        buttons.push_back(new QPushButton());

        buttons.back()->setText(button_name);
        connect(buttons.back(), &QPushButton::clicked, [this, function_input](){ insertMathInput(function_input);});

        table_view->setCellWidget(row, column, buttons.back());

        column++;
        if (column == max_columns){
            row++;
            column = 0;
        }
    } 


    ui->RightWindow->addTab(table_view, "MathInput");

}

void MainWindow::setCodeEditor(){
    editor = new CodeEditor();
    ui->ViewAndCode->insertWidget(0, editor);
}

void MainWindow::insertMathInput(QString insertion){
    editor->insertPlainText(insertion);
}

void MainWindow::initImage(){
    //TODO
    Q_INIT_RESOURCE(codeeditor_resources);
    scroll_area = new QScrollArea(this);
    scroll_area->setWidgetResizable(true);

    m_image = new ImageWidget(this);
    m_image->loadImage(":/photo_2022-10-09_23-28-33.jpg");
    scroll_area->setWidget(m_image);

    ui->RightWindow->insertTab(0, scroll_area, "PDF-View");
}

void MainWindow::setPDF(){

    m_image->loadImage(/* add your image */)
}

MainWindow::~MainWindow()
{
    delete ui;
}

