#ifndef IMAGEINSERT_H
#define IMAGEINSERT_H

#include <QDialog>
#include "ImageWidget.h"
#include <QString>
#include "CodeEditor.h"

namespace Ui {
    class vertical_layout;
}

class ImageInsert : public QDialog {
Q_OBJECT

public:
    explicit ImageInsert(QWidget *parent = nullptr);

    ~ImageInsert();

    CodeEditor *editor;

    QString image_name = "";
    QString path = "";

private slots:

    void on_ScaleButton_clicked(bool checked);

    void on_HeightandWidth_clicked(bool checked);

    void on_ExactlyInText_clicked(bool checked);

    void on_NextgoodPlace_clicked(bool checked);

    void on_LeftSide_clicked(bool checked);

    void on_RightSide_clicked(bool checked);

    void on_BottomButton_clicked(bool checked);

    void on_TopButton_clicked(bool checked);

    bool field_check();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::vertical_layout *ui;
    int dim_mode = 0;
    int positioning_mode = 0;
    QString picture_code = "";
    ImageWidget *image_widget;
};

#endif // IMAGEINSERT_H
