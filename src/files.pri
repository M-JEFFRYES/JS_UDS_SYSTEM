# Expect the including .pro to define:
#   SRC_DIR = absolute path to the 'src' directory

INCLUDEPATH += \
    $$SRC_DIR \
    $$SRC_DIR/Controls \
    $$SRC_DIR/DAQ \
    $$SRC_DIR/Display \
    $$SRC_DIR/Popups \
    $$SRC_DIR/libs

SOURCES += \
    $$SRC_DIR/Controls/investigationbar.cpp \
    $$SRC_DIR/Controls/investigationtypebar.cpp \
    $$SRC_DIR/Controls/pumpcontrol.cpp \
    $$SRC_DIR/DAQ/connectionbar.cpp \
    $$SRC_DIR/DAQ/readdata.cpp \
    $$SRC_DIR/DAQ/writedata.cpp \
    $$SRC_DIR/Display/footerdisplay.cpp \
    $$SRC_DIR/Display/tracesdisplay.cpp \
    $$SRC_DIR/Display/valuedisplaybar.cpp \
    $$SRC_DIR/Popups/patientinfoentry.cpp \
    $$SRC_DIR/libs/qcustomplot.cpp \
    $$SRC_DIR/mainwindow.cpp

HEADERS += \
    $$SRC_DIR/Controls/investigationbar.h \
    $$SRC_DIR/Controls/investigationtypebar.h \
    $$SRC_DIR/Controls/pumpcontrol.h \
    $$SRC_DIR/AppConstants.h \
    $$SRC_DIR/DAQ/connectionbar.h \
    $$SRC_DIR/DAQ/readdata.h \
    $$SRC_DIR/DAQ/writedata.h \
    $$SRC_DIR/Display/footerdisplay.h \
    $$SRC_DIR/Display/tracesdisplay.h \
    $$SRC_DIR/Display/valuedisplaybar.h \
    $$SRC_DIR/Popups/patientinfoentry.h \
    $$SRC_DIR/libs/qcustomplot.h \
    $$SRC_DIR/mainwindow.h

FORMS += \
    $$SRC_DIR/Controls/investigationbar.ui \
    $$SRC_DIR/Controls/investigationtypebar.ui \
    $$SRC_DIR/Controls/pumpcontrol.ui \
    $$SRC_DIR/DAQ/connectionbar.ui \
    $$SRC_DIR/Display/footerdisplay.ui \
    $$SRC_DIR/Display/tracesdisplay.ui \
    $$SRC_DIR/Display/valuedisplaybar.ui \
    $$SRC_DIR/Popups/patientinfoentry.ui \
    $$SRC_DIR/mainwindow.ui

RESOURCES += \
    $$SRC_DIR/resources.qrc
