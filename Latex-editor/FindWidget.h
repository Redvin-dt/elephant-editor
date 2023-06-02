#ifndef FINDWIDGET_H
#define FINDWIDGET_H

#include <QWindow>
#include "CodeEditor.h"
#include "mainwindow.h"

namespace Ui {
    class FindWidget;
}

class SyntaxHighlighter;
class CodeEditor;

class FindWidget : public QMainWindow
{
Q_OBJECT

public:
    explicit FindWidget(QMainWindow *window, CodeEditor *editor);
    ~FindWidget();

private:
    Ui::FindWidget *ui;

    SyntaxHighlighter* search_highlight;

    void Search();
};

#endif // FINDWIDGET_H
