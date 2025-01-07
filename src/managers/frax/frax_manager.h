#ifndef FRAX_MANAGER_H
#define FRAX_MANAGER_H

#include "managers/manager_base.h"
#include "server/sessions/frax_session.h"

#include "config/device_config.h"

#include <QProcess>

/*!
 * \class FraxManager
 * \brief The FraxManager class
 *
 * Concrete child class implementation of a device manager.
 * This class facilitates launch of the Frax (fracture risk assessment)
 * module (blackbox.exe) and reading the test output
 * .txt files it produces.  QProcess is used to facilitate operations.
 *
 * \sa ManagerBase, CDTTManager, ChoiceReactionManager
 *
 */

class FraxManager : public ManagerBase
{
    Q_OBJECT

public:
    explicit FraxManager(QSharedPointer<FraxSession> session);

    static QSharedPointer<DeviceConfig> config;

public slots:
    bool start() override;

    void measure() override;

    void readOutput() override;

private:
    QProcess m_process;

    // Reset the session
    bool clearData() override;

    // Set up device
    bool setUp() override;

    // Clean up the device for next time
    bool cleanUp() override;
};

#endif // FRAX_MANAGER_H
