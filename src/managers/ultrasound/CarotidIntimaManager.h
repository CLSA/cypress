#ifndef CAROTIDINTIMAMANAGER_H
#define CAROTIDINTIMAMANAGER_H

#include <QCoreApplication>
#include <QMessageBox>
#include <QVector>
#include <QMap>
#include <QString>
#include <QVariant>
#include <QByteArray>
#include <QFile>
#include <QDebug>
#include <QDir>

#include "dcmtk/config/osconfig.h"
#include "dcmtk/dcmdata/dctk.h"
#include "dcmtk/ofstd/ofstdinc.h"




class CarotidIntimaManager {
public:
    QString STILL_IMAGE_KEY = "STILL_IMAGE";
    QString CINELOOP_KEY = "CINELOOP";
    QString SR_KEY = "SR";
    void shutdown() {
    QStringList output = instrumentExecutionService.getExpectedOutputParameterVendorNames();
    QList<QByteArray> listDicomFiles = server.listDicomFiles();

    for (const QVector<QVariant>& row : gui.getData()) {
        QMap<QString, QVariant> values;
        QString suid = row.at(DicomStorageScp::STUDYINSTANCEUID).toString();
        int cineLoopIdx = 1;
        int stillImageIdx = 1;
        int srIdx = 1;
        bool added = false;

        for (const QByteArray& dcmData : listDicomFiles) {
            try {
                DcmInputBufferStream dcmBuffer;
                dcmBuffer.setBuffer(dcmData.data(), dcmData.size());
                DcmFileFormat dcmFile;
                dcmFile.transferInit();
                dcmFile.read(dcmBuffer);
                dcmFile.transferEnd();

                DcmDataset* dataset = dcmFile.getDataset();
                OFString studyInstanceUid;
                dataset->findAndGetOFString(DCM_StudyInstanceUID, studyInstanceUid);
                OFString mediaStorageSOPClassUID;
                dataset->findAndGetOFString(DCM_MediaStorageSOPClassUID, mediaStorageSOPClassUID);
                OFString modality;
                dataset->findAndGetOFString(DCM_Modality, modality);

                if (studyInstanceUid.c_str() == suid) {
                    QString key;

                    if (UID_UltrasoundImageStorage == mediaStorageSOPClassUID.c_str()) {
                        key = STILL_IMAGE_KEY;
                        if (stillImageIdx > 1 || stillImageIdx == 1 && output.contains(STILL_IMAGE_KEY + "_1")) {
                            key.append("_").append(QString::number(stillImageIdx));
                        }
                        stillImageIdx++;
                    } else if (UID_UltrasoundMultiframeImageStorage == mediaStorageSOPClassUID.c_str()) {
                        key = CINELOOP_KEY;
                        if (cineLoopIdx > 1 || cineLoopIdx == 1 && output.contains(CINELOOP_KEY + "_1")) {
                            key.append("_").append(QString::number(cineLoopIdx));
                        }
                        cineLoopIdx++;
                    } else if (SR_KEY == modality.c_str()) {
                        key = SR_KEY;
                        if (srIdx > 1 || srIdx == 1 && output.contains(SR_KEY + "_1")) {
                            key.append("_").append(QString::number(srIdx));
                        }
                        srIdx++;
                    } else {
                        qWarning() << "Received unknown DICOM file. Ignoring.";
                    }

                    if (output.contains(key)) {
                        QByteArray compressedData = qCompress(dcmData); // Compress the data
                        values.insert(key, compressedData);
                        added = true;
                        qDebug() << QString("[%1] dicom file: %2 bytes -- compressed file: %3 bytes")
                                        .arg(key)
                                        .arg(dcmData.size())
                                        .arg(compressedData.size());
                    }
                }
            } catch (const std::exception& e) {
                qCritical() << "Unexpected exception while reading DICOM file:" << e.what();
            }
        }

        if (added && output.contains("SIDE")) {
            QString laterality = row.at(DicomStorageScp::LATERALITY).toString();
            qDebug() << "SIDE is" << laterality;
            values.insert("SIDE", laterality);
        }
    }

    // Clean up DICOM files
    QDir dcmDir("path/to/dicom/directory");
    dcmDir.removeRecursively();
    }

};


#endif // CAROTIDINTIMAMANAGER_H
