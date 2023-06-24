#ifndef SERVER_H
#define SERVER_H

#include <cpprest/http_client.h>
#include <cpprest/json.h>
#pragma comment(lib, "cpprest_2_10")

using namespace web;
using namespace web::http;
using namespace web::http::client;

namespace cpprest_server {

    class Server {
    public:
        Server();

        json::value make_request(const method& mtd, const std::string& path, json::value const &jvalue);

        pplx::task<http_response> make_task_request(const method& mtd, const std::string& path, json::value const &jvalue);

    private:

        http_client client;

    };

    Server& get_server();

    void display_json(json::value const &jvalue, utility::string_t const &prefix);


}; // namespace cpprest_server
#endif // SERVER_H
