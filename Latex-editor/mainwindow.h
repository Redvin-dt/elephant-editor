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

private slots:

    void on_actionOpen_file_triggered();

    void on_actionSaveAs_triggered();

    void on_actionExit_triggered();


    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

    void on_actionCut_triggered();

    void on_actionUndo_triggered();

    void on_actionRedo_triggered();

    void on_actionBuild_triggered();

    void on_actionRUn_triggered();

    void on_actionBlank_Project_triggered();

    void on_actionExampleCPP_Project_triggered();

private:
    Ui::MainWindow *ui;

    CodeEditor *Editor;

    QString current_file = "";
};
#endif // MAINWINDOW_H
