#ifndef FRAX_TEST_H
#define FRAX_TEST_H

#include "data/test_base.h"
#include "server/sessions/frax_session.h"
#include "config/device_config.h"

class FraxTest : public TestBase
{
public:
    FraxTest();
    ~FraxTest() = default;

    bool writeInputFile(const QString &inputFilePath, const QJsonObject& inputData);
    bool readOutputFile(const QString&);

    // String representation for debug and GUI display purposes
    //
    QString toString() const override;

    bool isValid() const override;

    // String keys are converted to snake_case
    //
    QJsonObject toJsonObject() const override;

    QString getInputContents(const QJsonObject &inputData);
    QString interpretResults(double p);
private:
    QStringList m_outputKeyList;
    QString m_workingDirectoryPath;
    QString m_exePath;
    QSharedPointer<FraxSession> m_session;
};

Q_DECLARE_METATYPE(FraxTest);

#endif // FRAX_TEST_H
