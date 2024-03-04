#include "apex_transfer_widget.h"
#include "cypress_settings.h"

#include "ui_apex_transfer_widget.h"

#include <QDir>
#include <QFileInfo>
#include <QDebug>

ApexTransferWidget::ApexTransferWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ApexTransferWidget)
{
    ui->setupUi(this);

    m_patscanDbPath = CypressSettings::readSetting("dxa/patscanDbPath").toString();
    m_referenceDbPath = CypressSettings::readSetting("dxa/refscanDbPath").toString();
}

ApexTransferWidget::~ApexTransferWidget() {
    delete ui;
}


void ApexTransferWidget::transfer() {
    emit started();

    emit status("Transfering files from DEXA (1/2)");
    if (!copyPatScanDb())
        emit failed();

    emit status("Transfering files from DEXA (2/2)");
    if (!copyReferenceDb())
        emit failed();

    emit completed(m_patscanDbFileInfo, m_referenceDbFileInfo);
}

bool ApexTransferWidget::copyPatScanDb() {
    // Copies the PatScan.mdb file from the DEXA/APEX workstation and sets up a reference to the file.
    // Assumes that the Apex workstation is sharing the file

    if (m_patscanDbPath.isNull() || m_patscanDbPath.isEmpty())
        return false;

    const QFileInfo patscanFileInfo(m_patscanDbPath);
    if (!patscanFileInfo.exists())
        return false;

    if (!patscanFileInfo.isReadable())
        return false;

    const QFileInfo localPatScanFileInfo(QDir::current().absoluteFilePath(patscanFileInfo.fileName()));
    if (localPatScanFileInfo.exists()) {
        if (!QFile::remove(localPatScanFileInfo.absoluteFilePath()))
            return false;
    }

    if (!QFile::copy(m_patscanDbPath, localPatScanFileInfo.absoluteFilePath())) {
        qDebug() << "error copying patscan db from " << m_patscanDbPath << "to" << localPatScanFileInfo.absoluteFilePath();
        return false;
    }

    m_patscanDbFileInfo = localPatScanFileInfo;

    return true;
}


bool ApexTransferWidget::copyReferenceDb() {
    // Copies the reference.mdb file from the DEXA computer, assumes that the Apex workstation is sharing the file
    //
    const QFileInfo apexReferenceFileInfo(m_referenceDbPath);
    if (!apexReferenceFileInfo.exists())
        return false;

    if (!apexReferenceFileInfo.isReadable())
        return false;

    const QFileInfo localReferenceFileInfo(QDir::current().absoluteFilePath(apexReferenceFileInfo.fileName()));
    if (localReferenceFileInfo.exists()) {
        if (!QFile::remove(localReferenceFileInfo.absoluteFilePath())) {
            qDebug() << "could not remove existing local reference db at: " << m_referenceDbPath;
            return false;
        }
    }

    if (!QFile::copy(m_referenceDbPath, localReferenceFileInfo.absoluteFilePath())) {
        qDebug() << "error copying refscan db from " << m_referenceDbPath << "to" << localReferenceFileInfo.absoluteFilePath();
        return false;
    }

    m_referenceDbFileInfo = localReferenceFileInfo;

    return true;
}
