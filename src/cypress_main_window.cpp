#include "cypress_main_window.h"
#include "cypress_application.h"

#include "server/sessions/easyone_connect_session.h"
#include "server/sessions/hearcon_session.h"
#include "server/sessions/vivid_iq_session.h"

#include "ui_cypress_main_window.h"

#include "server/sessions/audiometer_session.h"
#include "server/sessions/grip_strength_session.h"
#include "server/sessions/retinal_camera_session.h"
#include "server/sessions/gen_proxy_session.h"

#include "server/sessions/watch_bp_session.h"
#include "server/sessions/weigh_scale_session.h"
#include "server/sessions/mac5_session.h"
#include "server/sessions/dxa/dxa_hip_session.h"
#include "server/sessions/dxa/dxa_session.h"
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
    if (!CypressSettings::isDebugMode())
        return;

    enableDevices(true);
    enableDebugParticipant();
    enableDebugDevices();
}

QJsonObject CypressMainWindow::getDebugInputData()
{
    QString origin = CypressSettings::readSetting("debug/origin").toString();
    QString uid = CypressSettings::readSetting("debug/uid").toString();
    QString barcode = CypressSettings::readSetting("debug/barcode").toString();
    QString language = CypressSettings::readSetting("debug/language").toString();
    QString interviewer = CypressSettings::readSetting("debug/interviewer").toString();
    QString date_of_birth = CypressSettings::readSetting("debug/date_of_birth").toString();
    QString gender = CypressSettings::readSetting("debug/gender").toString();
    QString height = CypressSettings::readSetting("debug/height").toString();
    QString weight = CypressSettings::readSetting("debug/weight").toString();
    bool smoker = CypressSettings::readSetting("debug/smoker").toBool();

    QJsonObject inputData {
        {"language", 		language},
        {"barcode", 		barcode},
        {"uid",  			uid },
        {"interviewer", 	interviewer},
        {"smoker", 			smoker},
        {"gender", 			gender},
        {"height", 			height},
        {"weight", 			weight},
        {"dob", 	date_of_birth},
    };

    return inputData;
}

void CypressMainWindow::enableDevices(bool enabled)
{
    // 1

    // New
    ui->launchAudiometer->setEnabled(enabled);
    ui->launchBpm->setEnabled(enabled);
    ui->launchSpirometer->setEnabled(enabled);
    ui->launchEcg->setEnabled(enabled);
    ui->launchUltrasound->setEnabled(enabled);

    // Old
    ui->launchWeightScale->setEnabled(true);
    ui->launchChoiceReaction->setEnabled(enabled);
    ui->launchCDTT->setEnabled(enabled);

    ui->launchGenProxy->setEnabled(enabled);
    //ui->launchDxa1->setEnabled(true);
    //ui->launchDxa2->setEnabled(true);
    //ui->launchFrax->setEnabled(enabled);

    // 2
    //ui->launchGripStrength->setEnabled(enabled);
    //ui->launchRetinalCamera->setEnabled(enabled);
    //ui->launchTonometer->setEnabled(enabled);
}

void CypressMainWindow::enableDebugParticipant()
{
    QJsonObject iniInputData = getDebugInputData();

    ui->languageValue->addItems({ "en", "fr" });

    ui->uidValue->setText(iniInputData["uid"].toString());
    ui->languageValue->setCurrentIndex(iniInputData["language"] == "en" ? 0 : 1);
    ui->barcodeValue->setText(iniInputData["barcode"].toString());
    ui->weightValue->setText(iniInputData["weight"].toString());
    ui->heightValue->setText(iniInputData["height"].toString());
    ui->sexValue->setCurrentIndex(iniInputData["sex"] == "Female" ? 0 : 1);
    ui->dateOfBirthValue->setDate(QDate::fromString(iniInputData["dob"].toString()));

    ui->languageValue->setEnabled(true);
    ui->uidValue->setEnabled(true);
    ui->barcodeValue->setEnabled(true);
    ui->weightValue->setEnabled(true);
    ui->heightValue->setEnabled(true);
    ui->sexValue->setEnabled(true);
    ui->dateOfBirthValue->setEnabled(true);

    connect(ui->barcodeValue, &QLineEdit::textChanged, this, [=](const QString& barcodeValue) {
        CypressSettings::writeSetting("debug/barcode", barcodeValue);
    });

    connect(ui->uidValue, &QLineEdit::textChanged, this, [=](const QString& uidValue) {
        CypressSettings::writeSetting("debug/uid", uidValue);
    });

    connect(ui->languageValue, &QComboBox::currentTextChanged, this, [=](const QString& languageValue) {
        qDebug() << "language" << languageValue;
        CypressSettings::writeSetting("debug/language", languageValue);
    });

    connect(ui->weightValue, &QLineEdit::textChanged, this, [=](const QString& weightValue) {
        CypressSettings::writeSetting("debug/weight", weightValue);
    });

    connect(ui->heightValue, &QLineEdit::textChanged, this, [=](const QString& heightValue) {
        CypressSettings::writeSetting("debug/height", heightValue);
    });

    connect(ui->sexValue, &QComboBox::currentTextChanged, this, [=](const QString& sexValue) {
        CypressSettings::writeSetting("debug/sex", sexValue);
    });

    connect(ui->dateOfBirthValue, &QDateEdit::dateChanged, this, [=](const QDate& dateValue) {
        CypressSettings::writeSetting("debug/dob", dateValue.toString(Qt::DateFormat::ISODate));
    });
}

void CypressMainWindow::enableDebugDevices()
{
    connect(ui->launchBpm, &QPushButton::clicked, this, [=]() {
        QJsonObject inputData = getDebugInputData();
        QSharedPointer<WatchBPSession> session(new WatchBPSession(nullptr, inputData, inputData["origin"].toString()));
        Cypress::getInstance().requestSession(session);
    });

    connect(ui->launchGenProxy, &QPushButton::clicked, this, [=]() {
        QJsonObject inputData = getDebugInputData();
        QSharedPointer<GenProxySession> session(new GenProxySession(nullptr, inputData, inputData["origin"].toString()));
        Cypress::getInstance().requestSession(session);
    });

    connect(ui->launchWeightScale, &QPushButton::clicked, this, [=]() {
        QJsonObject inputData = getDebugInputData();
        QSharedPointer<WeighScaleSession> session(new WeighScaleSession(nullptr, inputData, inputData["origin"].toString()));
        Cypress::getInstance().requestSession(session);
    });

    connect(ui->launchSpirometer, &QPushButton::clicked, this, [=]() {
        QJsonObject inputData = getDebugInputData();
        //QSharedPointer<SpirometerSession> session(new SpirometerSession(nullptr, inputData, origin));
        QSharedPointer<EasyoneConnectSession> session(new EasyoneConnectSession(nullptr, inputData, inputData["origin"].toString()));
        Cypress::getInstance().requestSession(session);
    });

    connect(ui->launchDxa1, &QPushButton::clicked, this, [=]() {
        QJsonObject inputData = getDebugInputData();
        QSharedPointer<DxaHipSession> session(new DxaHipSession(nullptr, inputData, inputData["origin"].toString()));
        Cypress::getInstance().requestSession(session);
    });

    connect(ui->launchDxa2, &QPushButton::clicked, this, [=]() {
        QJsonObject inputData = getDebugInputData();
        QSharedPointer<DXASession> session(new DXASession(nullptr, inputData, inputData["origin"].toString()));
        Cypress::getInstance().requestSession(session);
    });

    connect(ui->launchUltrasound, &QPushButton::clicked, this, [=]() {
        QJsonObject inputData = getDebugInputData();
        QSharedPointer<VividIQSession> session(new VividIQSession(nullptr, inputData, inputData["origin"].toString()));
        Cypress::getInstance().requestSession(session);
    });

    connect(ui->launchEcg, &QPushButton::clicked, this, [=]() {
        QJsonObject inputData = getDebugInputData();
        QSharedPointer<Mac5Session> session(new Mac5Session(nullptr, inputData, inputData["origin"].toString()));
        Cypress::getInstance().requestSession(session);
    });

    connect(ui->launchFrax, &QPushButton::clicked, this, [=]() {
        QJsonObject inputData = getDebugInputData();
        QSharedPointer<FraxSession> session(new FraxSession(nullptr, inputData, inputData["origin"].toString()));
        Cypress::getInstance().requestSession(session);
    });

    connect(ui->launchAudiometer, &QPushButton::clicked, this, [=]() {
        QJsonObject inputData = getDebugInputData();
        QSharedPointer<HearconSession> session(new HearconSession(nullptr, inputData, inputData["origin"].toString()));
        Cypress::getInstance().requestSession(session);
    });

    connect(ui->launchGripStrength, &QPushButton::clicked, this, [=]() {
        QJsonObject inputData = getDebugInputData();
        QSharedPointer<GripStrengthSession> session(new GripStrengthSession(nullptr, inputData, inputData["origin"].toString()));
        Cypress::getInstance().requestSession(session);
    });

    connect(ui->launchRetinalCamera, &QPushButton::clicked, this, [=]() {
        QJsonObject inputData = getDebugInputData();
        QSharedPointer<RetinalCameraSession> session(new RetinalCameraSession(nullptr, inputData, inputData["origin"].toString()));
        Cypress::getInstance().requestSession(session);
    });

    connect(ui->launchChoiceReaction, &QPushButton::clicked, this, [=]() {
        QJsonObject inputData = getDebugInputData();
        QSharedPointer<ChoiceReactionSession> session(new ChoiceReactionSession(nullptr, inputData, inputData["origin"].toString()));
        Cypress::getInstance().requestSession(session);
    });

    connect(ui->launchCDTT, &QPushButton::clicked, this, [=]() {
        QJsonObject inputData = getDebugInputData();
        QSharedPointer<CDTTSession> session(new CDTTSession(nullptr, inputData, inputData["origin"].toString()));
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
