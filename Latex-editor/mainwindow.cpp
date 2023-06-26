#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "Authorization.h"
#include "FindWidget.h"
#include "SelectDocument.h"
#include "Server.h"
#include "SyntaxHighlighter.h"
#include "TableWidget.h"
#include <QApplication>
#include <QDebug>
#include <QFileDialog>
#include <QFormLayout>
#include <QHeaderView>
#include <QIcon>
#include <QImage>
#include <QInputDialog>
#include <QLocale>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QSize>
#include <QTableWidget>
#include <QTextStream>
#include <QTimer>
#include <QTranslator>
#include <QVector>
#include <QtGui>
#include <cstddef>
#include <functional>
#include <iostream>
#include <memory>
#include <poppler/qt5/poppler-qt5.h>
#include <unistd.h>
#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QTimer>
#include <iostream>
#include "./ui_mainwindow.h"
#include "Server.h"
#include "SyntaxHighlighter.h"
#include "Authorization.h"
#include "CreateDocument.h"
const QSize MINIMAL_WINDOW_SIZE = QSize(1000, 500);
const QSize MINIMAL_CODE_EDITOR_SIZE = QSize(500, 300);
const QString START_IMAGE_FILENAME = ":/start_project_pdf.pdf";

MainWindow::MainWindow(QWidget *parent, bool isOnline, QString login)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    Q_INIT_RESOURCE(codeeditor_resources);
    ui->setupUi(this);
    setCodeEditor();
    setMinimumSize(MINIMAL_WINDOW_SIZE);
    initImage();
    initErrorMessage();
    initButtons();

    QIcon icon(":/3069224.png");
    setWindowTitle("Elephant editor");
    setWindowIcon(icon);
    syntax_highlighter = new SyntaxHighlighter(editor->document());
    isClientOnline = isOnline;
    const int doc_id = 0;
    auto sendText = [&]() {
        auto [text, type] = getText();
        if (!type || text.isEmpty()) {
            json::value getvalue = json::value::array();
            getvalue[0] = json::value::string(std::to_string(doc_id));
            cpprest_server::get_server().make_request(methods::GET, "get-text",
                                                      getvalue);
            return;
        }
        json::value putvalue = json::value::array();
        putvalue[0] = json::value::number(doc_id);
        int text_size = static_cast<int>(text.size());
        QString line;
        for (int position_in_text = 0, position_in_array = 1;
             position_in_text < text_size; position_in_text++) {
            if (text[position_in_text] == '\n') {
                putvalue[position_in_array] =
                    json::value::string(line.toStdString());
                position_in_array++;
                line.clear();
            } else {
                line += text[position_in_text];
            }

            if (position_in_text + 1 == text_size && !line.isEmpty()) {
                putvalue[position_in_array] =
                    json::value::string(line.toStdString());
            }
        }
        cpprest_server::display_json(putvalue, "S");
        json::value response_put = cpprest_server::get_server().make_request(
            methods::PUT, "put-text", putvalue);
        cpprest_server::display_json(response_put, "R");
        if (response_put != json::value::null()) {
            isClientOnline = true;
            replaceText(response_put);
        } else {
            isClientOnline = false;
        }

        // TODO think about delay

        json::value getvalue = json::value::array();
        getvalue[0] = json::value::string(std::to_string(doc_id));
        json::value response_get = cpprest_server::get_server().make_request(
            methods::GET, "get-text", getvalue);
        cpprest_server::display_json(response_get, "R");
        if (response_put != json::value::null()) {
            replaceText(response_put);
        } else {
            // offline
        }
    };
    user_login = login;
    auto client_status = [&]() {
        QString status_bar;
        if (isClientOnline){
            status_bar += "Network connection : online, User : " + user_login + ", Document " + current_doc;
        }
        else {
            status_bar += "Network connection : offline";
        }
        status_bar += ", ";
        if (!current_file.isEmpty()){
            status_bar += "Current file : " + current_file;
        }
        else {
            status_bar += "No file selected";
        }
        status_bar += ".";
        ui->InfoLine->setText(status_bar);
        ui->InfoLine->setReadOnly(true);
    };
    client_status();
    status_check.setInterval(5000);
    QObject::connect(&status_check, &QTimer::timeout, client_status);
    status_check.start();
    if (isClientOnline) {
        timer_send.setInterval(5000);
        QObject::connect(&timer_send, &QTimer::timeout, sendText);
        timer_send.start();
    }
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

    QImage first_image = pdfFirstPage->renderToImage(X_ZOOM, Y_ZOOM, PDF_X,
                                                     PDF_Y, WIDTH, HEIGHT);
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
    int size = (int)current_file.size();
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
    if (compile_errors.size()) {
        QTextCursor c = editor->textCursor();
        c.movePosition(QTextCursor::Start);
        int err_line = 0;
        for (auto line : compile_errors) {
            if (line.indexOf(".tex") != -1) {
                QStringList err_place = line.split(":");
                err_line = std::stoi(err_place[1].toStdString());
                break;
            }
        }
        qDebug() << err_line << "\n";
        c.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor,
                       err_line - 1);
        c.select(QTextCursor::LineUnderCursor);
        editor->setTextCursor(c);
        editor->setStyleSheet("selection-background-color: red");
        error_message->setText("Compile Error\n" + compile_errors.join('\n'));
    } else {

        error_message->setText("Compiled successfully\n");
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

void MainWindow::on_actionExit_triggered() { QApplication::quit(); }

void MainWindow::on_actionPaste_Table_triggered() {
    TableWidget *table_window = new TableWidget(
        [this](const QString &input) { insertMathInput(input); }, this);
    table_window->show();
}

void MainWindow::on_actionCopy_triggered() { editor->copy(); }

void MainWindow::on_actionPaste_triggered() { editor->paste(); }

void MainWindow::on_actionCut_triggered() { editor->cut(); }

void MainWindow::on_actionUndo_triggered() { editor->undo(); }

void MainWindow::on_actionRedo_triggered() { editor->redo(); }

void MainWindow::on_actionFind_triggered() {
    auto *w = new FindWidget(this, editor);
    w->show();
}

void MainWindow::on_actionFind_and_Replace_triggered() {
    bool ok_find;
    QString find = QInputDialog::getText(
        0, "Find and Replace", "Find:", QLineEdit::Normal, "", &ok_find);
    bool ok_replace;
    QString replace = QInputDialog::getText(
        0, "Find and Replace", "Replace:", QLineEdit::Normal, "", &ok_replace);

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

std::pair<QString, bool> MainWindow::getText() {
    if (editor->document()->isModified()) {
        editor->document()->setModified(false);
        return {editor->toPlainText(), 1};
    }
    return {"", 0};
}

void MainWindow::replaceText(json::value &response) {
    QString new_text = "";
    int response_size = static_cast<int>(response.size());
    for (int position_in_json = 1; position_in_json < response_size;
         position_in_json++) {
        if (!response[position_in_json].is_string()) {
            std::cerr << "WTF??? "
                      << "in get-text i = " << position_in_json
                      << " isnt string" << std::endl;
        }
        new_text +=
            QString::fromStdString(response[position_in_json].as_string());
    }
    // std::cerr << new_text.toStdString() << std::endl;
    editor->setText(new_text);
}

void MainWindow::on_actionShow_user_info_triggered() {
    // взять из бд
}

void MainWindow::on_actionChange_documet_triggered() {}

void MainWindow::on_actionInsert_Image_triggered() {
    window = new ImageInsert();
    QString file_path = QFileDialog::getOpenFileName(this, "Open Image");
    QStringList folders = file_path.split("/");
    window->image_name = folders.back();
    file_path.clear();
    for (int i = 0; i + 1 < folders.size(); i++) {
        if (i) {
            file_path += "/";
        }
        file_path += folders[i];
    }
    window->path = file_path;
    window->editor = editor;
    window->show();
    QString txt = editor->toPlainText();
    for (int i = 0; i < txt.size() - 16; i++) {
        if (txt.mid(i, 16) == "\\begin{document}") {
            txt.insert(i, "\\graphicspath{ {" + file_path + "} }\n");
            break;
        }
    }
    editor->setPlainText(txt);
}

void MainWindow::setOnline() { isClientOnline = true; }

void MainWindow::setOffline() { isClientOnline = false; }

bool MainWindow::getStatus() { return isClientOnline; }

void MainWindow::on_actionLog_Out_triggered() {
    auth->show();
    timer_send.stop();
    status_check.stop();
    hide();
}

void MainWindow::on_actionSelect_document_triggered() {
    if (isClientOnline){
        //get list of access docs from db and make it into QStringList
        QStringList list = {"asdasd", "dasdasd"};
        SelectDocument *select = new SelectDocument(list, this);

        select->show();

        QString *clicked_button = new QString("");

        for (std::size_t index = 0; index < list.size(); index++) {
            select->connect_button(
                index, [clicked_button = clicked_button, name = list[index]]() {
                    *clicked_button = name;
                });
        }
        //change doc_id
        current_doc = *clicked_button;
    }
    else {
        QMessageBox::information(this, "Network error",
                                 "Connect to server to select document");

    }
}

void MainWindow::on_actionCreate_new_document_triggered(){
    if (isClientOnline){
        QString *collabs;
        QString *doc_name;
        collabs = new QString();
        doc_name = new QString();
        CreateDocument *create_window = new CreateDocument(doc_name, collabs);
        create_window->show();
        current_doc = *doc_name;
        //change doc_id
        //send new doc to db and update users access
    }
    else {
        QMessageBox::information(this, "Network error",
                             "Connect to server to create new document");
    }
}
