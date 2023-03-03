#ifndef CAROTIDINTIMAMANAGER_H
#define CAROTIDINTIMAMANAGER_H

#include "managers/ManagerBase.h"

class CarotidIntimaManager : public ManagerBase
{
public:
    explicit CarotidIntimaManager();

    static bool isInstalled();

public slots:
    virtual void start() override;
    virtual void measure() override;
    virtual void finish() override;

private:
    // Reset the session
    bool clearData() override;

    // Set up device
    bool setUp() override;

    // Clean up the device for next time
    bool cleanUp() override;
};

#endif // CAROTIDINTIMAMANAGER_H
