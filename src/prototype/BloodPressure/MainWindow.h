#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "../../managers/BloodPressureManager.h"
#include "../../auxiliary/Constants.h"
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QDialog
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);
    ~MainWindow();

    // This method internally calls readInput
    //
    void initialize();

    // Call after initialize, launch the application and run
    // the device
    //
    void run();

    void setInputFileName(const QString& name) { m_inputFileName = name; }
    QString getInputFileName() const { return m_inputFileName; }

    void setOutputFileName(const QString& name) { m_outputFileName = name; }
    QString getOutputFileName() const { return m_outputFileName; }

    void setRunMode(const Constants::RunMode& getRunMode) { m_mode = getRunMode; }
    Constants::RunMode getRunMode() const { return m_mode; }

    void setVerbose(const bool& verbose) { m_verbose = verbose; }
    bool isVerbose() { return m_verbose; }

public slots:
    void writeOutput();

protected:
    void closeEvent(QCloseEvent*) override;

private:
    void readInput();
    void initializeModel();
    void initializeConnections();

    Ui::MainWindow* ui;
    QString m_inputFileName;
    QString m_outputFileName;
    Constants::RunMode m_mode;
    bool m_verbose;

    QVariantMap m_inputData;
    BloodPressureManager m_manager;
};

#endif // MAINWINDOW_H
