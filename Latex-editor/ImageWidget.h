#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>
#include <QImage>
#include <QPaintEvent>
#include <QString>

class ImageWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ImageWidget(QWidget *parent = nullptr);
    void loadImage(QString filename);

private:
    QImage m_image;
    QImage m_duplicate;

protected:
    void paintEvent(QPaintEvent *event);
};

#endif // IMAGEWIDGET_H
