#include "../include/data_base.h"
#include <bsoncxx/array/value.hpp>
#include <bsoncxx/builder/basic/array.hpp>
#include <bsoncxx/types.hpp>
#include <sstream>
#include <vector>
#include <iomanip>
#include "openssl/sha.h"

namespace mongodb {

    const std::string port = "mongodb://localhost:27017";

    using bsoncxx::builder::basic::kvp;
    using bsoncxx::builder::basic::make_array;
    using bsoncxx::builder::basic::make_document;

    DataBase::DataBase() {
        // Create an instance.
        inst = std::make_unique<mongocxx::instance>();

        // Replace the connection string with your MongoDB deployment's
        // connection string.
        uri = std::make_unique<mongocxx::uri>(port);

        // Set the version of the Stable API on the client.
        mongocxx::options::client client_options;
        api = std::make_unique<mongocxx::options::server_api>(
                mongocxx::options::server_api::version::k_version_1);
        client_options.server_api_opts(*api);

        // Setup the connection and get a handle on the "admin" database.
        conn = std::make_unique<mongocxx::client>(*uri, client_options);
        mongocxx::database db = (*conn)["elephant_editor"];

        users_collection = db["users"];
        documents_collection = db["documents"];
    }

    std::string sha256(const std::string &str) {
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, str.c_str(), str.size());
        SHA256_Final(hash, &sha256);
        std::stringstream ss;
        for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
            ss << std::hex << std::setw(2) << std::setfill('0') << (int) hash[i];
        }
        return ss.str();
    }

    bsoncxx::document::value
    DataBase::get_document(const std::string &document_name) {
        auto document = documents_collection.find_one(
                make_document(kvp("name", document_name)));

        if (document) {
            return *document;
        } else {
            throw AccessErrors("Invalid document name");
        }
    }

    bsoncxx::document::value DataBase::get_user(const std::string &user_name) {
        auto user =
                users_collection.find_one(make_document(kvp("name", user_name)));

        if (user) {
            return *user;
        } else {
            throw AccessErrors("Invalid user name");
        }
    }

    void DataBase::add_user(const std::string &user_name, const std::string &user_password) {
        std::string hashed_password = sha256(user_password);
        users_collection.insert_one(
                make_document(kvp("name", user_name),
                              kvp("password", hashed_password),
                              kvp("documents", bsoncxx::builder::basic::array())));
    }

    std::vector<std::string> separate_lines(const std::string &text) {
        std::vector<std::string> result;
        std::string line;
        for (int i = 0; i < text.size(); i++) {
            if (text[i] == '\n') {
                result.emplace_back(line);
                line = "";
            } else {
                line += text[i];
            }
        }

        result.emplace_back(line);

        return result;
    }

    void DataBase::add_document(const std::string &document_name,
                                const std::string &text, const std::string &owner) {

        static int id = 0;

        std::vector<std::string> lines = separate_lines(text);

        auto array_builder = bsoncxx::builder::basic::array{};

        for (const auto &element: lines) {
            array_builder.append(element);
        }

        auto users = bsoncxx::builder::basic::array{};
        users.append(owner);

        documents_collection.insert_one(
                make_document(kvp("id", id++), kvp("name", document_name), kvp("lines", array_builder.extract()),
                              kvp("users", users), kvp("owner", owner)));
    }

    void DataBase::add_document_to_user(const std::string &user_name,
                                        const std::string &document_name) {

        auto user = users_collection.update_one(
                make_document(kvp("name", user_name)),
                make_document(
                        kvp("$push", make_document(kvp("documents", document_name)))));
    }

    void DataBase::change_document_line(const std::string &document_name,
                                        std::size_t line_number,
                                        const std::string &new_text) {
        auto doc = documents_collection.update_one(
                make_document(kvp("name", document_name)),
                make_document(kvp(
                        "$set", make_document(kvp("lines." + std::to_string(line_number),
                                                  new_text)))));
    }

    void DataBase::change_document_text(const std::string &document_name,
                                        const std::string &new_text) {

        std::vector<std::string> lines = separate_lines(new_text);

        auto array_builder = bsoncxx::builder::basic::array{};

        for (const auto &element: lines) {
            array_builder.append(element);
        }

        for (std::size_t index = 0; index < lines.size(); index++) {
            auto doc = documents_collection.update_one(
                    make_document(kvp("name", document_name)),
                    make_document(
                            kvp("$set", make_document(kvp("lines." + std::to_string(index),
                                                          lines[index])))));
        }
    }

    std::vector<std::string>
    DataBase::get_document_lines(const std::string &document_name) {
        auto lines = documents_collection.find_one(
                make_document(kvp("name", document_name)));

        if (lines) {
            std::vector<std::string> result;
            for (const auto &item:
                    bsoncxx::array::view((*lines)["lines"].get_array())) {
                result.emplace_back(item.get_string().value.to_string());
            }

            return result;
        } else {
            throw AccessErrors("Invalid document name");
        }
    }

    bool DataBase::check_is_user_exist(const std::string &login) {
        auto user = users_collection.find_one(make_document(kvp("name", login)));
        if (user) {
            return true;
        } else {
            return false;
        }
    }

    bool DataBase::check_is_user_exist(const std::string &login, const std::string &password) {
        std::string hashed_password = sha256(password);
        auto user = users_collection.find_one(make_document(kvp("name", login), kvp("password", hashed_password)));
        if (user) {
            return true;
        } else {
            return false;
        }
    }

    void DataBase::add_user_to_document(const std::string &document_name,
                                        const std::string &user_name) {
        auto user = documents_collection.update_one(
                make_document(kvp("name", document_name)),
                make_document(kvp("$push", make_document(kvp("users", user_name)))));
    }

} // namespace mongodb
