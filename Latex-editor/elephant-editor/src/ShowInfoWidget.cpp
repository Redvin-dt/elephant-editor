#include "../include/ShowInfoWidget.h"
#include "../include/ui_ShowInfoWidget.h"

ShowInfoWidget::ShowInfoWidget(QWidget *parent) :
        QWidget(parent),
        ui(new Ui::ShowInfoWidget) {
    ui->setupUi(this);
}

ShowInfoWidget::~ShowInfoWidget() {
    delete ui;
}
