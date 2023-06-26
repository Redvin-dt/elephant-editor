#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Authorization.h"
#include "CodeEditor.h"
#include "ImageInsert.h"
#include "ImageWidget.h"
#include "MathButtons.h"
#include "QTimer"
#include "Server.h"
#include <QMainWindow>
#include <QScrollArea>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class SyntaxHighlighter;
class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    MainWindow(QWidget *parent = nullptr, bool isOnline = false);

    ~MainWindow();

    // return current editor text;
    std::pair<QString, bool> getText();

    Authorization *auth;
    // edit current editor text;
    void replaceText(json::value &response);

    void setOnline();
    void setOffline();
    bool getStatus();
  private slots:

    void on_actionSave_triggered();

    void on_actionPaste_Table_triggered();

    void on_actionSave_As_triggered();

    void on_actionOpen_file_triggered();

    void on_actionRun_triggered();

    void on_actionExit_triggered();

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

    void on_actionCut_triggered();

    void on_actionUndo_triggered();

    void on_actionRedo_triggered();

    void on_actionFind_triggered();

    void on_actionFind_and_Replace_triggered();

    void on_actionShow_user_info_triggered();

    void on_actionChange_documet_triggered();

    void on_actionInsert_Image_triggered();

    void on_actionLog_Out_triggered();

    void on_actionSelect_document_triggered();

  protected:
    SyntaxHighlighter *syntax_highlighter;

  private:
    Ui::MainWindow *ui;
    CodeEditor *editor;
    QScrollArea *scroll_area;
    ImageWidget *m_image;
    QString current_file = "";
    QStringList compile_errors;
    CodeEditor *error_message;
    MathButtons *buttons;
    ImageInsert *window;
    bool isClientOnline = false;
    QTimer timer_send;

    // Paint buttons in tab MathInput
    void initButtons();

    // Paint CodeEditor in the left side of mainwindow
    void setCodeEditor();

    // Function for buttons
    void insertMathInput(const QString &insertion);

    // initialize start image
    void initImage();

    // print pdf
    void setPDF();

    // initialize Field for showing errors
    void initErrorMessage();
};

#endif // MAINWINDOW_H
