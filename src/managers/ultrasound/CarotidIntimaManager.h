#ifndef CAROTIDINTIMAMANAGER_H
#define CAROTIDINTIMAMANAGER_H

#include <QObject>

class CarotidIntimaManager : public QObject
{
    Q_OBJECT
public:
    explicit CarotidIntimaManager(QObject *parent = nullptr);
};

#endif // CAROTIDINTIMAMANAGER_H
