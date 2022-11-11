#include <QProcess>
#include <QSqlDatabase>
#include <QFileInfo>

#include "RetinalCameraManager.h"


RetinalCameraManager::RetinalCameraManager(QObject *parent)
    : ManagerBase{parent}
{

}

// load and save device, paths and other constant settings to .ini
//
void RetinalCameraManager::loadSettings(const QSettings &)
{
    if (m_verbose)
        qDebug() << "RetinalCameraManager::loadSettings";
}

void RetinalCameraManager::saveSettings(QSettings*) const
{
    if (m_verbose)
        qDebug() << "RetinalCameraManager::saveSettings";
}


void RetinalCameraManager::setInputData(const QVariantMap &)
{
    if (m_verbose)
        qDebug() << "RetinalCameraManager::setInputData";
}

// collate test results and device and other meta data
// for the main application to write to .json
//
QJsonObject RetinalCameraManager::toJsonObject() const
{
    if (m_verbose)
        qDebug() << "RetinalCameraManager::toJsonObject";

    return QJsonObject();
}

// build a model from test and measurement data for UI display
// of the results
//
void RetinalCameraManager::initializeModel()
{
    if (m_verbose)
        qDebug() << "RetinalCameraManager::initializeModel";
}

// update the model whenever the test data changes
//
void RetinalCameraManager::updateModel()
{
    if (m_verbose)
        qDebug() << "RetinalCameraManager::updateModel";
}

// subclasses call methods after main initialization just prior
// to running (eg., emit dataChanged signal)
//

// org.obiba.onyx.jade.instrument.topcon.imagenetr4lite.workDir=C:/Program\ Files/TOPCON/IMAGEnet\ R4/
// org.obiba.onyx.jade.instrument.topcon.imagenetr4lite.executable=imagenet.exe
//
// org.obiba.onyx.jade.instrument.topcon.imagenetr4lite.driver=net.sourceforge.jtds.jdbc.Driver
// org.obiba.onyx.jade.instrument.topcon.imagenetr4lite.url=jdbc:jtds:sqlserver://localhost:1433/IMAGEnet_R4
// org.obiba.onyx.jade.instrument.topcon.imagenetr4lite.username=
// org.obiba.onyx.jade.instrument.topcon.imagenetr4lite.password=
void RetinalCameraManager::start()
{
    if (m_verbose)
        qDebug() << "RetinalCameraManager::start";

    QString program = "C:\\Users\\antho\\OneDrive\\Desktop\\qt-unified-windows-x64-4.4.1-online.exe";
    QFileInfo info(program);
    QStringList arguments;


    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC3");
    QString connectString = "Driver={SQL Server Native Client 11.0};";                     // Driver is now {SQL Server Native Client 11.0}
    connectString.append("Server=localhost:1433");   // Hostname,SQL-Server Instance
    connectString.append("Database=IMAGEnet_R4;");  // Schema
    connectString.append("Uid=;");           // User
    connectString.append("Pwd=;");           // Pass

    db.setDatabaseName(connectString);

    bool ok = db.open();

    if (!ok)
    {
        qDebug() << "could not open db";
    }

    if (!info.exists())
    {
       qDebug() << "Program does not exist: " << program;
       return;
    }

    if (!info.isExecutable())
    {
        qDebug() << "Program is not executable: " << program;
        return;
    }

    // clean last participant data from database
    //
    if (m_verbose)
        qDebug() << "cleaning database..";


    m_process.setProgram(program);
    m_process.start();

    bool started = m_process.waitForStarted();
    if (!started)
    {
        if (m_verbose)
            qDebug() << "program running..";
        return;
    }

    // enter participant data to database
    //
    if (m_verbose)
        qDebug() << "entering new participant data..";

    // launch process
    //
    if (m_verbose)
        qDebug() << "launching process..";







    // wait for process to finish
    //
    if (m_verbose)
        qDebug() << "started, now waiting..";

    m_process.waitForFinished(360000);

    if (m_verbose)
        qDebug() << "process finished..";

    // process data for left and right eye
    //
    if (m_verbose)
        qDebug() << "reading left eye data..";


    if (m_verbose)
        qDebug() << "reading right eye data..";

    // send data to server
    //
    if (m_verbose)
        qDebug() << "submit..";
}

// SLOT
// actual measure will only execute if the barcode has been
// verified.  Subclasses must reimplement accordingly.
//
void RetinalCameraManager::measure()
{
    if (m_verbose)
        qDebug() << "RetinalCameraManager::measure";
}

// SLOT
// subclasses call methods just prior to main close event
//
void RetinalCameraManager::finish()
{
    if (m_verbose)
        qDebug() << "RetinalCameraManager::finish";
}

// SLOT
// Context dependent clear test data and possibly device data (eg., serial port info)
void RetinalCameraManager::clearData()
{
    if (m_verbose)
        qDebug() << "RetinalCameraManager::clearData";
}
