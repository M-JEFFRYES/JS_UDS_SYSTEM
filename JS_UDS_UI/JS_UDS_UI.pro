QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Controls/investigationtypebar.cpp \
    DAQ/connectionbar.cpp \
    DAQ/readdata.cpp \
    Display/tracesdisplay.cpp \
    Display/valuedisplaybar.cpp \
    libs/qcustomplot.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Controls/investigationtypebar.h \
    DAQ/connectionbar.h \
    DAQ/readdata.h \
    Display/tracesdisplay.h \
    Display/valuedisplaybar.h \
    libs/qcustomplot.h \
    mainwindow.h

FORMS += \
    Controls/investigationtypebar.ui \
    DAQ/connectionbar.ui \
    Display/tracesdisplay.ui \
    Display/valuedisplaybar.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
