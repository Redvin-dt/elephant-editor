#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QTimer>
#include <iostream>

#include "mainwindow.h"
#include "Authorization.h"
#include "TableWidget.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale: uiLanguages) {
        const QString baseName = "Latex-editor_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    //TODO auth

    Authorization w;
    w.show();
    return a.exec();
}
