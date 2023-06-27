#include "../include/Server.h"

using namespace web;
using namespace web::http;
using namespace web::http::client;

namespace cpprest_server {

    Server::Server() : client("http://127.0.0.1:23939") {
    }

    Server &get_server() {
        static Server server = Server();
        return server;
    }

    void display_json(json::value const &jvalue, utility::string_t const &prefix) {
        std::cerr << prefix << ' ' << jvalue.serialize() << std::endl;
    }

    pplx::task<http_response>
    Server::make_task_request(const method &mtd, const std::string &path, json::value const &jvalue) {
        return client.request(mtd, path, jvalue);
    }

    json::value Server::make_request(const method &mtd, const std::string &path, json::value const &jvalue) {
        json::value response;
        make_task_request(mtd, path, jvalue)
                .then([](const http_response &response) {
                    if (response.status_code() == status_codes::OK) {
                        return response.extract_json();
                    }
                    return pplx::task_from_result(json::value());
                }).then([&response](const pplx::task<json::value> &previousTask) {
            try {
                response = previousTask.get();
            } catch (http_exception const &e) {
                std::cout << e.what() << std::endl;
            }
        }).wait();
        return response;
    }

}; // namespace cpprest_server