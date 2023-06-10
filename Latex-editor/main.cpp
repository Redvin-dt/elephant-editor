#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "TableWidget.h"
#include "mainwindow.h"

#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <iostream>

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

#define TRACE(msg) cout << msg
#define TRACE_ACTION(a, k, v) cout << a << " (" << k << ", " << v << ")\n"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "Latex-editor_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    http_listener listener("http://127.0.0.1:23939");

    MainWindow w;
    w.show();

    std::cerr << "ok\n";
    return a.exec();
}
