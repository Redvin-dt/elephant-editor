#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "CodeEditor.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    CodeEditor *editor;

    //Paint buttons in tab MathInput
    void setButton();
    //Paint CodeEditor in the left side of mainwindow
    void setCodeEditor();
    //Function for buttons
    void insertMathInput(QString insertion);
};
#endif // MAINWINDOW_H
