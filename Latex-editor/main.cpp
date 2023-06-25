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
    const int doc_id = 0;

    Authorization w;
    w.show();
    QTimer timer_send;
    auto sendText = [&]() {
        auto [text, type] = w.mainwindow->getText();
        if (!type || text.isEmpty()) {
            json::value getvalue = json::value::array();
            getvalue[0] = json::value::string(std::to_string(doc_id));
            cpprest_server::get_server().make_request(methods::GET, "get-text", getvalue);
            return;
        }
        std::cerr << "send\n";

        json::value putvalue = json::value::array();
        putvalue[0] = json::value::number(doc_id);
        int text_size = static_cast<int>(text.size());
        QString line;
        for (int position_in_text = 0, position_in_array = 1; position_in_text < text_size; position_in_text++) {
            if (text[position_in_text] == '\n') {
                putvalue[position_in_array] = json::value::string(line.toStdString());
                position_in_array++;
                line.clear();
            } else {
                line += text[position_in_text];
            }

            if (position_in_text + 1 == text_size && !line.isEmpty()) {
                putvalue[position_in_array] = json::value::string(line.toStdString());
            }
        }
        cpprest_server::display_json(putvalue, "S");
        json::value response_put = cpprest_server::get_server().make_request(methods::PUT, "put-text", putvalue);
        cpprest_server::display_json(response_put, "R");
        w.mainwindow->replaceText(response_put);

        //TODO think about delay

        json::value getvalue = json::value::array();
        getvalue[0] = json::value::string(std::to_string(doc_id));
        json::value response_get = cpprest_server::get_server().make_request(methods::GET, "get-text", getvalue);
        cpprest_server::display_json(response_get, "R");
        w.mainwindow->replaceText(response_get);
    };
    timer_send.setInterval(5000);
    QObject::connect(&timer_send, &QTimer::timeout, sendText);
    timer_send.start();
    return a.exec();
}
