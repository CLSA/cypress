#ifndef BODY_COMPOSITION_MANAGER_H
#define BODY_COMPOSITION_MANAGER_H

#include "managers/serial_port/serial_port_manager.h"
#include "data/body_composition/tests/body_composition_test.h"

#include <QQueue>

/*!
 * \class BodyCompositionManager
 * @brief The BodyCompositionManager class
 *
 * Concrete child class implementation of a RS232 serial
 * device manager.  This class facilitates connection to a
 * Tanita BF-350 body composition analyzer for read/write instructions
 * and test data retrieval.
 *
 * \sa ManagerBase, SerialPortManager, AudiometerManager
 *
 */

class BodyCompositionManager : public SerialPortManager
{
    Q_OBJECT

public:
    explicit BodyCompositionManager(QSharedPointer<CypressSession> session);

    static bool isAvailable();
    static bool isInstalled();

    static QMap<QString,QByteArray> initDefaultLUT();
    static QMap<QByteArray,QString> initCommandLUT();
    static QMap<QByteArray,QString> initIncorrectResponseLUT();
    static QMap<QByteArray,QString> initConfirmationLUT();

    // update inputs from the UI
    //
    void updateInputData(const QVariantMap&);

    static int AGE_MIN;
    static int AGE_MAX;
    static int HEIGHT_MIN_METRIC;
    static int HEIGHT_MAX_METRIC;
    static double HEIGHT_MIN_IMPERIAL;
    static double HEIGHT_MAX_IMPERIAL;
    static QByteArray END_CODE;

signals:
   // ready to receive the input map
   //
   void canInput();

   void canConfirm();

   void error(const QString&);

   // notify the UI to fill in the age, height and gender
   //
   void notifyGenderInput(const QString&);
   void notifyAgeInput(const QString&);
   void notifyHeightInput(const QString&);

public slots:

    // connect to the serial port
    // opens the serial port with required parametere (baud rate etc.)
    // connects the port readyRead signal to the readDevice slot
    // emits canMeasure signal if the port is open
    //
    void connectDevice() override;

    // reset all device settings (removed inputs)
    //
    void resetDevice();

    // confirm the input after setInputs
    //
    void confirmSettings();

    // retrieve a measurement from the device
    //
    void measure() override;

    // implementation of final clean up of device after disconnecting and all
    // data has been retrieved and processed by any upstream classes
    //
    void finish() override;

private slots:

    // retrieve data from the scale over RS232
    // emits canFinish signal if the test data is valid
    // Read is based on the last written code
    //
    void readDevice() override;

    void writeDevice() override;

private:
    static QMap<QString,QByteArray> defaultLUT;
    static QMap<QByteArray,QString> commandLUT;
    static QMap<QByteArray,QString> incorrectLUT;
    static QMap<QByteArray,QString> confirmLUT;

    bool hasEndCode(const QByteArray&) const;

    void processResponse();

    BodyCompositionTest m_test;

    // Reset the session
    bool clearData() override;

    // Set up device
    bool setUp() override;

    // Clean up the device for next time
    bool cleanUp() override;

    void clearQueue();

    QVector<QByteArray> m_cache;
    QQueue<QByteArray> m_queue;
};

#endif // BODY_COMPOSITION_MANAGER_H
