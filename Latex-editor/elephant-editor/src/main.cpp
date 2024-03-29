#include "../include/Authorization.h"
#include "../include/TableWidget.h"
#include "../include/mainwindow.h"
#include <QApplication>
#include <QLocale>
#include <QTimer>
#include <QTranslator>
#include <iostream>

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

    Authorization w;
    w.show();

    return a.exec();
}
