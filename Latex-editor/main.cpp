#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QTimer>
#include <iostream>

#include "mainwindow.h"

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
    const int doc_id = 0;

    MainWindow w;
    w.show();

    QTimer timer_send;
    auto sendText = [&]() {
        auto [text, type] = w.getText();
        if (!type || text.isEmpty()) {
            json::value getvalue = json::value::array();
            getvalue[0] = json::value::string(std::to_string(doc_id));
            cpprest_server::get_server().make_request(methods::GET, "get-text", getvalue);
            return;
        }
        std::cerr << "send\n";

        json::value putvalue = json::value::array();
        putvalue[0] = json::value::string(std::to_string(doc_id));
        //TODO split string
        cpprest_server::display_json(putvalue, "S");
        json::value response_put = cpprest_server::get_server().make_request(methods::PUT, "put-text", putvalue);
        //TODO replace string
        cpprest_server::display_json(response_put, "R");


        //TODO think about delay


        json::value getvalue = json::value::array();
        getvalue[0] = json::value::string(std::to_string(doc_id));
        json::value response_get = cpprest_server::get_server().make_request(methods::GET, "get-text", getvalue);
        //TODO replace string
        cpprest_server::display_json(response_get, "R");
    };
    timer_send.setInterval(5000);
    QObject::connect(&timer_send, &QTimer::timeout, sendText);
    timer_send.start();

    return a.exec();
}
