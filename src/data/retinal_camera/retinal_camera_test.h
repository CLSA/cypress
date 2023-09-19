#ifndef RETINALCAMERATEST_H
#define RETINALCAMERATEST_H

#include "data/TestBase.h"

class RetinalCameraTest : public TestBase<Measurement>
{
public:
    RetinalCameraTest();

    void simulate();
    bool isValid() const override;

    QJsonObject toJsonObject() const override;
    QString toString() const override;

private:
    QStringList m_outputKeyList;
    QMap<QString, QString> m_displayNames;
};

#endif // RETINALCAMERATEST_H
