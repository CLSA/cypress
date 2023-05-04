#ifndef CAROTIDINTIMAMANAGER_H
#define CAROTIDINTIMAMANAGER_H

#include <QObject>
#include "managers/ManagerBase.h"

class CarotidIntimaManager : public ManagerBase
{
    Q_OBJECT
public:
    explicit CarotidIntimaManager(QObject *parent = nullptr);

public slots:
    void start() override;
    void measure() override;
    void finish() override;

protected:
    void setInputData(const QVariantMap &) override;
    bool setUp() override;
    bool clearData() override;
    bool cleanUp() override;
};

#endif // CAROTIDINTIMAMANAGER_H
