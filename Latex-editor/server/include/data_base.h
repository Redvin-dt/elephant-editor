#ifndef DATA_BASE
#define DATA_BASE

#include <bsoncxx/json.hpp>
#include <memory>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <stdexcept>
#include <string>
#include <vector>

namespace mongodb {
/**
 * @brief DataBase interaction class
 *
 */
    class DataBase {
        std::unique_ptr<mongocxx::instance> inst;
        std::unique_ptr<mongocxx::client> conn;
        std::unique_ptr<mongocxx::v_noabi::options::server_api> api;
        std::unique_ptr<mongocxx::v_noabi::uri> uri;

        mongocxx::v_noabi::collection documents_collection;
        mongocxx::v_noabi::collection users_collection;
        mongocxx::client client;
        mongocxx::database db;

    public:
        /**
         * @brief Construct a new Data Base object
         *
         */
        DataBase();

        /**
         * @brief Get the document object
         *
         * @param document_name
         * @return bsoncxx::document::value
         */
        bsoncxx::document::value get_document(const std::string &document_name);

        /**
         * @brief Get the user object
         *
         * @param user_name
         * @return bsoncxx::document::value
         */
        bsoncxx::document::value get_user(const std::string &user_name);

        /**
         * @brief  add user to user_colection
         *
         * @param user_name
         */
        void add_user(const std::string &user_name, const std::string &user_password);

        /**
         * @brief add document to document collection
         *
         * @param document_name
         * @param text
         */
        void add_document(const std::string &document_name,
                          const std::string &text, const std::string &owner);

        /**
         * @brief add avalible document to user
         *
         * @param user_name
         * @param document_name
         */
        void add_document_to_user(const std::string &user_name,
                                  const std::string &document_name);

        /**
         * @brief add user that has access for document
         *
         * @param document_name
         * @param user_name
         */
        void add_user_to_document(const std::string &document_name,
                                  const std::string &user_name);

        /**
         * @brief change line in document with given name
         *
         * @param document_name
         * @param line_number
         * @param new_text
         */
        void change_document_line(const std::string &document_name,
                                  std::size_t line_number,
                                  const std::string &new_text);

        /**
         *
         * @param login
         * @param password
         * @return
         */
        bool check_is_user_exist(const std::string &login, const std::string &password);

        /**
         *
         * @param login
         * @return
         */
        bool check_is_user_exist(const std::string &login);

        /**
         * @brief change whole document text in db
         *
         * @param document_name
         * @param new_text
         * @return * void
         */
        void change_document_text(const std::string &document_name,
                                  const std::string &new_text);

        /**
         * @brief Get the document lines object
         *
         * @param document_name
         * @return std::vector<std::string>
         */
        std::vector<std::string>
        get_document_lines(const std::string &document_name);
    };

    class AccessErrors : public std::runtime_error {
    public:
        AccessErrors(const std::string &message) : runtime_error(message) {}
    };

} // namespace mongodb

#endif // DATA_BASE
