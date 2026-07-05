from PySide6.QtSql import QSqlDatabase, QSqlQuery

from pathlib import Path


class ApexDatabaseReader:
    def __init__(self, patscan_path: Path, reference_path: Path):
        self.patscan_db = QSqlDatabase.addDatabase("QODBC", "patscan")
        self.patscan_db.setDatabaseName(
            (
                f"Driver={{Microsoft Access Driver (*.mdb, *.accdb)}};"
                f"DBQ={patscan_path.resolve()};"
            )
        )

        self.reference_db = QSqlDatabase.addDatabase("QODBC", "reference")
        self.reference_db.setDatabaseName(
            (
                f"Driver={{Microsoft Access Driver (*.mdb, *.accdb)}};"
                f"DBQ={reference_path.resolve()};"
            )
        )

        if not self.patscan_db.open():
            print("could not open database")
            print(f"Error: {self.patscan_db.lastError().text()}")
            return

        if not self.reference_db.open():
            print("could not open database")
            print(f"Error: {self.patscan_db.lastError().text()}")

    def get_patient_info(self, site_visit_id: str):
        query = QSqlQuery(self.patscan_db)

        query.prepare(
            "SELECT PATIENT_KEY, BIRTHDATE, SEX, ETHNICITY, WEIGHT, HEIGHT "
            "FROM PATIENT "
            "WHERE IDENTIFIER1 = :site_visit_id"
        )

        query.bindValue(":site_visit_id", site_visit_id)
        if not query.exec():
            raise Exception("patient scan query failed")

        record = {}
        while query.next():
            record["PATIENT_KEY"] = str(query.value("PATIENT_KEY"))
            record["BIRTHDATE"] = str(query.value("BIRTHDATE"))
            record["SEX"] = str(query.value("SEX"))
            record["ETHNICITY"] = str(query.value("ETHNICITY"))
            record["WEIGHT"] = str(query.value("WEIGHT"))
            record["HEIGHT"] = str(query.value("HEIGHT"))

        return record

    def get_scan_analysis(self, patient_data: dict):
        query = QSqlQuery(self.patscan_db)

        # query.prepare(
        #     'SELECT SCANID, SCAN_MODE, SCAN_DATE ' \
        #     'FROM ScanAnalysis ' \
        #     'WHERE PATIENT_KEY = :patientKey AND SCAN_TYPE = :scanType')
        query.prepare(
            "SELECT SCANID, SCAN_TYPE, SCAN_MODE, SCAN_DATE "
            "FROM ScanAnalysis "
            "WHERE PATIENT_KEY = :patientKey"
        )

        query.bindValue(":patientKey", patient_data["PATIENT_KEY"])
        # query.bindValue(':scanType', scan_type)

        if not query.exec():
            return

        scans = []
        while query.next():
            scan_id = query.value("SCANID")
            scan_type = query.value("SCAN_TYPE")
            scan_mode = query.value("SCAN_MODE")
            scan_date = str(query.value("SCAN_DATE"))

            scans.append(
                {
                    "SCANID": scan_id,
                    "SCAN_TYPE": scan_type,
                    "SCAN_MODE": scan_mode,
                    "SCAN_DATE": scan_date,
                }
            )

        return scans


def get_hip_scan_data(db: QSqlDatabase, patient_key: str, scan_id: str):
    query = QSqlQuery(db)

    query.prepare(
        "SELECT * FROM Hip WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId"
    )
    query.bindValue(":patientKey", patient_key)
    query.bindValue(":scanId", scan_id)

    if not query.exec():
        raise Exception("hip query failed")

    if not query.first():
        print(f"error: could not find a result for {patient_key} {scan_id}")
        return


def get_whole_body_scan_data(db: QSqlDatabase, patient_key: str, scan_id):
    query = QSqlQuery(db)

    query.prepare(
        "SELECT * FROM Wbody WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId"
    )
    query.bindValue(":patientKey", patient_key)
    query.bindValue(":scanId", scan_id)

    if not query.exec():
        raise Exception("error: wbody query failed")

    if not query.first():
        print(
            f"error: could not find wbody scan data for patient: {patient_key}, scan_id: {scan_id}"
        )
        return

    # process variables

    query.prepare(
        "SELECT * FROM WbodyComposition WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId"
    )
    query.bindValue(":patientKey", patient_key)
    query.bindValue(":scanId", scan_id)

    if not query.exec():
        raise Exception("error: wbodycomp query failed")

    if not query.first():
        print(
            f"error: could not find wbodycomp scan data for patient: {patient_key}, scan_id: {scan_id}"
        )
        return

    # process variables


def get_forearm_scan_data(db: QSqlDatabase, patient_key: str, scan_id: str):
    query = QSqlQuery(db)

    query.prepare(
        "SELECT * FROM Forearm WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId"
    )
    query.bindValue(":patientKey", patient_key)
    query.bindValue(":scanId", scan_id)

    if not query.exec():
        raise Exception("forearm query failed")

    if not query.first():
        print(
            f"error: could not find forearm scan data for patient: {patient_key}, scan_id: {scan_id}"
        )
        return

    # process variables


def get_spine_scan_data(db: QSqlDatabase, patient_key: str, scan_id: str):
    query = QSqlQuery(db)

    query.prepare(
        "SELECT * FROM Spine WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId"
    )
    query.bindValue(":patientKey", patient_key)
    query.bindValue(":scanId", scan_id)

    if not query.exec():
        raise Exception("forearm query failed")

    if not query.first():
        print(
            f"error: could not find spine scan data for patient: {patient_key}, scan_id: {scan_id}"
        )
        return
