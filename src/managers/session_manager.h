#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include <QObject>

enum Language
{
   English,
   French,
};

class Params
{
public:
    QString m_uuid {};
    QString m_answer_id {};
    QString m_barcode {};
    QString m_language {};
    Language language { English };
};

class Session: public QObject
{
    Q_OBJECT
public:
    explicit Session(QObject *parent = nullptr);

    void start(Params& params);
    void end();

    QString getId();
    QString getAnswerId();
    QString getBarcode();
    QString getLanguage();

signals:

private:
    Params m_params;
};

#endif // SESSIONMANAGER_H
