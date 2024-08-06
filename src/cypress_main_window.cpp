#include "cypress_main_window.h"
#include "cypress_application.h"

#include "server/sessions/easyone_connect_session.h"
#include "server/sessions/hearcon_session.h"
#include "ui_cypress_main_window.h"

#include "server/sessions/audiometer_session.h"
#include "server/sessions/grip_strength_session.h"
#include "server/sessions/retinal_camera_session.h"

#include "server/sessions/bpm_session.h"
#include "server/sessions/watch_bp_session.h"
#include "server/sessions/weigh_scale_session.h"
#include "server/sessions/ecg_session.h"
#include "server/sessions/dxa/dxa_hip_session.h"
#include "server/sessions/dxa/dxa_session.h"
#include "server/sessions/spirometer_session.h"
#include "server/sessions/ultrasound_session.h"
#include "server/sessions/frax_session.h"
#include "server/sessions/cdtt_session.h"
#include "server/sessions/choice_reaction_session.h"

#include <QVBoxLayout>
#include <QDebug>

CypressMainWindow::CypressMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CypressMainWindow)
{
    ui->setupUi(this);

    qDebug() << "CypressMainWindow";

    if (!CypressSettings::isDebugMode())
        return;

    const QString origin = CypressSettings::readSetting("debug/origin").toString();
    const QString barcode = CypressSettings::readSetting("debug/barcode").toString();
    const QString language = CypressSettings::readSetting("debug/language").toString();
    const QString interviewer = CypressSettings::readSetting("debug/interviewer").toString();
    const QString date_of_birth = CypressSettings::readSetting("debug/date_of_birth").toString();
    const QString gender = CypressSettings::readSetting("debug/gender").toString();
    const QString height = CypressSettings::readSetting("debug/height").toString();
    const QString weight = CypressSettings::readSetting("debug/weight").toString();
    const bool smoker = CypressSettings::readSetting("debug/smoker").toBool();

    const QJsonObject inputData{
        {"language", 		language},
        {"barcode", 		barcode},
        {"interviewer", 	interviewer},
        {"smoker", 			smoker},
        {"gender", 			gender},
        {"height", 			height},
        {"weight", 			weight},
        {"date_of_birth", 	date_of_birth},
    };

    ui->launchAudiometer->setEnabled(true);
    //ui->launchWeightScale->setEnabled(true);
    ui->launchSpirometer->setEnabled(true);
    //ui->launchDxa1->setEnabled(true);
    //ui->launchDxa2->setEnabled(true);
    //ui->launchFrax->setEnabled(true);
    //ui->launchUltrasound->setEnabled(true);
    //ui->launchEcg->setEnabled(true);
    //ui->launchGripStrength->setEnabled(true);
    //ui->launchRetinalCamera->setEnabled(true);
    //ui->launchChoiceReaction->setEnabled(true);
    //ui->launchCDTT->setEnabled(true);
    ui->launchBpm->setEnabled(true);

    connect(ui->launchBpm, &QPushButton::clicked, this, [=]() {
        //QSharedPointer<BPMSession> session(new BPMSession(nullptr, inputData, origin));
        QSharedPointer<WatchBPSession> session(new WatchBPSession(nullptr, inputData, origin));
        Cypress::getInstance().requestSession(session);
    });

    connect(ui->launchWeightScale, &QPushButton::clicked, this, [=]() {
        QSharedPointer<WeighScaleSession> session(new WeighScaleSession(nullptr, inputData, origin));
        Cypress::getInstance().requestSession(session);
    });

    connect(ui->launchSpirometer, &QPushButton::clicked, this, [=]() {
        //QSharedPointer<SpirometerSession> session(new SpirometerSession(nullptr, inputData, origin));
        QSharedPointer<EasyoneConnectSession> session(new EasyoneConnectSession(nullptr, inputData, origin));
        Cypress::getInstance().requestSession(session);
    });

    connect(ui->launchDxa1, &QPushButton::clicked, this, [=]() {
        QSharedPointer<DxaHipSession> session(new DxaHipSession(nullptr, inputData, origin));
        Cypress::getInstance().requestSession(session);
    });

    connect(ui->launchDxa2, &QPushButton::clicked, this, [=]() {
        QSharedPointer<DXASession> session(new DXASession(nullptr, inputData, origin));
        Cypress::getInstance().requestSession(session);
    });

    connect(ui->launchUltrasound, &QPushButton::clicked, this, [=]() {
        QSharedPointer<UltrasoundSession> session(new UltrasoundSession(nullptr, inputData, origin));
        Cypress::getInstance().requestSession(session);
    });

    connect(ui->launchEcg, &QPushButton::clicked, this, [=]() {
        QSharedPointer<ECGSession> session(new ECGSession(nullptr, inputData, origin));
        Cypress::getInstance().requestSession(session);
    });

    connect(ui->launchFrax, &QPushButton::clicked, this, [=]() {
        QSharedPointer<FraxSession> session(new FraxSession(nullptr, inputData, origin));
        Cypress::getInstance().requestSession(session);
    });

    connect(ui->launchAudiometer, &QPushButton::clicked, this, [=]() {
        QSharedPointer<HearconSession> session(new HearconSession(nullptr, inputData, origin));
        Cypress::getInstance().requestSession(session);
    });

    connect(ui->launchGripStrength, &QPushButton::clicked, this, [=]() {
        QSharedPointer<GripStrengthSession> session(new GripStrengthSession(nullptr, inputData, origin));
        Cypress::getInstance().requestSession(session);
    });

    connect(ui->launchRetinalCamera, &QPushButton::clicked, this, [=]() {
        QSharedPointer<RetinalCameraSession> session(new RetinalCameraSession(nullptr, inputData, origin));
        Cypress::getInstance().requestSession(session);
    });

    connect(ui->launchChoiceReaction, &QPushButton::clicked, this, [=]() {
        QSharedPointer<ChoiceReactionSession> session(new ChoiceReactionSession(nullptr, inputData, origin));
        Cypress::getInstance().requestSession(session);
    });

    connect(ui->launchCDTT, &QPushButton::clicked, this, [=]() {
        QSharedPointer<CDTTSession> session(new CDTTSession(nullptr, inputData, origin));
        Cypress::getInstance().requestSession(session);
    });
}

CypressMainWindow::~CypressMainWindow()
{
    delete ui;
}

void CypressMainWindow::closeEvent(QCloseEvent* event)
{
    hide();
    event->ignore();
}
