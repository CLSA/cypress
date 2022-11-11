#ifndef DXATEST_H
#define DXATEST_H

#include <QObject>
#include <QMap>
#include <QVariant>
#include <QString>

enum Side {
    LEFT,
    RIGHT
};

class DXATest : public QObject
{
    Q_OBJECT
public:
    explicit DXATest(QObject *parent = nullptr);
    Side side;
    QMap<QString, QVariant> data;
};

#endif // DXATEST_H
