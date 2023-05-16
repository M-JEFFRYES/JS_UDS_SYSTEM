cmake_minimum_required(VERSION 3.5)

project(JS_UDS_TESTS LANGUAGES CXX)

enable_testing()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Test)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(MainApplication tst_mainapplication.cpp)
add_test(NAME MainApplication COMMAND MainApplication)

target_link_libraries(MainApplication PRIVATE Qt${QT_VERSION_MAJOR}::Test JS_UDS_LOGIC)

