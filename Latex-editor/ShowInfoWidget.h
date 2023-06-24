#ifndef SHOWINFOWIDGET_H
#define SHOWINFOWIDGET_H

#include <QWidget>

namespace Ui {
class ShowInfoWidget;
}

class ShowInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ShowInfoWidget(QWidget *parent = nullptr);
    ~ShowInfoWidget();

private:
    Ui::ShowInfoWidget *ui;
};

#endif // SHOWINFOWIDGET_H
