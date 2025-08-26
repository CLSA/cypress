#include "grip_strength_dialog.h"
#include "managers/grip_strength/grip_strength_manager.h"

#include <QCloseEvent>
#include <QDebug>
#include <QMessageBox>

GripStrengthDialog::GripStrengthDialog(QWidget *parent, QSharedPointer<GripStrengthSession> session)
    : DialogBase(parent, session)
    , ui(new Ui::GripStrengthDialog)
{
    ui->setupUi(this);

    this->setWindowTitle("Grip Strength");
    this->setWindowFlags(Qt::WindowFullscreenButtonHint);

    m_manager.reset(new GripStrengthManager(session));
    ui->measurementTable->hideManualEntry();
    ui->measurementTable->disableFinishButton();
    ui->measurementTable->disableMeasureButton();

    QSharedPointer<GripStrengthManager> manager = qSharedPointerCast<GripStrengthManager>(m_manager);
    ui->testInfoWidget->setSessionInformation(*session);

    QList<TableColumn> columns;
    columns << TableColumn("rep1", "Rep 1", new TextDelegate("", QRegExp(), false));
    columns << TableColumn("rep2", "Rep 2", new TextDelegate("", QRegExp(), false));
    columns << TableColumn("rep3", "Rep 3", new TextDelegate("", QRegExp(), false));

    // started
    connect(manager.get(), &GripStrengthManager::started, ui->measurementTable, [=]() {
        ui->measurementTable->enableMeasureButton();
        ui->measurementTable->initializeModel(columns);
    });

    // request measure
    connect(ui->measurementTable, &MeasurementTable::measure, manager.get(), &GripStrengthManager::measure);

    // data changed
    connect(manager.get(), &GripStrengthManager::dataChanged, this, [=](QSharedPointer<TestBase> test) {
        QString avg = QString::number(test->getMetaData("average").toDouble(), 'f', 2);
        QString max = QString::number(test->getMetaData("maximum").toDouble(), 'f', 2);
        QString cv = QString::number(test->getMetaData("cv").toDouble(), 'f', 2);

        ui->averageValue->setText(avg);
        ui->maximumValue->setText(max);
        ui->cvValue->setText(cv);

        ui->measurementTable->handleTestUpdate(test);
    });


    connect(manager.get(), &GripStrengthManager::cannotFinish, ui->measurementTable, [=]() {
        ui->measurementTable->disableFinishButton();
    });
    connect(manager.get(), &GripStrengthManager::canFinish, ui->measurementTable, [=]() {
        ui->measurementTable->enableFinishButton();
    });

    connect(ui->measurementTable, &MeasurementTable::finish, manager.get(), [=]() {
        ui->measurementTable->disableFinishButton();
        QApplication::processEvents();
        manager->finish();
    });

    connect(manager.get(), &GripStrengthManager::success, this, &GripStrengthDialog::success);
    connect(manager.get(), &GripStrengthManager::error, this, &GripStrengthDialog::error);
}

GripStrengthDialog::~GripStrengthDialog()
{
    delete ui;
}
