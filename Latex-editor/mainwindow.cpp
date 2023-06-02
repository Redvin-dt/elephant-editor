#include "mainwindow.h"
#include "FindWidget.h"
#include <poppler/qt5/poppler-qt5.h>
#include <unistd.h>
#include <QDebug>
#include <QFileDialog>
#include <QFormLayout>
#include <QHeaderView>
#include <QImage>
#include <QInputDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QSize>
#include <QTableWidget>
#include <QVector>
#include <QtGui>
#include <cstddef>
#include <memory>
#include <QScrollArea>
#include <QTextStream>
#include <QVector>
#include <functional>

#include "./ui_mainwindow.h"
#include "SyntaxHighlighter.h"

const QSize MINIMAL_WINDOW_SIZE = QSize(1000, 500);
const QSize MINIMAL_CODE_EDITOR_SIZE = QSize(500, 300);
const QString START_IMAGE_FILENAME = ":/start_project_pdf.pdf";

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    setCodeEditor();
    setMinimumSize(MINIMAL_WINDOW_SIZE);
    initImage();
    initErrorMessage();
    initButtons();
    syntax_highlighter = new SyntaxHighlighter(editor->document());
}

void MainWindow::initButtons() {
    std::function<void(QString)> func = [this](const QString &input) {
        insertMathInput(input);
    };
    buttons = new MathButtons(this, func);
    ui->RightWindow->addTab(buttons, "MathInput");
}

void MainWindow::setCodeEditor() {
    editor = new CodeEditor();
    editor->setMinimumSize(MINIMAL_CODE_EDITOR_SIZE);
    ui->ViewAndCode->setChildrenCollapsible(false);
    ui->ViewAndCode->insertWidget(0, editor);
}

void MainWindow::insertMathInput(const QString &insertion) {
    editor->insertPlainText(insertion);
}

void MainWindow::initImage() {
    Q_INIT_RESOURCE(codeeditor_resources);
    scroll_area = new QScrollArea(this);
    scroll_area->setWidgetResizable(true);

    m_image = new ImageWidget(this);
    m_image->loadImage(":/photo_2022-10-09_23-28-33.jpg");
    scroll_area->setWidget(m_image);

    ui->RightWindow->insertTab(0, scroll_area, "PDF-View");
}

void MainWindow::setPDF() {
    QString filename = current_file;
    filename.replace(".tex", ".pdf");
    QStringList dirs = filename.split("/");
    filename = dirs.back();

    if (current_file.isEmpty()) {
        return;
    }
    std::unique_ptr<Poppler::Document> document(
            Poppler::Document::load(filename));
    if (!document || document->isLocked()) {
        return;
    }
    std::unique_ptr<Poppler::Page> pdfFirstPage(document->page(0));
    if (pdfFirstPage == 0) {
        return;
    }

    const int X_ZOOM = 150;
    const int Y_ZOOM = 150;
    const int PDF_X = 0;
    const int PDF_Y = 0;
    const int WIDTH = 1260;
    const int HEIGHT = 1800;

    QImage first_image =
            pdfFirstPage->renderToImage(X_ZOOM, Y_ZOOM, PDF_X, PDF_Y, WIDTH, HEIGHT);
    if (first_image.isNull()) {
        return;
    }

    for (int page_num = 1; page_num < document->numPages(); page_num++) {
        std::unique_ptr<Poppler::Page> pdfPage(document->page(page_num));
        QImage image =
                pdfPage->renderToImage(X_ZOOM, Y_ZOOM, PDF_X, PDF_Y, WIDTH, HEIGHT);

        QImage image_sum(qMax(first_image.width(), image.width()),
                         first_image.height() + image.height(),
                         first_image.format());
        image_sum.fill(0);

        QPainter painter(&image_sum);
        painter.drawImage(PDF_X, PDF_Y, first_image);
        painter.drawImage(PDF_X, first_image.height(), image);
        painter.end();

        first_image = image_sum;
    }

    m_image->setImage(first_image);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_actionSave_As_triggered() {
    QString filename = QFileDialog::getSaveFileName(this, "Save as");
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Warning",
                             "Cannot save file : " + file.errorString());
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

void MainWindow::on_actionSave_triggered() {
    QString filename;
    if (current_file.isEmpty()) {
        filename = QFileDialog::getSaveFileName(this, "Save as");
    } else {
        filename = current_file;
    }
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Warning",
                             "Cannot save file : " + file.errorString());
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

void MainWindow::on_actionOpen_file_triggered() {
    QString filename = QFileDialog::getOpenFileName(this, "Open file");
    QFile file(filename);
    qDebug() << filename << '\n';
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Warning",
                             "Cannot open file : " + file.errorString());
        return;
    }

    current_file = filename;
    int size = (int) current_file.size();
    if (size < 5 || current_file[size - 4] != '.' ||
        current_file[size - 3] != 't' || current_file[size - 2] != 'e' ||
        current_file[size - 1] != 'x') {
        QMessageBox::warning(this, "Warning", "File extension is not '.tex'");
        return;
    } // как минимум a.tex

    setWindowTitle(filename);
    QTextStream in(&file);
    QString text = in.readAll();
    editor->setText(text);
    file.close();
}

void MainWindow::on_actionRun_triggered() {
    on_actionSave_triggered();
    compile_errors.clear();
    const QString COMPILE_OPTIONS =
            "pdflatex --file-line-error -halt-on-error -interaction=nonstopmode ";
    const QString INSTALLING_OPTIONS =
            "texliveonfly --arguments=\"--file-line-error -halt-on-error "
            "-interaction=nonstopmode\" ";
    QProcess installing(this);
    QProcess compiling(this);

    installing.start(INSTALLING_OPTIONS + "\"" + current_file + "\"");
    installing.waitForFinished(-1);

    compiling.start(COMPILE_OPTIONS + "\"" + current_file + "\"");
    compiling.waitForFinished(-1);

    QString extrafiles = current_file;
    QStringList dirs = extrafiles.split("/");
    extrafiles = dirs.back();
    extrafiles.replace(".tex", ".log");

    QFile file_log(extrafiles);
    if (file_log.open(QIODevice::ReadOnly)) {
        QTextStream in(&file_log);
        QString line_block;
        while (!in.atEnd()) {
            QString line = in.readLine();
            if (line.isEmpty()) {
                if (line_block.indexOf("LaTeX Error:") != -1 ||
                    line_block.indexOf("Emergency stop.") != -1) {
                    compile_errors.append(line_block);
                }
                line_block.clear();
            } else {
                line_block += line + "\n";
            }
        }
        if (!line_block.isEmpty()) {
            if (line_block.indexOf("LaTeX Error:") != -1 ||
                line_block.indexOf("Emergency stop.") != -1) {
                compile_errors.append(line_block);
            }
            line_block.clear();
        }
    }

    QFile::remove(extrafiles);
    extrafiles.replace(".log", ".aux");
    QFile::remove(extrafiles);
    extrafiles.replace(".aux", ".out");
    QFile::remove(extrafiles);
    extrafiles.replace(".out", ".synctex.gz");
    QFile::remove(extrafiles);

    setPDF();
}


void MainWindow::on_actionExit_triggered() {
    QApplication::quit();
}

void MainWindow::on_actionCopy_triggered() {
    editor->copy();
}

void MainWindow::on_actionPaste_triggered() {
    editor->paste();
}

void MainWindow::on_actionCut_triggered() {
    editor->cut();
}

void MainWindow::on_actionUndo_triggered() {
    editor->undo();
}

void MainWindow::on_actionRedo_triggered() {
    editor->redo();
}

void MainWindow::on_actionFind_triggered() {
    FindWidget *w = new FindWidget(this, editor);
    w->show();
}

void MainWindow::on_actionFind_and_Replace_triggered() {
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

    while (editor->find(find)) {
        editor->textCursor().insertText(replace);
    }
}


void MainWindow::initErrorMessage() {
    QGridLayout *layout = new QGridLayout(ui->Messages);
    error_message = new CodeEditor(ui->Messages);
    layout->addWidget(error_message);
    ui->Messages->setLayout(layout);
    error_message->setReadOnly(true);
}
