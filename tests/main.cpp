// tests/main.cpp
#include <QApplication>
#include <QtTest>

#include "tst_smoke.h"
#include "DAQ/tst_readdata.h"

int main(int argc, char** argv) {
    QApplication app(argc, argv); // ok for non-GUI tests too
    int status = 0;

    status |= QTest::qExec(new SmokeTest, argc, argv);
    status |= QTest::qExec(new ReadDataTest, argc, argv);

    return status;
}
