#ifndef CAROTIDINTIMAMANAGER_H
#define CAROTIDINTIMAMANAGER_H

#include "managers/ManagerBase.h"

class CarotidIntimaManager : public ManagerBase
{
public:
    explicit CarotidIntimaManager(QWidget *parent = nullptr);

public slots:
    virtual void start() override;
    virtual void measure() override;
    virtual void finish() override;

protected:
    virtual void clearData() override;
};

#endif // CAROTIDINTIMAMANAGER_H
