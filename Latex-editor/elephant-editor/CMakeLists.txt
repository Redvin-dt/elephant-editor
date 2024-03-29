cmake_minimum_required(VERSION 3.5)

project(Latex-editor VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FindPkgConfig)
pkg_check_modules(POPPLER_QT5 REQUIRED IMPORTED_TARGET poppler-qt5)

#add_compile_options(-fsanitize=address)
#add_compile_options(-fsanitize=undefined)



find_package(QT NAMES Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt5 REQUIRED COMPONENTS Gui)
find_package(Qt5 REQUIRED COMPONENTS Core)
find_package(OpenSSL REQUIRED)

find_package(cpprestsdk REQUIRED NAMES cpprestsdk cpprest)

set(TS_FILES Latex-editor_en_US.ts)

set(PROJECT_SOURCES
        src/main.cpp
        src/mainwindow.cpp
        include/mainwindow.h
        src/mainwindow.ui
        src/CodeEditor.cpp
        include/CodeEditor.h
        src/SyntaxStyle.cpp
        include/SyntaxStyle.h
        src/ImageWidget.cpp
        include/ImageWidget.h
        src/LineNumberArea.cpp
        include/LineNumberArea.h
        src/LatexCompleter.cpp
        include/LatexCompleter.h
        src/SyntaxHighlighter.cpp
        include/SyntaxHighlighter.h
        src/MathButtons.cpp
        include/MathButtons.h
        src/FindWidget.cpp
        include/FramedText.h
        src/FramedText.cpp
        include/FindWidget.h
        src/FindWidget.ui
        include/TableWidget.h
        src/TableWidget.cpp
        src/ShowInfoWidget.cpp
        include/ShowInfoWidget.h
        src/ShowInfoWidget.ui
        src/Server.cpp
        include/Server.h
        src/ImageInsert.cpp
        include/ImageInsert.h
        src/ImageInsert.ui
        include/Authorization.h
        src/Authorization.cpp
        src/Authorization.ui
        src/SignUp.ui
        src/SignUp.cpp
        include/SignUp.h
        src/CreateDocument.cpp
        include/CreateDocument.h
        src/CreateDocument.ui
        include/SelectDocument.h
        src/SelectDocument.cpp
        ${TS_FILES}
        )

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Latex-editor
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Latex-editor APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(Latex-editor SHARED
            ${PROJECT_SOURCES}
            ${RESOURCES_FILE}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Latex-editor
            ${PROJECT_SOURCES}
            resources/codeeditor_resources.qrc
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(Latex-editor PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(Latex-editor PRIVATE Qt${QT_VERSION_MAJOR}::Gui)
target_link_libraries(Latex-editor PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(Latex-editor PRIVATE PkgConfig::POPPLER_QT5)
target_link_libraries(Latex-editor PRIVATE cpprestsdk::cpprest OpenSSL::Crypto)
target_link_libraries(Latex-editor PRIVATE OpenSSL::SSL)

set_target_properties(Latex-editor PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS Latex-editor
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Latex-editor)
endif()
