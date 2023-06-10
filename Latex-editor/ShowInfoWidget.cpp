#include "ShowInfoWidget.h"
#include "ui_ShowInfoWidget.h"

ShowInfoWidget::ShowInfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShowInfoWidget)
{
    ui->setupUi(this);
}

ShowInfoWidget::~ShowInfoWidget()
{
    delete ui;
}
