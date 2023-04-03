#ifndef TEST_DICOMDIRECTORYWATCHER_H
#define TEST_DICOMDIRECTORYWATCHER_H

#include <QtTest>

class Test_DicomDirectoryWatcher : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();
    void testValidateTags();
    void testOnDirectoryChanged();
};

#endif // TEST_DICOMDIRECTORYWATCHER_H
