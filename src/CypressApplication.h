#ifndef CYPRESSAPPLICATION_H
#define CYPRESSAPPLICATION_H

#include "auxiliary/Constants.h"
#include "server/Server.h"

#include <QObject>
#include <QCoreApplication>
#include <QJsonObject>

QT_FORWARD_DECLARE_CLASS(DialogBase)

enum Mode {
    Live,
    Sim,
    Debug,
};

enum Status {
    Starting,
    Waiting,
    Active,
};

class CypressApplication : public QObject
{
    Q_OBJECT
public:
    explicit CypressApplication(QObject *parent = Q_NULLPTR);
    ~CypressApplication();

    void setArgs(const QVariantMap&);
    void initialize();

    static QJsonObject getStatus();
    static QScopedPointer<Server> restApiServer;

    static Status status;
    static Mode mode;

    static QElapsedTimer timer;
    static QDateTime startTime;

public slots:
    bool startTest(Constants::MeasureType type);

signals:
    bool endTest(QJsonObject results);

private:
    QString m_inputFileName; // TODO this data should be from POST request in production
    QString m_outputFileName; // TODO ""

    Constants::RunMode m_mode { Constants::RunMode::modeUnknown };
    Constants::MeasureType m_type { Constants::MeasureType::typeUnknown };
    QScopedPointer<DialogBase> m_dialog;

    bool m_verbose = true;
};


#endif // CYPRESSAPPLICATION_H
