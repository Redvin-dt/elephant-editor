#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QFormLayout>
#include <QHeaderView>
#include <QImage>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QSize>
#include <QTableWidget>
#include <QVector>
#include <QtGui>
#include <cstddef>
#include <memory>
#include <poppler/qt5/poppler-qt5.h>
#include <QMessageBox>
#include <QFileDialog>
#include <unistd.h>

const QSize MINIMAL_WINDOW_SIZE = QSize(1300, 1000);
const QSize MINIMAL_CODE_EDITOR_SIZE = QSize(500, 300);
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
}

void MainWindow::setButton(){

    static QVector <QString> buttons_names = {"int", "frac", "cdot", "sqrt", "textbf"};
    static QVector <QString> buttons_functions = {"\\int", "\\frac{}{}", "\\cdot", "\\sqrt", "\\textbf"};
    static QVector <QPushButton*> buttons;

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

  // initialize buttons
  std::size_t row = 0, column = 0;
  for (std::size_t index = 0; index < buttons_names.size(); index++) {
    QString button_name = buttons_names[index];
    QString function_input = buttons_functions[index];
    buttons.push_back(new QPushButton());

    buttons.back()->setText(button_name);
    connect(buttons.back(), &QPushButton::clicked,
            [this, function_input]() { insertMathInput(function_input); });

    table_view->setCellWidget(row, column, buttons.back());

    column++;
    if (column == MAX_TABLE_COLUMNS) {
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

void MainWindow::insertMathInput(QString insertion) {
  editor->insertPlainText(insertion);
}

void MainWindow::initImage(){
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

  installing.start(INSTALLING_OPTIONS + current_file);
  installing.waitForFinished(-1);

  compiling.start(COMPILE_OPTIONS + current_file);
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
