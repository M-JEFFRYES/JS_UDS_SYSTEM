#include <QtTest>
#include <QObject>

class SmokeTest : public QObject {
    Q_OBJECT
private slots:
    void math_works() {
        QCOMPARE(2 + 2, 4);
    }
};

QTEST_APPLESS_MAIN(SmokeTest)
#include "tst_smoke.moc"
