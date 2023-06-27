#include "../include/data_base.h"
#include <cpprest/http_client.h>
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <iostream>
#include <string>
#include <vector>

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;
using namespace web::http::client;

#define TRACE(msg) std::cout << msg
#define TRACE_ACTION(a, k, v) std::cout << a << " (" << k << ", " << v << ")\n"

mongodb::DataBase &get_db() {
    static mongodb::DataBase db = mongodb::DataBase();
    return db;
}

void display_json(json::value const &jvalue, utility::string_t const &prefix) {
    std::cout << prefix << ' ' << jvalue.serialize() << std::endl;
}

void handle_get(http_request request) {
    TRACE("\nhandle GET\n");
    // processing GET-request
    auto uri = request.absolute_uri();
    auto path = uri.path();
    json::value response;
    request.extract_json()
            .then([&response](
                    pplx::task<json::value>
                    task) { // extracts the request content into a json
                response = task.get();
            })
            .wait();
    if (path == "/get-text") {
        const std::string doc_name = response.as_array()[0].as_string();
        const std::vector<std::string> lines = get_db().get_document_lines(doc_name);
        json::value answer = json::value::array();
        for (int i = 0; i < lines.size(); i++) {
            answer[i] = json::value::string(lines[i]);
        }
        request.reply(status_codes::OK, answer);
    } else if (path == "/check-user") {
        const std::string login = response.as_array()[0].as_string();
        if (response.as_array().size() == 2) {
            const std::string password = response.as_array()[1].as_string();
            json::value answer = json::value::boolean(get_db().check_is_user_exist(login, password));
            request.reply(status_codes::OK, answer);
        } else {
            json::value answer = json::value::boolean(get_db().check_is_user_exist(login));
            request.reply(status_codes::OK, answer);
        }
    } else if (path == "/get-documents") {
        const std::string login = response.as_array()[0].as_string();
        auto user_info = get_db().get_user(login);
        auto it = std::next(user_info.begin(), 3);
        bsoncxx::builder::basic::document basic_builder{};
        basic_builder.append(bsoncxx::builder::basic::kvp("", it->get_value()));
        std::string doc_names = bsoncxx::to_json(basic_builder.extract());
        doc_names.erase(0, 9);
        doc_names.erase(doc_names.size() - 4);
        std::string document;
        int position = 0;
        json::value answer = json::value::array();
        for (int i = 0; i < doc_names.size(); i++) {
            if (i == 0) {
                continue;
            }
            if (doc_names[i] == '"') {
                answer[position++] = json::value::string(document);
                document = "";
                i += 3;
            } else {
                document += doc_names[i];
            }
        }
        request.reply(status_codes::OK, answer);
    } else if (path == "/get-document") {
        const std::string doc_name = response.as_array()[0].as_string();
        const std::vector<std::string> lines = get_db().get_document_lines(doc_name);
        json::value answer = json::value::array();
        for (int i = 0; i < lines.size(); i++) {
            answer[i] = json::value::string(lines[i]);
        }
        request.reply(status_codes::OK, answer);
    } else if (path == "/is-server-alive") {
        json::value answer = json::value::boolean(true);
        request.reply(status_codes::OK, answer);
    }
}

void handle_post(http_request request) {
    TRACE("\nhandle POST\n");
    // Обработка POST-запроса
    auto uri = request.absolute_uri();
    auto path = uri.path();
    json::value response;
    request.extract_json()
            .then([&response](
                    pplx::task<json::value>
                    task) { // extracts the request content into a json
                response = task.get();
            })
            .wait();
    if (path == "/add-user") {
        const std::string login = response.as_array()[0].as_string();
        const std::string password = response.as_array()[1].as_string();
        get_db().add_user(login, password);
        request.reply(status_codes::OK, json::value::null());
    } else if (path == "/add-document") {
        const std::string doc_name = response.as_array()[0].as_string();
        const std::string owner = response.as_array()[1].as_string();
        get_db().add_document(doc_name, "", owner);
        get_db().add_document_to_user(owner, doc_name);
        auto response_array = response.as_array();
        for (int i = 2; i < response_array.size(); i++) {
            get_db().add_document_to_user(response_array[i].as_string(), doc_name);
            get_db().add_user_to_document(doc_name, response_array[i].as_string());
        }
        request.reply(status_codes::OK, json::value::null());
    }
}

void handle_put(http_request request) {
    TRACE("\nhandle PUT\n");
    // processing PUT-request
    auto uri = request.absolute_uri();
    const std::string &path = uri.path();
    json::value response;
    request.extract_json()
            .then([&response](
                    pplx::task<json::value>
                    task) { // extracts the request content into a json
                response = task.get();
            })
            .wait();
    if (path == "/put-text") {
        const std::string doc_name = response.as_array()[0].as_string();
        auto response_array = response.as_array();
        for (int i = 1; i < response_array.size(); i++) {
            get_db().change_document_line(doc_name, i - 1, response_array[i].as_string());
        }
        request.reply(status_codes::OK, json::value::null());
    } else {
        request.reply(status_codes::OK, json::value::null());
    }
}

void handle_del(http_request request) {
    TRACE("\nhandle DEL\n");
}

int main() {
    http_listener listener("http://127.0.0.1:23939");

    listener.support(methods::GET, handle_get);
    listener.support(methods::POST, handle_post);
    listener.support(methods::PUT, handle_put);
    listener.support(methods::DEL, handle_del);

    try {
        listener.open()
                .then([&listener]() { TRACE("\nstarting to listen\n"); })
                .wait();
        while (true);
    } catch (std::exception const &e) {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
