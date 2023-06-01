#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QImage>
#include <QPaintEvent>
#include <QString>
#include <QWidget>

class ImageWidget : public QWidget {
    Q_OBJECT
  public:
    explicit ImageWidget(QWidget *parent = nullptr);

    // load image with current filename
    void loadImage(QString filename);

    // set image from Qimage
    void setImage(QImage new_image);

  private:
    QImage m_image;
    QImage m_duplicate;

  protected:
    // Repaint picture when window resized
    void paintEvent(QPaintEvent *event);
};

#endif // IMAGEWIDGET_H
