#ifndef VIVIDI_MANAGER_H
#define VIVIDI_MANAGER_H

#include <QObject>
#include "managers/manager_base.h"

class VividiManager: public ManagerBase
{
    Q_OBJECT
public:
    explicit VividiManager(QObject *parent = nullptr);

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

#endif // VIVIDI_MANAGER_H
