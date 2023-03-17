#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "CodeEditor.h"
#include <QDebug>
#include <QObject>
#include <QFileDialog>
#include <QMessageBox>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Editor = new CodeEditor();
    ui->gridLayout->addWidget(Editor, 0, 1);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionOpen_file_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open file");
    QFile file(filename);
    qDebug() << filename << '\n';
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot open file : " + file.errorString());
        return;
    }

    current_file = filename;
    int size = (int)current_file.size();
    if (size < 5 || current_file[size - 4] != '.' || current_file[size - 3] != 't'
            || current_file[size - 2] != 'e' || current_file[size - 1] != 'x') {
        QMessageBox::warning(this, "Warning", "File extension is not '.tex'");
        return;
    } // как минимум a.tex

    setWindowTitle(filename);
    QTextStream in(&file);
    QString text = in.readAll();
    Editor->setText(text);
    file.close();
}

void MainWindow::on_actionSaveAs_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, "Save as");
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot save file : " + file.errorString());
        return;
    }

    current_file = filename;
    setWindowTitle(filename);
    QTextStream out(&file);
    QString text = Editor->toPlainText();
    out << text;
    file.close();
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionCopy_triggered()
{
    Editor->copy();
}

void MainWindow::on_actionPaste_triggered()
{
    Editor->paste();
}


void MainWindow::on_actionCut_triggered()
{
    Editor->cut();
}

void MainWindow::on_actionUndo_triggered()
{
    Editor->undo();
}

void MainWindow::on_actionRedo_triggered()
{
    Editor->redo();
}

void MainWindow::on_actionBuild_triggered()
{

}

void MainWindow::on_actionRUn_triggered()
{

}


void MainWindow::on_actionBlank_Project_triggered()
{
    QString filename = "/home/roman/PROJECT_CPP/elephant-editor/Latex-editor/resources/blank.tex";
    QFile file(filename);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot open file : " + file.errorString());
        return;
    }

    setWindowTitle("blank.tex");
    QTextStream in(&file);
    QString text = in.readAll();
    Editor->setText(text);
    file.close();
}


void MainWindow::on_actionExampleCPP_Project_triggered()
{
    QString filename = "/home/roman/PROJECT_CPP/elephant-editor/Latex-editor/resources/cpp.tex";
    QFile file(filename);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot open file : " + file.errorString());
        return;
    }

    setWindowTitle("cpp.tex");
    QTextStream in(&file);
    QString text = in.readAll();
    Editor->setText(text);
    file.close();
}
