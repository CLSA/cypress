#ifndef CAROTIDINTIMAMANAGER_H
#define CAROTIDINTIMAMANAGER_H

#include "managers/ManagerBase.h"

class CarotidIntimaManager : public ManagerBase
{
public:
    explicit CarotidIntimaManager(QObject *parent = nullptr);

    virtual void loadSettings(const QSettings &) override;
    virtual void saveSettings(QSettings*) const override;

    virtual QJsonObject toJsonObject() const override;
    virtual void initializeModel() override;
    virtual void updateModel() override;
    virtual void setInputData(const QVariantMap &) override;

public slots:
    virtual void start() override;
    virtual void measure() override;
    virtual void finish() override;

protected:
    virtual void clearData() override;
};

#endif // CAROTIDINTIMAMANAGER_H
