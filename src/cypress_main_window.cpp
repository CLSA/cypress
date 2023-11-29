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
#include "widgets/device_settings/spirometer_settings_widget.h"
#include "widgets/device_settings/tonometer_settings_widget.h"
#include "widgets/device_settings/weight_scale_settings_widget.h"

#include "server/sessions/bpm_session.h"
#include "server/sessions/weigh_scale_session.h"
#include "server/sessions/ecg_session.h"
#include "server/sessions/dxa/dxa_hip_session.h"
#include "server/sessions/dxa/dxa_session.h"
#include "server/sessions/spirometer_session.h"
#include "server/sessions/ultrasound_session.h"
#include "server/sessions/frax_session.h"
#include "server/sessions/cdtt_session.h"
#include "server/sessions/choice_reaction_session.h"

#include "dialogs/blood_pressure_dialog.h"
#include "dialogs/ecg_dialog.h"
#include "dialogs/cimt_vivid_i_dialog.h"
#include "dialogs/spirometer_dialog.h"
#include "dialogs/weigh_scale_dialog.h"
#include "dialogs/dxa_dialog.h"
#include "dialogs/dxa_hip_dialog.h"

#include "dialogs/audiometer_dialog.h"
#include "dialogs/frax_dialog.h"
#include "dialogs/retinal_camera_dialog.h"
#include "dialogs/grip_strength_dialog.h"

#include "dialogs/cdtt_dialog.h"
#include "dialogs/choice_reaction_dialog.h"
#include "dialogs/frax_dialog.h"


#include <QVBoxLayout>
#include <QDebug>

CypressMainWindow::CypressMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CypressMainWindow)
{
    ui->setupUi(this);

    // Go to main page
    connect(ui->actionTest_Locally, &QAction::triggered, this, [=]() {
        this->setCentralWidget(this);
    });

    // Room 1
    connect(ui->launchBpm, &QPushButton::clicked, this, [=]() {
        QJsonObject inputData {
            {"language", "en"},
            {"barcode", "12345678"},
            {"interviewer", "CLSA"},
        };

        QSharedPointer<BPMSession> session(new BPMSession(nullptr, inputData));

        m_device_dialog.reset(new BloodPressureDialog(nullptr, session));
        m_device_dialog->run();
        m_device_dialog->show();
    });
    connect(ui->launchWeightScale, &QPushButton::clicked, this, [=]() {
       QJsonObject inputData {
            {"language", "en"},
            {"barcode", "12345678"},
            {"interviewer", "CLSA"},
        };

        QSharedPointer<WeighScaleSession> session(new WeighScaleSession(nullptr, inputData));

        m_device_dialog.reset(new WeighScaleDialog(nullptr, session));
        m_device_dialog->run();
        m_device_dialog->show();
    });
    connect(ui->launchSpirometer, &QPushButton::clicked, this, [=]() {
       QJsonObject inputData {
            {"language", "en"},
            {"barcode", "12345678"},
            {"interviewer", "Test"},
        };

        QSharedPointer<SpirometerSession> session(new SpirometerSession(nullptr, inputData));

        m_device_dialog.reset(new SpirometerDialog(nullptr, session));
        m_device_dialog->run();
        m_device_dialog->show();
    });
    connect(ui->launchDxa1, &QPushButton::clicked, this, [=]() {
        QJsonObject inputData {
            {"language", "en"},
            {"barcode", "12345678"},
            {"interviewer", "Test"},
        };

        QSharedPointer<DxaHipSession> session(new DxaHipSession(nullptr, inputData));

        m_device_dialog.reset(new DxaHipDialog(nullptr, session));
        m_device_dialog->run();
        m_device_dialog->show();
    });
    connect(ui->launchDxa2, &QPushButton::clicked, this, [=]() {
        QJsonObject inputData {
            {"language", "en"},
            {"barcode", "12345678"},
            {"interviewer", "Test"},
        };

        QSharedPointer<DXASession> session(new DXASession(nullptr, inputData));

        m_device_dialog.reset(new DXADialog(nullptr, session));
        m_device_dialog->run();
        m_device_dialog->show();
    });
    connect(ui->launchUltrasound, &QPushButton::clicked, this, [=]() {
        QJsonObject inputData {
            {"language", "en"},
            {"barcode", "12345678"},
            {"interviewer", "Test"},
        };

        QSharedPointer<UltrasoundSession> session(new UltrasoundSession(nullptr, inputData));

        m_device_dialog.reset(new CimtVividiDialog(nullptr, session));
        m_device_dialog->run();
        m_device_dialog->show();
    });
    connect(ui->launchEcg, &QPushButton::clicked, this, [=]() {
        QJsonObject inputData {
            {"language", "en"},
            {"barcode", "12345678"},
            {"interviewer", "Test"},
        };

        QSharedPointer<ECGSession> session(new ECGSession(nullptr, inputData));
        m_device_dialog.reset(new EcgDialog(nullptr, session));
        m_device_dialog->run();
        m_device_dialog->show();
    });
    connect(ui->launchFrax, &QPushButton::clicked, this, [=]() {
        QJsonObject inputData {
            {"language", "en"},
            {"barcode", "12345678"},
            {"interviewer", "Test"},
        };

        QSharedPointer<FraxSession> session(new FraxSession(nullptr, inputData));
        m_device_dialog.reset(new FraxDialog(nullptr, session));
        m_device_dialog->run();
        m_device_dialog->show();
    });

    // DCS Room 2
    connect(ui->launchAudiometer, &QPushButton::clicked, this, [=]() {
        QJsonObject inputData {
            {"language", "en"},
            {"barcode", "12345678"},
            {"interviewer", "Test"},
        };

        QSharedPointer<AudiometerSession> session(new AudiometerSession(nullptr, inputData));
        m_device_dialog.reset(new AudiometerDialog(nullptr, session));
        m_device_dialog->run();
        m_device_dialog->show();
    });

    // DCS Room 3
    connect(ui->launchGripStrength, &QPushButton::clicked, this, [=]() {
        QJsonObject inputData {
            {"language", "en"},
            {"barcode", "12345678"},
            {"interviewer", "Test"},
        };

        QSharedPointer<GripStrengthSession> session(new GripStrengthSession(nullptr, inputData));
        m_device_dialog.reset(new GripStrengthDialog(nullptr, session));
        m_device_dialog->run();
        m_device_dialog->show();
    });
    connect(ui->launchRetinalCamera, &QPushButton::clicked, this, [=]() {
        QJsonObject inputData {
            {"language", "en"},
            {"barcode", "12345678"},
            {"interviewer", "Test"},
        };

        QSharedPointer<RetinalCameraSession> session(new RetinalCameraSession(nullptr, inputData));
        m_device_dialog.reset(new RetinalCameraDialog(nullptr, session));
        m_device_dialog->run();
        m_device_dialog->show();
    });
    connect(ui->launchChoiceReaction, &QPushButton::clicked, this, [=]() {
        QJsonObject inputData {
            {"language", "en"},
            {"barcode", "12345678"},
            {"interviewer", "Test"},
        };

        QSharedPointer<ChoiceReactionSession> session(new ChoiceReactionSession(nullptr, inputData));
        m_device_dialog.reset(new ChoiceReactionDialog(nullptr, session));
        m_device_dialog->run();
        m_device_dialog->show();
    });
    connect(ui->launchCDTT, &QPushButton::clicked, this, [=]() {
        QJsonObject inputData {
            {"language", "en"},
            {"barcode", "12345678"},
            {"interviewer", "Test"},
        };

        QSharedPointer<CDTTSession> session(new CDTTSession(nullptr, inputData));

        m_device_dialog.reset(new CDTTDialog(nullptr, session));
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
