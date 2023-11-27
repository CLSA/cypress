#include "cypress_main_window.h"
#include "ui_cypress_main_window.h"

#include "widgets/device_settings/vivid_i_settings_widget.h"
#include "widgets/device_settings/audiometer_settings_widget.h"
#include "widgets/device_settings/blood_pressure_settings_widget.h"
#include "widgets/device_settings/body_composition_settings_widget.h"
#include "widgets/device_settings/cdtt_settings_widget.h"
#include "widgets/device_settings/choice_reaction_settings_widget.h"
#include "widgets/device_settings/dexa_settings_widget.h"
#include "widgets/device_settings/ecg_settings_widget.h"
#include "widgets/device_settings/frax_settings_widget.h"
#include "widgets/device_settings/grip_strength_settings_widget.h"
#include "widgets/device_settings/retinal_camera_settings_widget.h"
//#include "widgets/device_settings/signature_pad_settings_widget.h"
#include "widgets/device_settings/spirometer_settings_widget.h"
#include "widgets/device_settings/tonometer_settings_widget.h"
#include "widgets/device_settings/weight_scale_settings_widget.h"

#include "server/sessions/bpm_session.h"
#include "dialogs/blood_pressure_dialog.h"


#include <QVBoxLayout>
#include <QDebug>

CypressMainWindow::CypressMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CypressMainWindow)
{
    ui->setupUi(this);

    connect(ui->launchBpm, &QPushButton::clicked, this, [=]() {
        QJsonObject inputData {
            {"language", "en"},
            {"barcode", "12345678"},
            {"interviewer", "Test"},
            };
        ;

        QSharedPointer<BPMSession> session(new BPMSession(nullptr, inputData));
        m_device_dialog.reset(new BloodPressureDialog(nullptr, session));

        m_device_dialog->run();
        m_device_dialog->show();
    });

    connect(ui->actionAudiometer_2, &QAction::triggered, this, [=]() {
        AudiometerSettingsWidget* audiometerSettings = new AudiometerSettingsWidget;
        this->setCentralWidget(audiometerSettings);
    });

    connect(ui->actionBlood_Pressure, &QAction::triggered, this, [=]() {
        BloodPressureSettingsWidget* bloodPressureSettings = new BloodPressureSettingsWidget;
        this->setCentralWidget(bloodPressureSettings);
    });

    connect(ui->actionBody_Comp, &QAction::triggered, this, [=]() {
        BodyCompositionSettingsWidget* bodyCompositionSettings = new BodyCompositionSettingsWidget;
        this->setCentralWidget(bodyCompositionSettings);
    });

    connect(ui->actionCDTT_2, &QAction::triggered, this, [=]() {
        CDTTSettingsWidget* cdttSettings = new CDTTSettingsWidget;
        this->setCentralWidget(cdttSettings);
    });

    connect(ui->actionCCB, &QAction::triggered, this, [=]() {
        ChoiceReactionSettingsWidget* choiceReactionSettings = new ChoiceReactionSettingsWidget;
        this->setCentralWidget(choiceReactionSettings);
    });

    connect(ui->actionDEXA, &QAction::triggered, this, [=]() {
        DexaSettingsWidget* dexaSettings = new DexaSettingsWidget;
        this->setCentralWidget(dexaSettings);
    });

    connect(ui->actionECG_2, &QAction::triggered, this, [=]() {
        ECGSettingsWidget* ecgSettings = new ECGSettingsWidget;
        this->setCentralWidget(ecgSettings);
    });

    connect(ui->actionFrax, &QAction::triggered, this, [=]() {
        FraxSettingsWidget* fraxSettings = new FraxSettingsWidget;
        this->setCentralWidget(fraxSettings);
    });

    connect(ui->actionGrip_Strength, &QAction::triggered, this, [=] {
        GripStrengthSettingsWidget* gripStrengthSettings = new GripStrengthSettingsWidget;
        this->setCentralWidget(gripStrengthSettings);
    });

    connect(ui->actionRetinal_Cam, &QAction::triggered, this, [=] {
        RetinalCameraSettingsWidget* retinalCameraSettings = new RetinalCameraSettingsWidget;
        this->setCentralWidget(retinalCameraSettings);
    });

    //connect(ui->actionSignature, &QAction::triggered, this, [=] {
    //    SignaturePadSettingsWidget*  signaturePadSettings = new SignaturePadSettingsWidget;
    //    this->setCentralWidget(signaturePadSettings);
    //});

    connect(ui->actionSpirometer, &QAction::triggered, this, [=] {
        SpirometerSettingsWidget* spirometerSettings = new SpirometerSettingsWidget;
        this->setCentralWidget(spirometerSettings);
    });

    connect(ui->actionTonometer, &QAction::triggered, this, [=] {
        TonometerSettingsWidget* tonometerSettings = new TonometerSettingsWidget;
        this->setCentralWidget(tonometerSettings);
    });

    connect(ui->actionVividI, &QAction::triggered, this, [=]() {
        VividISettingsWidget* vividISettings = new VividISettingsWidget;
        this->setCentralWidget(vividISettings);
    });

    connect(ui->actionWeigh_Scale, &QAction::triggered, this, [=] {
        WeightScaleSettingsWidget* weighScaleSettings = new WeightScaleSettingsWidget;
        this->setCentralWidget(weighScaleSettings);
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
