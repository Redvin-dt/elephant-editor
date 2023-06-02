/********************************************************************************
** Form generated from reading UI file 'FindWidget.ui'
**
** Created by: Qt User Interface Compiler version 5.15.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FINDWIDGET_H
#define UI_FINDWIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FindWidget
{
public:
    QLineEdit *lineEdit;
    QPushButton *pushButton;

    void setupUi(QWidget *FindWidget)
    {
        if (FindWidget->objectName().isEmpty())
            FindWidget->setObjectName(QString::fromUtf8("FindWidget"));
        FindWidget->resize(442, 53);
        lineEdit = new QLineEdit(FindWidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(10, 10, 331, 25));
        pushButton = new QPushButton(FindWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(350, 10, 80, 25));

        retranslateUi(FindWidget);

        QMetaObject::connectSlotsByName(FindWidget);
    } // setupUi

    void retranslateUi(QWidget *FindWidget)
    {
        FindWidget->setWindowTitle(QCoreApplication::translate("FindWidget", "Form", nullptr));
        pushButton->setText(QCoreApplication::translate("FindWidget", "Search", nullptr));
    } // retranslateUi

};

namespace Ui {
    class FindWidget: public Ui_FindWidget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FINDWIDGET_H
