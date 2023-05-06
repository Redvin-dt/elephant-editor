#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPushButton>
#include <QVector>
#include <cstddef>
#include <QScrollArea>
#include <QTableWidget>
#include <QHeaderView>
#include <QTextStream>
#include <memory>
#include <QScrollArea>
#include <QImage>
#include <QDebug>
#include <QFormLayout>
#include <QtGui>
#include <QSize>
#include <poppler/qt5/poppler-qt5.h>
#include <QMessageBox>
#include <QFileDialog>
#include <unistd.h>
#include <QInputDialog>
#include "SyntaxHighlighter.h"

const QSize MINIMAL_WINDOW_SIZE = QSize(1300, 1000);
const QSize MINIMAL_CODE_EDITOR_SIZE = QSize(500, 300);
const QString START_IMAGE_FILENAME = ":/start_project_pdf.pdf";
const std::size_t MAX_TABLE_COLUMNS = 4;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setCodeEditor();
    setButton();
    setMinimumSize(MINIMAL_WINDOW_SIZE);
    initImage();
    initErrorMessage();
    syntax_highlighter = new SyntaxHighlighter(editor->document());
}

void MainWindow::setButton(){
    Q_INIT_RESOURCE(codeeditor_resources);
    QFile file(":/mathSymbols.txt");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "can not load buttons (file not found)" << '\n';
        return;
    }

    static QVector <QString> buttons_names;
    static QVector <QString> buttons_functions;
    static QVector <QPushButton*> buttons;

    QTextStream in(&file);
    while (!in.atEnd()){
        QString line = in.readLine();
        QStringList list = line.split("^");
        if (list.size() < 4|| list[3].size() == 0 || list[3][0] != '\\'){
            continue;
        }

        buttons_names.append(list[1]);
        buttons_functions.append(list[3]);
    }

    /* static QVector <QString> buttons_names = {"int", "frac", "cdot", "sqrt", "textbf"};
    static QVector <QString> buttons_functions = {"\\int", "\\frac{}{}", "\\cdot", "\\sqrt", "\\textbf"};
    static QVector <QPushButton*> buttons; */

    //create table for buttons
    QTableWidget *table_view = new QTableWidget(this);
    table_view->setRowCount(1 + buttons_names.size() / MAX_TABLE_COLUMNS + (buttons.size() % MAX_TABLE_COLUMNS != 0));
    table_view->setColumnCount(MAX_TABLE_COLUMNS);
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
        if (column == MAX_TABLE_COLUMNS){
            row++;
            column = 0;
        }
    }


    ui->RightWindow->addTab(table_view, "MathInput");

}

void MainWindow::setCodeEditor(){
    editor = new CodeEditor();
    editor->setMinimumSize(MINIMAL_CODE_EDITOR_SIZE);
    ui->ViewAndCode->setChildrenCollapsible(false);
    ui->ViewAndCode->insertWidget(0, editor);
}

void MainWindow::insertMathInput(QString insertion){
    editor->insertPlainText(insertion);
}

void MainWindow::initImage(){
    Q_INIT_RESOURCE(codeeditor_resources);
    scroll_area = new QScrollArea(this);
    scroll_area->setWidgetResizable(true);
    Poppler::Document* document = Poppler::Document::load("../Latex-editor/resources/start_project_pdf.pdf");
    Poppler::Page* pdfFirstPage = document->page(0);
    QImage first_image = pdfFirstPage->renderToImage(150, 150, 0, 0, 1260, 1450);
    m_image = new ImageWidget(this);
    m_image->setImage(first_image);
    scroll_area->setWidget(m_image);
    ui->RightWindow->insertTab(0, scroll_area, "PDF-View");
    delete pdfFirstPage;
    delete document;
}

void MainWindow::setPDF(){
    QString filename = current_file;
    filename.replace(".tex", ".pdf");
    QStringList dirs = filename.split("/");
    filename = dirs.back();
    if (current_file.isEmpty()){
        return;
    }
    Poppler::Document* document = Poppler::Document::load(filename);
    if (!document || document->isLocked()) {
        delete document;
        return;
    }
    if (document == 0) {
        delete document;
        return;
    }
    Poppler::Page* pdfFirstPage = document->page(0);
    if (pdfFirstPage == 0) {
        delete pdfFirstPage;
        delete document;
        return;
    }
    QImage first_image = pdfFirstPage->renderToImage(150, 150, 0, 0, 1260, 1800);
    if (first_image.isNull()) {
        delete pdfFirstPage;
        delete document;
        return;
    }
    for (int page_num = 1; page_num < document->numPages(); page_num++){
        Poppler::Page* pdfPage = document->page(page_num);
        QImage image = pdfPage->renderToImage(150, 150, 0, 0, 1260, 1800);
        QImage image_sum( qMax( first_image.width(), image.width() ), first_image.height() + image.height(), first_image.format() );
        image_sum.fill( 0 );
        QPainter p( &image_sum );
        p.drawImage( 0, 0, first_image );
        p.drawImage( 0, first_image.height(), image );
        p.end();
        first_image = image_sum;
    }
    m_image->setImage(first_image);
    delete pdfFirstPage;
    delete document;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionSave_As_triggered()
{
    QString filename = QFileDialog::getSaveFileName(this, "Save as");
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot save file : " + file.errorString());
        return;
    }
    qDebug() << filename << '\n';
    current_file = filename;
    setWindowTitle(filename);
    QTextStream out(&file);
    QString text = editor->toPlainText();
    out << text;
    file.close();
}


void MainWindow::on_actionSave_triggered()
{
    QString filename;
    if (current_file.isEmpty()){
        filename = QFileDialog::getSaveFileName(this, "Save as");
    }
    else {
        filename = current_file;
    }
    QFile file(filename);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot save file : " + file.errorString());
        return;
    }
    qDebug() << filename << '\n';
    current_file = filename;
    setWindowTitle(filename);
    QTextStream out(&file);
    QString text = editor->toPlainText();
    out << text;
    file.close();
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
    editor->setText(text);
    file.close();
}


void MainWindow::on_actionRun_triggered()
{
    on_actionSave_triggered();
    compile_errors.clear();
    QProcess compiling(this);
    QString compile_options = "pdflatex --file-line-error -halt-on-error -interaction=nonstopmode ";
    QString installing_comm = "texliveonfly --arguments=\"--file-line-error -halt-on-error -interaction=nonstopmode\" ";
    QProcess installing(this);
    installing.start(installing_comm + current_file);
    installing.waitForFinished(-1);
    qDebug() << current_file;
    compiling.start(compile_options + current_file);
    compiling.waitForFinished(-1);
    QString extrafiles = current_file;
    QStringList dirs = extrafiles.split("/");
    extrafiles = dirs.back();
    extrafiles.replace(".tex", ".log");
    QFile file_log(extrafiles);
    if (file_log.open(QIODevice::ReadOnly)){
        QTextStream in(&file_log);
        QString line_block;
        while(!in.atEnd()){
            QString line = in.readLine();
            if (line.isEmpty()){
                if (line_block.indexOf("LaTeX Error:") != -1 || line_block.indexOf("Emergency stop.") != -1){
                    compile_errors.append(line_block);
                }
                line_block.clear();
            }
            else {
                line_block += line + "\n";
            }
        }
        if (!line_block.isEmpty()){
            if (line_block.indexOf("LaTeX Error:") != -1){
                compile_errors.append(line_block);
            }
            line_block.clear();
        }
    }
    if (compile_errors.isEmpty()){
        setPDF();
    }
    //QFile::remove(extrafiles);
    extrafiles.replace(".log", ".aux");
    QFile::remove(extrafiles);
    extrafiles.replace(".aux", ".out");
    QFile::remove(extrafiles);
    extrafiles.replace(".out", ".synctex.gz");
    QFile::remove(extrafiles);
    if (compile_errors.isEmpty()){
        compile_errors.append("Compilation completed successfully");
    }
    error_message->setText(compile_errors.join("\n"));
}


void MainWindow::on_actionExit_triggered()
{
    QApplication::quit();
}

void MainWindow::on_actionCopy_triggered()
{
    editor->copy();
}

void MainWindow::on_actionPaste_triggered()
{
    editor->paste();
}

void MainWindow::on_actionCut_triggered()
{
    editor->cut();
}

void MainWindow::on_actionUndo_triggered()
{
    editor->undo();
}

void MainWindow::on_actionRedo_triggered()
{
    editor->redo();
}


void MainWindow::on_actionFind_triggered()
{

}


void MainWindow::on_actionFind_and_Replace_triggered()
{
    bool ok_find;
    QString find = QInputDialog::getText(0, "Find and Replace",
                                             "Find:", QLineEdit::Normal,
                                             "", &ok_find);
    bool ok_replace;
    QString replace = QInputDialog::getText(0, "Find and Replace",
                                             "Replace:", QLineEdit::Normal,
                                             "", &ok_replace);

    if (!ok_replace || !ok_find) {
        return;
    }

    editor->moveCursor(QTextCursor::Start);

    while(editor->find(find)){
        editor->textCursor().insertText(replace);
    }
}

void MainWindow::initErrorMessage(){
    QGridLayout *layout = new QGridLayout(ui->Messages);
    error_message = new CodeEditor(ui->Messages);
    layout->addWidget(error_message);
    ui->Messages->setLayout(layout);
    error_message->setReadOnly(true);
}

