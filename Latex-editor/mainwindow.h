#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "CodeEditor.h"
#include "ImageWidget.h"
#include <QScrollArea>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionSave_triggered();

    void on_actionSave_As_triggered();

    void on_actionOpen_file_triggered();

    void on_actionRun_triggered();

private:
    Ui::MainWindow *ui;
    CodeEditor *editor;
    QScrollArea *scroll_area;
    ImageWidget *m_image;
    QString current_file = "";
    QStringList compile_errors;
    
    //Paint buttons in tab MathInput
    void setButton();

    //Paint CodeEditor in the left side of mainwindow
    void setCodeEditor();

    //Function for buttons
    void insertMathInput(QString insertion);

    //init start image
    void initImage();

    //print pdf
    void setPDF();
};
#endif // MAINWINDOW_H
