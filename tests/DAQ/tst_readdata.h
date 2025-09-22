#pragma once
#include <QObject>

class ReadDataTest : public QObject {
    Q_OBJECT
  private slots:
    void init_setsZeroes();
    void setTestingType_routesDerivedFields();
};
