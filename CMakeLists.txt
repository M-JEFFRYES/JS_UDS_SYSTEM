cmake_minimum_required(VERSION 3.15)

enable_testing()
project(JS_UDS LANGUAGES CXX)


find_package(QT NAMES Qt6 Qt5 COMPONENTS Core Qml Quick REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Qml Quick REQUIRED)

add_subdirectory(JS_UDS_APP)
add_subdirectory(JS_UDS_LOGIC)
add_subdirectory(JS_UDS_TESTS)

