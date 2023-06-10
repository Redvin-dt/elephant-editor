/********************************************************************************
** Form generated from reading UI file 'ShowInfoWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SHOWINFOWIDGET_H
#define UI_SHOWINFOWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ShowInfoWidget
{
public:
    QTextBrowser *textBrowser;

    void setupUi(QWidget *ShowInfoWidget)
    {
        if (ShowInfoWidget->objectName().isEmpty())
            ShowInfoWidget->setObjectName(QString::fromUtf8("ShowInfoWidget"));
        ShowInfoWidget->resize(400, 300);
        textBrowser = new QTextBrowser(ShowInfoWidget);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setGeometry(QRect(20, 20, 361, 261));

        retranslateUi(ShowInfoWidget);

        QMetaObject::connectSlotsByName(ShowInfoWidget);
    } // setupUi

    void retranslateUi(QWidget *ShowInfoWidget)
    {
        ShowInfoWidget->setWindowTitle(QCoreApplication::translate("ShowInfoWidget", "Form", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ShowInfoWidget: public Ui_ShowInfoWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SHOWINFOWIDGET_H
