#include "cimt_vivid_i_dialog.h"
#include "ui_cimt_vivid_i_dialog.h"
#include "managers/ultrasound/vividi_manager.h"
#include "cypress_session.h"

#include <QHostInfo>
#include <QTableWidget>
#include <QHeaderView>


CimtVividiDialog::CimtVividiDialog(const CypressSession& session, QWidget *parent) :
    DialogBase(parent, session),
    ui(new Ui::CimtVividiDialog)
{
    ui->setupUi(this);
    ui->dicomWidget->setDicomLabels("CLSADICOM", QHostInfo::localHostName(), "9001");


    QString patientId = tr("Patient ID");
    QString studyInstanceId = tr("Study Instance ID");
    QString fileNumber = tr("File Number");
    QString laterality = tr("Laterality");

    QTableWidget* tableWidget = ui->filesReceived->getTable();

    tableWidget->setColumnCount(4);
    tableWidget->setHorizontalHeaderLabels(QStringList() << patientId << studyInstanceId << fileNumber << laterality);

    // Add some data (optional)
    for (int row = 0; row < 5; ++row) {
        tableWidget->insertRow(row);
        for (int col = 0; col < 4; ++col) {
            QTableWidgetItem *item = new QTableWidgetItem(QString("Row %1, Col %2").arg(row + 1).arg(col + 1));
            tableWidget->setItem(row, col, item);
        }
    }

    m_manager.reset(new VividiManager(session));
}

CimtVividiDialog::~CimtVividiDialog()
{
    delete ui;
}
