#pragma once
#include <QHash>
#include <QString>
#include <QVector>

// What each test type provides to ReadData
struct TestConfig {
    QVector<QString> all;
    QVector<QString> sercomm;
    QVector<QString> plot;
};

// Global, read-only map (defined in TestConfigs.cpp)
extern const QHash<QString, TestConfig> kTestConfigs;