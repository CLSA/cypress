from PySide6.QtSql import QSqlDatabase, QSqlQuery

from pathlib import Path

def check_online(path: Path):
    return False

def copy_patscan_db(from_path: Path, to_path: Path):
    return False

def copy_reference_db(from_path: Path, to_path: Path):
    return False


def get_patient_scan(database: QSqlDatabase, participant_id: str):
    query = QSqlQuery(database)

    query.prepare('SELECT PATIENT_KEY, BIRTHDATE, ETHNICITY, WEIGHT, HEIGHT ' \
    'FROM PATIENT ' \
    'WHERE IDENTIFIER1 = :participantId')

    query.bindValue(':participantId', participant_id)
    if not query.exec():
        raise Exception("patient scan query failed")

    record = {}
    while query.next():
        record['PATIENT_KEY'] = query.value("PATIENT_KEY")
        record['BIRTHDATE']   = query.value("BIRTHDATE")
        record['SEX']         = query.value("SEX")
        record['ETHNICITY']   = query.value('ETHNICITY')
        record['WEIGHT']      = query.value('WEIGHT')
        record['HEIGHT']      = query.value('HEIGHT')

    return record


def get_scan_analysis(database: QSqlDatabase, patient_data: dict, scan_type: str):
    query = QSqlQuery(database)

    query.prepare(
        'SELECT SCANID, SCAN_MODE, SCAN_DATE ' \
        'FROM ScanAnalysis ' \
        'WHERE PATIENT_KEY = :patientKey AND SCAN_TYPE = :scanType')

    query.bindValue(':patientKey', patient_data['patient_key'])
    query.bindValue(':scanType', scan_type)


def get_hip_scan_data(database: QSqlDatabase, patient_key: str, scan_id: str):
    query = QSqlQuery(database)

    query.prepare('SELECT * FROM Hip WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId')
    query.bindValue(':patientKey', patient_key)
    query.bindValue(':scanId', scan_id)

    if not query.exec():
        raise Exception("hip query failed")

    if not query.first():
        print(f'error: could not find a result for {patient_key} {scan_id}')
        return


def get_whole_body_scan_data(database: QSqlDatabase, patient_key: str, scan_id):
    query = QSqlQuery(database)

    query.prepare('SELECT * FROM Wbody WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId')
    query.bindValue(':patientKey', patient_key)
    query.bindValue(':scanId', scan_id)

    if not query.exec():
        raise Exception("error: wbody query failed")

    if not query.first():
        print(f'error: could not find wbody scan data for patient: {patient_key}, scan_id: {scan_id}')
        return

    # process variables

    query.prepare('SELECT * FROM WbodyComposition WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId')
    query.bindValue(':patientKey', patient_key)
    query.bindValue(':scanId', scan_id)

    if not query.exec():
        raise Exception("error: wbodycomp query failed")

    if not query.first():
        print(f'error: could not find wbodycomp scan data for patient: {patient_key}, scan_id: {scan_id}')
        return

    # process variables

def get_forearm_scan_data(database: QSqlDatabase, patient_key: str, scan_id: str):
    query = QSqlQuery(database)

    query.prepare('SELECT * FROM Forearm WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId')
    query.bindValue(':patientKey', patient_key)
    query.bindValue(':scanId', scan_id)

    if not query.exec():
        raise Exception("forearm query failed")

    if not query.first():
        print(f'error: could not find forearm scan data for patient: {patient_key}, scan_id: {scan_id}')
        return

    # process variables


def get_spine_scan_data(database: QSqlDatabase, patient_key: str, scan_id: str):
    query = QSqlQuery(database)

    query.prepare('SELECT * FROM Spine WHERE PATIENT_KEY = :patientKey AND SCANID = :scanId')
    query.bindValue(':patientKey', patient_key)
    query.bindValue(':scanId', scan_id)

    if not query.exec():
        raise Exception("forearm query failed")

    if not query.first():
        print(f'error: could not find spine scan data for patient: {patient_key}, scan_id: {scan_id}')
        return

