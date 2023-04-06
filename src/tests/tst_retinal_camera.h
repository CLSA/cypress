#ifndef TSTRETINALCAMERA_H
#define TSTRETINALCAMERA_H

#include <QObject>

class TestRetinalCamera : public QObject
{
    Q_OBJECT
public:
    TestRetinalCamera();

private Q_SLOTS:
    void testInitialize();
    void testExtractExam();
};

#endif // TSTRETINALCAMERA_H
