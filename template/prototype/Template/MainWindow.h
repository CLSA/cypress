#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "ui_MainWindow.h"
#include "../../managers/TemplateManager.h"
#include <QStandardItemModel>

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
    QString inputFileName() { return m_inputFileName; }

    void setOutputFileName(const QString& name) { m_outputFileName = name; }
    QString outputFileName() { return m_outputFileName; }

    void setMode(const QString& mode) { m_mode = mode.toLower(); }
    QString mode() { return m_mode; }

    void setVerbose(const bool& verbose) { m_verbose = verbose; }
    bool isVerbose() { return m_verbose; }

public slots:
    void writeOutput();

protected:
    void closeEvent(QCloseEvent*) override;

private:
    void setupConnections();
    void initializeButtonState();
    void readInput();
    void populateBarcodeDisplay();

    Ui::MainWindow* ui;
    QString m_inputFileName;
    QString m_outputFileName;
    QString m_mode;
    bool m_verbose;

    QMap<QString, QVariant> m_inputData;
    QMap<QString, QVariant> m_outputData;

    TemplateManager m_manager;

    QStandardItemModel m_model;
};

#endif MAINWINDOW_H