#ifndef DICOMSCP_H
#define DICOMSCP_H

#include <QObject>

class DicomSCP : public QObject
{
    Q_OBJECT
public:
    explicit DicomSCP(QObject *parent = nullptr);

signals:

};

#endif // DICOMSCP_H
