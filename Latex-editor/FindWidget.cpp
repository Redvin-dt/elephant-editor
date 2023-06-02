#include "FindWidget.h"
#include "ui_FindWidget.h"
#include "CodeEditor.h"

#include "SyntaxHighlighter.h"
#include "mainwindow.h"

FindWidget::FindWidget(QMainWindow *parent, CodeEditor *editor) :
        QMainWindow(parent),
        ui(new Ui::FindWidget) {
    ui->setupUi(this);

    search_highlight = new SyntaxHighlighter(editor->document());

    connect(ui->pushButton, &QPushButton::clicked, this, &FindWidget::Search);
}

void FindWidget::Search() {
    search_highlight->find(ui->lineEdit->text());
}

FindWidget::~FindWidget() {
    delete ui;
}
