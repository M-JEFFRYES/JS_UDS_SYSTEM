#include <QtTest>

// add necessary includes here

class MainApplication : public QObject
{
    Q_OBJECT

public:
    MainApplication();
    ~MainApplication();

private slots:
    void test_case1();

};

MainApplication::MainApplication()
{

}

MainApplication::~MainApplication()
{

}

void MainApplication::test_case1()
{

}

QTEST_APPLESS_MAIN(MainApplication)

#include "tst_mainapplication.moc"
