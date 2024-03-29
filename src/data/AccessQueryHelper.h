#ifndef ACCESSQUERYHELPER_H
#define ACCESSQUERYHELPER_H

#include <QObject>

/*!
 * \class AccessQueryHelper
 * \brief Helper class to query MS Access mdb files
 *
 * MS Access files can be queried over ODBC driver
 * using the basic SELECT, DELETE and INSERT
 * SQL statements.  The available operations are
 * dependent on participant gender, birth date and 8 digit interview barcode.
 * The returned QVariant from processQuery method must be interpreted
 * within the context of the current operation.
 *
 * CAVEAT: the schema is assumed to be that of the Reichert Tonometer.
 * The implementation herein should be refactored for other schema.
 *
 * \sa TonometerManager, TonometerTest
 *
 */

QT_FORWARD_DECLARE_CLASS(QSqlDatabase)

class AccessQueryHelper
{
public:

    enum Operation {
        Count,  // get the number of Patients records matching the participant
        CountMeasures,  // get the number of Patients records with Measures matching the participant
        Delete, // clear the db of all Patients records matching the participant
        DeleteMeasures, // clear the db of all Measures records matching the participant
        Insert, // intsert the participant into the Patients table
        Results // get all Measures for the participant
    };

    AccessQueryHelper();

    // header key with remaining row elements interpreted as data.
    //
    void setOperation(const AccessQueryHelper::Operation&);

    // Build and execute QSqlQuery from the provided participant identification keys
    // and query the provided QSqlDatabase (MS Access file).  The database connection must be
    // established and verified before calling.
    //
    QVariant processQuery(const QVariantMap&, const QSqlDatabase&);

    // Set the keys for labelling the data and verifying against record fields
    // returned from the Results operation query.
    //
    void setHeader(const QStringList&);

private:
  Operation m_operation;
  QStringList m_header;

};

#endif // ACCESSQUERYHELPER_H
