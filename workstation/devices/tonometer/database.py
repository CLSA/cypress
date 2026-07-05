"""
 // connect to database
    m_database = QSqlDatabase::addDatabase("QODBC");
    m_database.setDatabaseName("Driver={Microsoft Access Driver (*.mdb)};DBQ=" + QDir::toNativeSeparators(m_databasePath));


    if (!m_database.open()) {
        qCritical() << "ORAManager::start - could not open database";
        return false;
    }

    QSqlQuery query(m_database);

    query.prepare("INSERT INTO Patients ( Name, BirthDate, Sex, GroupID, ID, RaceID ) VALUES ( :name, :birthDate, :sex, :groupId, :id, :raceId )");
    query.bindValue(":name",      m_session->getBarcode() + ",CLSA");
    query.bindValue(":birthDate", m_session->getInputData()["dob"].toString());
    query.bindValue(":sex",	      m_session->getInputData()["sex"].toString().startsWith("m", Qt::CaseSensitivity::CaseInsensitive));
    query.bindValue(":groupId",	  2);
    query.bindValue(":id", 	      m_session->getBarcode().toInt());
    query.bindValue(":raceId",    1);

    if (!query.exec()) {
        qCritical() << "Database error:" << m_database.lastError().text();
        return false;
    }

    m_database.close();

    qInfo() << "TonometerManager::extractMeasures";

    if (!m_database.isOpen()) {
        if (!m_database.open()) {
            throw QException();
        }
    }

    QSqlQuery query(m_database);
    query.prepare("SELECT PatientID from Patients where ID = :id");
    query.bindValue(":id", m_session->getBarcode().toInt());

    if (!query.exec()) {
        qWarning() << "Database error:" << m_database.lastError().text();
    }

    query.prepare("SELECT * from Measures where Eye = :eye ORDER BY MeasureDate desc");
    query.bindValue(":eye", QString(eye));

    QVariantMap resultMap;
    if (!query.exec()) {
        qWarning() << "Database error:" << m_database.lastError().text();
        return resultMap;
    }

    while (query.next()) {
        for (int i = 0; i < query.record().count(); ++i) {
            resultMap.insert(query.record().fieldName(i), query.value(i));
        }
    }

    return resultMap;

"""

from pathlib import Path

from PySide6.QtSql import QSqlDatabase, QSqlQuery

import logging

from retinal_camera.settings import DEVICE_NAME

logger = logging.getLogger(DEVICE_NAME)


class TonometerDatabase:
    def __init__(self, db_name: str):
        logger.debug(f"TonometerDatabase::__init__ - {db_name}")

        if not db_name:
            raise ValueError("db_name must exist")

        self.db = QSqlDatabase.addDatabase("QODBC")
        self.db.setDatabaseName(db_name)

        if not self.db.open():
            logger.error("database failed to open")
            raise Exception()

    def restore_database(self):
        pass

    def insert_participant(self, person_uid: str, first_name: str, last_name: str):
        pass

    def read_results(self):
        pass
