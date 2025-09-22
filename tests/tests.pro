TEMPLATE = app
CONFIG  += c++11 testcase console
QT      += core testlib        # add 'widgets' if you test GUI classes

OBJECTS_DIR = $$OUT_PWD/tmp/obj
MOC_DIR     = $$OUT_PWD/tmp/moc
RCC_DIR     = $$OUT_PWD/tmp/rcc
UI_DIR      = $$OUT_PWD/tmp/ui
DESTDIR     = $$OUT_PWD/out

TARGET = uds_core_tests

SOURCES += $$PWD/tst_smoke.cpp

INCLUDEPATH += $$PWD/../src $$PWD/../libcore
LIBS += -L$$OUT_PWD/../libcore/out -luds_core
