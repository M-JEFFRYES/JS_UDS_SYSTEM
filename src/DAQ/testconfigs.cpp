#include "DAQ/testconfigs.h"
#include "AppConstants.h" // TestTypeConstants + *Consts::VARS_*

// Build once at start-up
static QHash<QString, TestConfig> buildConfigs() {
    QHash<QString, TestConfig> h;

    h.insert(TestTypeConstants::PRESSURE_TEST_DESC, {PressureConsts::VARS_ALL,
                                                     PressureConsts::VARS_SERCOMM,
                                                     PressureConsts::VARS_PLOT});

    h.insert(TestTypeConstants::VOLUME_VOID_TEST_DESC, {VolumeVoidConsts::VARS_ALL,
                                                        VolumeVoidConsts::VARS_SERCOMM,
                                                        VolumeVoidConsts::VARS_PLOT});

    h.insert(TestTypeConstants::VOLUME_INFUSED_TEST_DESC, {VolumeInfusedConsts::VARS_ALL,
                                                           VolumeInfusedConsts::VARS_SERCOMM,
                                                           VolumeInfusedConsts::VARS_PLOT});

    h.insert(TestTypeConstants::INFUSION_RATE_TEST_DESC, {InfusionRateConsts::VARS_ALL,
                                                          InfusionRateConsts::VARS_SERCOMM,
                                                          InfusionRateConsts::VARS_PLOT});

    h.insert(TestTypeConstants::UDS_INVESTIGATION_DESC, {UDSConsts::VARS_ALL,
                                                         UDSConsts::VARS_SERCOMM,
                                                         UDSConsts::VARS_PLOT});

    return h;
}

const QHash<QString, TestConfig> kTestConfigs = buildConfigs();
