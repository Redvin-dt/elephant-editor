#include "../include/ImageInsert.h"
#include "../include/ui_ImageInsert.h"
#include <QMessageBox>
#include <QFile>
#include <QProcess>
#include <poppler-qt5.h>
#include <QSize>
#include <memory>

ImageInsert::ImageInsert(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::vertical_layout),
        image_widget(new ImageWidget(this)) {
    ui->setupUi(this);
    ui->horizontalLayout->addWidget(image_widget);
    image_widget->loadImage(":image_template.png");
    image_widget->setMinimumSize(QSize(378, 460));
}

ImageInsert::~ImageInsert() {
    delete ui;
}

void ImageInsert::on_ScaleButton_clicked(bool checked) {
    if (checked) {
        dim_mode = 1;
        ui->Scale->setReadOnly(false);
        ui->Height->setReadOnly(true);
        ui->Width->setReadOnly(true);
    }
}


void ImageInsert::on_HeightandWidth_clicked(bool checked) {
    if (checked) {
        dim_mode = 2;
        ui->Scale->setReadOnly(true);
        ui->Height->setReadOnly(false);
        ui->Width->setReadOnly(false);
    }
}


void ImageInsert::on_ExactlyInText_clicked(bool checked) {
    if (checked) {
        positioning_mode = 1;
    }
}


void ImageInsert::on_NextgoodPlace_clicked(bool checked) {
    if (checked) {
        positioning_mode = 2;
    }
}


void ImageInsert::on_LeftSide_clicked(bool checked) {
    if (checked) {
        positioning_mode = 3;
    }
}


void ImageInsert::on_RightSide_clicked(bool checked) {
    if (checked) {
        positioning_mode = 4;
    }
}


void ImageInsert::on_BottomButton_clicked(bool checked) {
    if (checked) {
        positioning_mode = 5;
    }
}


void ImageInsert::on_TopButton_clicked(bool checked) {
    if (checked) {
        positioning_mode = 6;
    }
}


bool ImageInsert::field_check() {
    if (dim_mode == 0) {
        QMessageBox::information(this, "Field error", "chose dimensions mode");
    } else if (positioning_mode == 0) {
        QMessageBox::information(this, "Field error", "chose positioning mode");
    } else if (dim_mode == 1 && ui->Scale->text().isEmpty()) {
        QMessageBox::information(this, "Field error", "write scale of image");
    } else if (dim_mode == 2 && (ui->Height->text().isEmpty() || ui->Width->text().isEmpty())) {
        QMessageBox::information(this, "Field error", "write height and width of image");
    } else {
        return 0;
    }
    return 1;
}


void ImageInsert::on_pushButton_clicked() {
    if (field_check()) {
        return;
    }
    picture_code = "";
    picture_code += "\\begin{figure}";
    if (positioning_mode == 1) {
        picture_code += "[H]";
    } else if (positioning_mode == 2) {
        picture_code += "[h]";
    } else if (positioning_mode == 3) {
        picture_code += "{l}";
    } else if (positioning_mode == 4) {
        picture_code += "{r}";
    } else if (positioning_mode == 5) {
        picture_code += "[b]";
    } else if (positioning_mode == 6) {
        picture_code += "[t]";
    }
    picture_code += "\n";
    if (!(ui->lineEdit_5->text().isEmpty())) {
        picture_code += "\\caption{" + ui->lineEdit_5->text() + "}\n";
    }
    picture_code += "\\centering\n";
    if (dim_mode == 2) {
        picture_code += "\\includegraphics[width=" + ui->Width->text() + "cm" + ", height=" + ui->Height->text() + "cm";
        if (!ui->lineEdit_4->text().isEmpty()) {
            picture_code += ", angle=" + ui->lineEdit_4->text();
        }
        picture_code += "]";
        picture_code += "{" + image_name + "}" + '\n';
    } else {
        picture_code += "\\includegraphics[scale=" + ui->Scale->text();
        if (!ui->lineEdit_4->text().isEmpty()) {
            picture_code += ", angle=" + ui->lineEdit_4->text();
        }
        picture_code += "]";
        picture_code += "{" + image_name + "}" + '\n';
    }
    picture_code += "\\end{figure}";
    editor->insertPlainText(picture_code);
}


void ImageInsert::on_pushButton_2_clicked() {
    QFile latex_template("latex_template_imageinsert.tex");
    latex_template.open(QIODevice::WriteOnly | QIODevice::Append);
    if (field_check()) {
        return;
    }
    picture_code = "";
    picture_code += "\\begin{figure}";
    if (positioning_mode == 1) {
        picture_code += "[H]";
    } else if (positioning_mode == 2) {
        picture_code += "[h]";
    } else if (positioning_mode == 3) {
        picture_code += "{l}";
    } else if (positioning_mode == 4) {
        picture_code += "{r}";
    } else if (positioning_mode == 5) {
        picture_code += "[b]";
    } else if (positioning_mode == 6) {
        picture_code += "[t]";
    }
    picture_code += "\n";
    if (!(ui->lineEdit_5->text().isEmpty())) {
        picture_code += "\\caption{" + ui->lineEdit_5->text() + "}\n";
    }
    picture_code += "\\centering\n";
    if (dim_mode == 2) {
        picture_code += "\\includegraphics[width=" + ui->Width->text() + "cm" + ", height=" + ui->Height->text() + "cm";
        if (!ui->lineEdit_4->text().isEmpty()) {
            picture_code += ", angle=" + ui->lineEdit_4->text();
        }
        picture_code += "]";
        picture_code += "{" + image_name + "}" + '\n';
    } else {
        picture_code += "\\includegraphics[scale=" + ui->Scale->text();
        if (!ui->lineEdit_4->text().isEmpty()) {
            picture_code += ", angle=" + ui->lineEdit_4->text();
        }
        picture_code += "]";
        picture_code += "{" + image_name + "}" + '\n';
    }
    picture_code += "\\end{figure}";
    QString latex_code =
            "\\documentclass[a4paper,12pt]{article}\n"
            "\\usepackage{graphicx}\n"
            "\\usepackage{float}\n"
            "\\usepackage[usenames]{color}\n"
            "\\usepackage{colortbl}\n"
            "\\graphicspath{ {" +
            path + "} }\n\\begin{document}\n"
                   " \\textcolor{white}{a}\n"
                   "\\\\\\textcolor{white}{a}\n" + picture_code + "\n" +
            "\\end{document}";
    latex_template.write(latex_code.toStdString().c_str());
    latex_template.close();
    const QString COMPILE_OPTIONS =
            "pdflatex --file-line-error -halt-on-error -interaction=nonstopmode ";
    QProcess compiling(this);
    compiling.start(COMPILE_OPTIONS + "latex_template_imageinsert.tex");
    compiling.waitForFinished(-1);
    QString extrafiles = "latex_template_imageinsert.log";
    QFile::remove(extrafiles);
    extrafiles.replace(".log", ".aux");
    QFile::remove(extrafiles);
    extrafiles.replace(".aux", ".out");
    QFile::remove(extrafiles);
    extrafiles.replace(".out", ".synctex.gz");
    QFile::remove(extrafiles);
    std::unique_ptr<Poppler::Document> document(
            Poppler::Document::load("latex_template_imageinsert.pdf"));
    QFile::remove("latex_template_imageinsert.tex");
    if (!document || document->isLocked()) {
        return;
    }
    std::unique_ptr<Poppler::Page> pdfFirstPage(document->page(0));
    if (pdfFirstPage == 0) {
        return;
    }

    const int X_ZOOM = 55;
    const int Y_ZOOM = 50;
    const int PDF_X = 30;
    const int PDF_Y = 50;
    const int WIDTH = 378;
    const int HEIGHT = 460;

    QImage first_image =
            pdfFirstPage->renderToImage(X_ZOOM, Y_ZOOM, PDF_X, PDF_Y, WIDTH, HEIGHT);
    QFile::remove("latex_template_imageinsert.pdf");
    image_widget->setImage(first_image);
}

