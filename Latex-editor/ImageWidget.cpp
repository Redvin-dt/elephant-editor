#include "ImageWidget.h"
#include <QDebug>
#include <QPainter>
#include <QPoint>
#include <QRect>
#include <QSize>

const QPoint ZERO_POINT = QPoint(0, 0);

ImageWidget::ImageWidget(QWidget *parent) : QWidget{parent} {}

void ImageWidget::loadImage(QString filename) {
    Q_INIT_RESOURCE(codeeditor_resources);
    m_image.load(filename);
    m_duplicate = m_image;
    setMinimumSize(m_image.size());
    this->update();
}

void ImageWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    if (!m_image.isNull()) {
        // reload image to increase quality
        m_image = m_duplicate;
        // scale to width of parent widget
        m_image = m_image.scaledToWidth(this->size().width(),
                                        Qt::SmoothTransformation);
        // m_image = m_image.scaledToHeight(std::max(this->size().height(),
        // m_image.size().height()));
        painter.drawImage(ZERO_POINT, m_image);
    }
}

void ImageWidget::setImage(QImage new_image) {
    m_image = new_image;
    m_duplicate = m_image;
    setMinimumSize(m_image.size());
    this->update();
}
