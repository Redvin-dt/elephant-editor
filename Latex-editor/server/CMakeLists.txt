cmake_minimum_required(VERSION 3.25)
project(SDK_SERVER)

set(CMAKE_CXX_STANDARD 20)

find_package(cpprestsdk REQUIRED NAMES cpprestsdk cpprest)
find_package(libmongocxx REQUIRED)
find_package(libbsoncxx REQUIRED)
find_package(OpenSSL REQUIRED)

add_executable(SDK_SERVER src/server.cpp src/data_base.cpp include/data_base.h)

include_directories(${LIBMONGOCXX_INCLUDE_DIR})
include_directories(${LIBBSONCXX_INCLUDE_DIR})
include_directories("/usr/local/include/mongocxx/v_noabi")
include_directories("/usr/local/include/bsoncxx/v_noabi")
include_directories("/usr/local/include/libmongoc-1.0")
include_directories("/usr/local/include/libbson-1.0")
include_directories("/usr/local/lib")

# target_include_directories(${PROJECT_NAME} PRIVATE "/usr/local/include/mongocxx/v_noabi")
# target_include_directories(${PROJECT_NAME} PRIVATE "/usr/local/include/bsoncxx/v_noabi")

target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBMONGOCXX_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBBSONCXX_LIBRARIES})
target_link_libraries(SDK_SERVER PRIVATE cpprestsdk::cpprest OpenSSL::Crypto)
target_link_libraries(SDK_SERVER PRIVATE OpenSSL::SSL)

