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
#include "server/sessions/oct_session.h"

#include "managers/general_proxy_form/general_proxy_manager.h"
#include "managers/weigh_scale/weigh_scale_manager.h"
#include "managers/blood_pressure/watch_bp_manager.h"
#include "managers/audiometer/hearcon_manager.h"
#include "managers/ecg/mac5_manager.h"
#include "managers/cdtt/cdtt_manager.h"
#include "managers/choice_reaction/choice_reaction_manager.h"
#include "managers/spirometer/easyone_manager.h"
#include "managers/dxa/dxa_hip_manager.h"
#include "managers/dxa/dxa_manager.h"
#include "managers/ultrasound/vivid_iq_manager.h"
#include "managers/retinal_camera/oct_manager.h"
#include "managers/tonometer/ora_manager.h"
#include "managers/grip_strength/grip_strength_manager.h"

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
    // Follow Up 4
    ui->f4GenProxy->setEnabled(!GeneralProxyManager::config.hasErrors());

    ui->f4Weight->setEnabled(!WeighScaleManager::config.hasErrors());
    ui->f4WatchBP->setEnabled(!WatchBPManager::config.hasErrors());
    ui->f4EasyoneConnect->setEnabled(!EasyoneConnectManager::config.hasErrors());
    ui->f4MAC5->setEnabled(!Mac5Manager::config.hasErrors());
    ui->f4ECHO->setEnabled(!VividIQManager::config.hasErrors());
    ui->f4DXA1->setEnabled(!DxaHipManager::config.hasErrors());
    ui->f4DXA2->setEnabled(!DXAManager::config.hasErrors());
    ui->f4FRAX->setEnabled(!GripStrengthManager::config.hasErrors());

    ui->f4Hearcon->setEnabled(!HearconManager::config.hasErrors());
    ui->f4CDTT->setEnabled(!CDTTManager::config.hasErrors());
    ui->f4ChoiceReaction->setEnabled(!ChoiceReactionManager::config.hasErrors());

    ui->f4GripStrength->setEnabled(!GripStrengthManager::config.hasErrors());
    ui->f4Tonometer->setEnabled(!ORAManager::config.hasErrors());
    ui->f4OCTLeft->setEnabled(!OCTManager::config.hasErrors());
    ui->f4OCTRight->setEnabled(!OCTManager::config.hasErrors());

    // Follow Up 3
    ui->f3Weight->setEnabled(true);
    ui->f3ChoiceReaction->setEnabled(enabled);
    ui->f3CDTT->setEnabled(enabled);

    ui->f3DXA1->setEnabled(true);
    ui->f3DXA2->setEnabled(true);
    ui->f3FRAX->setEnabled(enabled);

    ui->f3GripStrength->setEnabled(enabled);
    ui->f3RetinalLeft->setEnabled(enabled);
    ui->f3RetinalRight->setEnabled(enabled);
    ui->f3Tonometer->setEnabled(enabled);
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
    connect(ui->f4WatchBP, &QPushButton::clicked, this, [=]() {
        QJsonObject inputData = getDebugInputData();
        QSharedPointer<WatchBPSession> session(new WatchBPSession(nullptr, inputData, inputData["origin"].toString()));
        Cypress::getInstance().requestSession(session);
    });
    connect(ui->f4GenProxy, &QPushButton::clicked, this, [=]() {
        QJsonObject inputData = getDebugInputData();
        QSharedPointer<GenProxySession> session(new GenProxySession(nullptr, inputData, inputData["origin"].toString()));
        Cypress::getInstance().requestSession(session);
    });
    connect(ui->f4Weight, &QPushButton::clicked, this, [=]() {
        QJsonObject inputData = getDebugInputData();
        QSharedPointer<WeighScaleSession> session(new WeighScaleSession(nullptr, inputData, inputData["origin"].toString()));
        Cypress::getInstance().requestSession(session);
    });
    connect(ui->f4EasyoneConnect, &QPushButton::clicked, this, [=]() {
        QJsonObject inputData = getDebugInputData();
        QSharedPointer<EasyoneConnectSession> session(new EasyoneConnectSession(nullptr, inputData, inputData["origin"].toString()));
        Cypress::getInstance().requestSession(session);
    });
    connect(ui->f4DXA1, &QPushButton::clicked, this, [=]() {
        QJsonObject inputData = getDebugInputData();
        QSharedPointer<DxaHipSession> session(new DxaHipSession(nullptr, inputData, inputData["origin"].toString()));
        Cypress::getInstance().requestSession(session);
    });
    connect(ui->f4DXA2, &QPushButton::clicked, this, [=]() {
        QJsonObject inputData = getDebugInputData();
        QSharedPointer<DXASession> session(new DXASession(nullptr, inputData, inputData["origin"].toString()));
        Cypress::getInstance().requestSession(session);
    });
    connect(ui->f4ECHO, &QPushButton::clicked, this, [=]() {
        QJsonObject inputData = getDebugInputData();
        QSharedPointer<VividIQSession> session(new VividIQSession(nullptr, inputData, inputData["origin"].toString()));
        Cypress::getInstance().requestSession(session);
    });
    connect(ui->f4MAC5, &QPushButton::clicked, this, [=]() {
        QJsonObject inputData = getDebugInputData();
        QSharedPointer<Mac5Session> session(new Mac5Session(nullptr, inputData, inputData["origin"].toString()));
        Cypress::getInstance().requestSession(session);
    });
    connect(ui->f4FRAX, &QPushButton::clicked, this, [=]() {
        QJsonObject inputData = getDebugInputData();
        QSharedPointer<FraxSession> session(new FraxSession(nullptr, inputData, inputData["origin"].toString()));
        Cypress::getInstance().requestSession(session);
    });
    connect(ui->f4Hearcon, &QPushButton::clicked, this, [=]() {
        QJsonObject inputData = getDebugInputData();
        QSharedPointer<HearconSession> session(new HearconSession(nullptr, inputData, inputData["origin"].toString()));
        Cypress::getInstance().requestSession(session);
    });
    connect(ui->f4GripStrength, &QPushButton::clicked, this, [=]() {
        QJsonObject inputData = getDebugInputData();
        QSharedPointer<GripStrengthSession> session(new GripStrengthSession(nullptr, inputData, inputData["origin"].toString()));
        Cypress::getInstance().requestSession(session);
    });
    connect(ui->f4ChoiceReaction, &QPushButton::clicked, this, [=]() {
        QJsonObject inputData = getDebugInputData();
        QSharedPointer<ChoiceReactionSession> session(new ChoiceReactionSession(nullptr, inputData, inputData["origin"].toString()));
        Cypress::getInstance().requestSession(session);
    });
    connect(ui->f4CDTT, &QPushButton::clicked, this, [=]() {
        QJsonObject inputData = getDebugInputData();
        QSharedPointer<CDTTSession> session(new CDTTSession(nullptr, inputData, inputData["origin"].toString()));
        Cypress::getInstance().requestSession(session);
    });
    connect(ui->f4OCTLeft, &QPushButton::clicked, this, [=]() {
        QJsonObject inputData = getDebugInputData();
        QSharedPointer<OCTSession> session(new OCTSession(nullptr, inputData, inputData["origin"].toString(), OCTSession::Left));
        Cypress::getInstance().requestSession(session);
    });
    connect(ui->f4OCTRight, &QPushButton::clicked, this, [=]() {
        QJsonObject inputData = getDebugInputData();
        QSharedPointer<OCTSession> session(new OCTSession(nullptr, inputData, inputData["origin"].toString(), OCTSession::Right));
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
