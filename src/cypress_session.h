#ifndef CYPRESSSESSION_H
#define CYPRESSSESSION_H

#include <QObject>
#include <QJsonObject>

class CypressSession
{
public:
    CypressSession(QJsonObject inputData);
    ~CypressSession();

    static bool isValid(const QJsonObject& inputData);
    QString getSessionId();
    QString getBarcode();
    QString getLanguage();
    QString getInterviewer();

private:
    QJsonObject m_inputData;

    QString m_interview;
    QString m_sessionId;
    QString m_barcode;
    QString m_language;

    QDateTime m_startDateTime;
    QDateTime m_endDateTime;
};

#endif // CYPRESSSESSION_H
