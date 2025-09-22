TEMPLATE = app
CONFIG  += c++11
QT      += core gui widgets printsupport serialport

# Clean output layout
OBJECTS_DIR = $$OUT_PWD/tmp/obj
MOC_DIR     = $$OUT_PWD/tmp/moc
RCC_DIR     = $$OUT_PWD/tmp/rcc
UI_DIR      = $$OUT_PWD/tmp/ui
DESTDIR     = $$OUT_PWD/out

# App name / bundle name
TARGET = JS_UDS_UI

# Common files from src/
SRC_DIR = $$PWD
include($$SRC_DIR/files.pri)

# App entry point
SOURCES += $$SRC_DIR/main.cpp

# Link to the static lib built in ../libcore
INCLUDEPATH += $$PWD $$PWD/../libcore
LIBS += -L$$OUT_PWD/../libcore/out -luds_core

# (Optional) silence SDK version warnings on macOS
# CONFIG += sdk_no_version_check
