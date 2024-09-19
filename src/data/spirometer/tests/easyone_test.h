#ifndef EASYONE_TEST_H
#define EASYONE_TEST_H

#include "data/spirometer/tests/spirometer_test.h"

#include <QJsonObject>
#include <QDomNode>

class EasyoneTest : public SpirometerTest
{

    // SpirometerTest interface
protected:
    void readTrials(const QDomNode &) override;
    void readPDFReportPath(const QDomNode &) override;
};

#endif // EASYONE_TEST_H
