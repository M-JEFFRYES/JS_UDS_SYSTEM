#include "tst_readdata.h"
#include "DAQ/readdata.h"
#include "AppConstants.h"
#include <QtTest>

void ReadDataTest::init_setsZeroes() {
    ReadData rd;
    QCOMPARE(rd.getInfusionFlowrate(), 0.0);
}

void ReadDataTest::setTestingType_routesDerivedFields() {
    // UDS: expects Q, INFRATE, PDET
    {
        ReadData rd;
        rd.setTestingType(TestTypeConstants::UDS_INVESTIGATION_DESC);
        auto m = rd.readCurrentDataset(
            QStringLiteral("Time,1000000;VI,10;VV,20;PBLAD,30;PABD,12"),
            0, false);
        QVERIFY(m.count(QStringLiteral("Q")) > 0);
        QVERIFY(m.count(QStringLiteral("INFRATE")) > 0);
        QVERIFY(m.count(QStringLiteral("PDET")) > 0);
    }

    // VOLUME_VOID: expects Q only
    {
        ReadData rd;
        rd.setTestingType(TestTypeConstants::VOLUME_VOID_TEST_DESC);
        auto m = rd.readCurrentDataset(QStringLiteral("Time,1000000;VV,20"), 0, false);
        QVERIFY(m.count(QStringLiteral("Q")) > 0);
        QVERIFY(m.count(QStringLiteral("INFRATE")) == 0);
        QVERIFY(m.count(QStringLiteral("PDET")) == 0);
    }

    // VOLUME_INFUSED: expects INFRATE only
    {
        ReadData rd;
        rd.setTestingType(TestTypeConstants::VOLUME_INFUSED_TEST_DESC);
        auto m = rd.readCurrentDataset(QStringLiteral("Time,1000000;VI,15"), 0, false);
        QVERIFY(m.count(QStringLiteral("INFRATE")) > 0);
        QVERIFY(m.count(QStringLiteral("Q")) == 0);
        QVERIFY(m.count(QStringLiteral("PDET")) == 0);
    }

    // INFUSION_RATE: expects INFRATE only
    {
        ReadData rd;
        rd.setTestingType(TestTypeConstants::INFUSION_RATE_TEST_DESC);
        auto m = rd.readCurrentDataset(QStringLiteral("Time,1000000;VI,5"), 0, false);
        QVERIFY(m.count(QStringLiteral("INFRATE")) > 0);
        QVERIFY(m.count(QStringLiteral("Q")) == 0);
        QVERIFY(m.count(QStringLiteral("PDET")) == 0);
    }

    // PRESSURE_TEST: expects none of the derived fields
    {
        ReadData rd;
        rd.setTestingType(TestTypeConstants::PRESSURE_TEST_DESC);
        auto m = rd.readCurrentDataset(QStringLiteral("Time,1000000;PSENS,123"), 0, false);
        QVERIFY(m.count(QStringLiteral("INFRATE")) == 0);
        QVERIFY(m.count(QStringLiteral("Q")) == 0);
        QVERIFY(m.count(QStringLiteral("PDET")) == 0);
    }
}
