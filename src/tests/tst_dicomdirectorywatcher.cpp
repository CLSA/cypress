#include "tst_dicomdirectorywatcher.h"
#include <dcmtk/dcmdata/dcfilefo.h>
#include <dcmtk/dcmdata/dcdeftag.h>

void Test_DicomDirectoryWatcher::initTestCase()
{
    // Initialization before running any test case
}

void Test_DicomDirectoryWatcher::cleanupTestCase()
{
    // Cleanup after running all test cases
}

void Test_DicomDirectoryWatcher::testValidateTags()
{
    DicomDirectoryWatcher watcher("");  // Create a watcher with an empty path

    // Test a DICOM file with all required tags and correct values
    {
       DcmFileFormat fileFormat;

       // Create a valid DICOM file format object
       DcmDataset* dataset = fileFormat.getDataset();
       dataset->putAndInsertString(DCM_PatientName, "John Doe");
       dataset->putAndInsertString(DCM_PatientID, "12345");
       // ... add other required dataset tags

       DcmMetaInfo* metaInfo = fileFormat.getMetaInfo();
       metaInfo->putAndInsertString(DCM_MediaStorageSOPClassUID, "1.2.3.4.5");
       metaInfo->putAndInsertString(DCM_MediaStorageSOPInstanceUID, "1.2.3.4.5.6");

       // ... add other required meta info tags

       QVERIFY(watcher.validateTags(fileFormat));  // Expect the validation to succeed
    }

    // Test a DICOM file with a missing required tag
    {
       DcmFileFormat fileFormat;

       // Create a DICOM file format object with a missing tag
       DcmDataset* dataset = fileFormat.getDataset();
       dataset->putAndInsertString(DCM_PatientName, "John Doe");
       // DCM_PatientID tag is intentionally missing
       // ... add other required dataset tags

       DcmMetaInfo* metaInfo = fileFormat.getMetaInfo();
       metaInfo->putAndInsertString(DCM_MediaStorageSOPClassUID, "1.2.3.4.5");
       metaInfo->putAndInsertString(DCM_MediaStorageSOPInstanceUID, "1.2.3.4.5.6");
       // ... add other required meta info tags

       QVERIFY(!watcher.validateTags(fileFormat));  // Expect the validation to fail
    }

    // Test a DICOM file with an incorrect tag value
    {
       DcmFileFormat fileFormat;

       // Create a DICOM file format object with an incorrect tag value
       DcmDataset* dataset = fileFormat.getDataset();
       dataset->putAndInsertString(DCM_PatientName, "John Doe");
       dataset->putAndInsertString(DCM_PatientID, "incorrect_value");
       // ... add other required dataset tags

       DcmMetaInfo* metaInfo = fileFormat.getMetaInfo();
       metaInfo->putAndInsertString(DCM_MediaStorageSOPClassUID, "1.2.3.4.5");
       metaInfo->putAndInsertString(DCM_MediaStorageSOPInstanceUID, "1.2.3.4.5.6");
       // ... add other required meta info tags

       QVERIFY(!watcher.validateTags(fileFormat));  // Expect the validation to fail
    }
}

void Test_DicomDirectoryWatcher::testOnDirectoryChanged()
{
    QTemporaryDir tempDir;  // Create a temporary directory

    // Ensure the temporary directory was created successfully
    QVERIFY(tempDir.isValid());

    DicomDirectoryWatcher watcher(tempDir.path());  // Create a watcher for the temporary directory

    // Create a signal spy to track directoryChanged signal emissions
    QSignalSpy directoryChangedSpy(&watcher, &DicomDirectoryWatcher::directoryChanged);

    // Test adding a DICOM file to the directory
    {
        QString testFilePath = tempDir.path() + "/test1.dcm";
        QFile testFile(testFilePath);
        QVERIFY(testFile.open(QIODevice::WriteOnly));
        testFile.write("Dummy DICOM data");
        testFile.close();

        QThread::msleep(100);  // Give the watcher some time to detect the change
        QCoreApplication::processEvents();

        QVERIFY(directoryChangedSpy.count() == 1);
        QCOMPARE(directoryChangedSpy.takeFirst().at(0).toString(), tempDir.path());

        // Run onDirectoryChanged() and check for any issues with the added file
        watcher.onDirectoryChanged(tempDir.path());
    }

    // Test removing a DICOM file from the directory
    {
        QString testFilePath = tempDir.path() + "/test1.dcm";
        QVERIFY(QFile::remove(testFilePath));

        QThread::msleep(100);  // Give the watcher some time to detect the change
        QCoreApplication::processEvents();

        QVERIFY(directoryChangedSpy.count() == 1);
        QCOMPARE(directoryChangedSpy.takeFirst().at(0).toString(), tempDir.path());

        // Run onDirectoryChanged() and check for any issues with the removed file
        watcher.onDirectoryChanged(tempDir.path());
    }
}
