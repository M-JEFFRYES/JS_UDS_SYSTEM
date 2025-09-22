TEMPLATE = lib
CONFIG  += staticlib c++11
QT      += core gui widgets printsupport serialport

# Keep build outputs tidy
OBJECTS_DIR = $$OUT_PWD/tmp/obj
MOC_DIR     = $$OUT_PWD/tmp/moc
RCC_DIR     = $$OUT_PWD/tmp/rcc
UI_DIR      = $$OUT_PWD/tmp/ui
DESTDIR     = $$OUT_PWD/out

# Name of the library -> libuds_core.a
TARGET = uds_core

# Point the file list to src/
SRC_DIR = $$PWD/../src
include($$SRC_DIR/files.pri)

# Ensure main.cpp isn't compiled into the library
SOURCES -= $$SRC_DIR/main.cpp
