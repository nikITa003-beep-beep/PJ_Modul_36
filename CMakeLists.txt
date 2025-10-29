cmake_minimum_required(VERSION 3.5)
project(ChatCPPTest)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core Widgets REQUIRED LinguistTools)

qt5_add_resources(QT_RESOURCES resources.qrc)

set(SOURCES
    CommandLineInterface.cpp
    Database.cpp
    Message.cpp
    Parsing.cpp
    User.cpp
    sha1.cpp
    loginform.cpp
    mainwindow.cpp
    registrationform.cpp
    startscreen.cpp
    serverwindow.cpp
    Test.cpp
)

set(HEADERS
    CommandLineInterface.h
    Database.h
    Message.h
    Parsing.h
    User.h
    sha1.h
    loginform.h
    mainwindow.h
    registrationform.h
    startscreen.h
    serverwindow.h
)

set(FORMS
    loginform.ui
    mainwindow.ui
    registrationform.ui
    startscreen.ui
    serverwindow.ui
)

add_executable(ChatCPPTest ${SOURCES} ${HEADERS} ${FORMS} ${QT_RESOURCES})

target_link_libraries(ChatCPPTest Qt5::Core Qt5::Widgets)

set(TS_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/translations/my_ru.ts
)

file(GLOB_RECURSE TS_SOURCES "*.cpp" "*.h" "*.ui")

add_custom_target(translations)

foreach(_ts_file ${TS_FILES})
    get_filename_component(_ts_name ${_ts_file} NAME_WE)
    add_custom_command(
        OUTPUT "${_ts_file}"
        DEPENDS ${TS_SOURCES}
        COMMAND ${Qt5_LUPDATE_EXECUTABLE} -recursive
        ${CMAKE_CURRENT_SOURCE_DIR} -ts ${_ts_file}
    )
    add_custom_target(translate_${_ts_name} DEPENDS ${_ts_file})
    add_dependencies(translations translate_${_ts_name})
endforeach()

qt5_add_translation(QM_FILES ${TS_FILES})