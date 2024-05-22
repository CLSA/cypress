#ifndef ECG_TEST_H
#define ECG_TEST_H

#include "data/test_base.h"

QT_FORWARD_DECLARE_CLASS(QDomNode)

class ECGTest : public TestBase
{
public:
    ECGTest();
    ~ECGTest() = default;

    void fromFile(const QString &);
    bool isValid() const override;

    QString toString() const override;
    QJsonObject toJsonObject() const override;

private:
    QList<QString> m_outputKeyList;

    void readObservationDatetime(const QDomNode&);
    void readClinicalInfo(const QDomNode&);
    void readPatientInfo(const QDomNode&);
    void readFilterSetting(const QDomNode&);
};

Q_DECLARE_METATYPE(ECGTest);

#endif // ECG_TEST_H
