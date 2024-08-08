#include "hearing_measurement.h"

#include <QJsonObject>
#include <QDateTime>
#include <QDebug>
#include <QRegExp>

const q_stringMap HearingMeasurement::codeLookup = HearingMeasurement::initCodeLookup();
const q_stringMap HearingMeasurement::outcomeLookup = HearingMeasurement::initOutcomeLookup();
const QMap<int,QString> HearingMeasurement::frequencyLookup = HearingMeasurement::initFrequencyLookup();

 /*
  *    opal expects
  *
RES_TEST_ID text
RES_TEST_DATETIM datetime
RES_LAST_CALIBRATION_DATE datetime
RES_RIGHT_1KT integer
RES_RIGHT_500 integer
RES_RIGHT_1K integer
RES_RIGHT_2K integer
RES_RIGHT_3K integer
RES_RIGHT_4K integer
RES_RIGHT_6K integer
RES_RIGHT_8K integer
RES_LEFT_1KT integer
RES_LEFT_500 integer
RES_LEFT_1K integer
RES_LEFT_2K integer
RES_LEFT_3K integer
RES_LEFT_4K integer
RES_LEFT_6K integer
RES_LEFT_8K integer
RES_RIGHT_1KT_ERR tex
RES_RIGHT_500_ERR text
RES_RIGHT_1K_ERR text
RES_RIGHT_2K_ERR text
RES_RIGHT_3K_ERR text
RES_RIGHT_4K_ERR text
RES_RIGHT_6K_ERR text
RES_RIGHT_8K_ERR text
RES_LEFT_1KT_ERR text
RES_LEFT_500_ERR text
RES_LEFT_1K_ERR text
RES_LEFT_2K_ERR text
RES_LEFT_3K_ERR text
RES_LEFT_4K_ERR text
RES_LEFT_6K_ERR text
RES_LEFT_8K_ERR
  *
  *
  */

// a hearing threshold level can either be a valid integer
// value in dB at a specific frequency and side or it can be an error code combined with a recommended
// outcome
// the characteristics for a given measurement are:
// frequency label string
// threshold level integer (can be null but there must be an error code)
// side left or right string
// error code string  (default no error or null)
// error code label string (default null)
// outcome string (default null)
//
q_stringMap HearingMeasurement::initCodeLookup()
{
    q_stringMap map;
    map["AA"] = "NOT_TESTED";
    map["DD"] = "DELETED";
    map["EA"] = "CONTRALATERAL_RECORDED";
    map["EB"] = "BASELINE_SHIFT";
    map["EC"] = "ADJACENT_FREQ";
    map["ED"] = "OUT_OF_RANGE";
    map["EE"] = "NO_RESPONSE";
    map["EF"] = "NO_THRESHOLD";
    map["E1"] = "NO_RESPONSE_1K";
    map["E2"] = "NO_THRESHOLD_1K";
    map["E3"] = "VERIFY_FAILED_1K";
    map["E4"] = "HANDSWITCH_ERROR";
    map["E5"] = "RESPONSE_NO_TONE";
    map["E6"] = "NO_THRESHOLD_AGAIN";
    map["E7"] = "TOO_MANY_FAILURES";
    map["E8"] = "EQUIPMENT_ERROR";
    return map;
}

q_stringMap HearingMeasurement::initOutcomeLookup()
{
    q_stringMap map;
    map["NOT_TESTED"] = "RUN_TEST";
    map["DELETED"] = "NONE";
    map["CONTRALATERAL_RECORDED"] = "RERUN_TEST";
    map["BASELINE_SHIFT"] = "RERUN_TEST";
    map["ADJACENT_FREQ"] = "RERUN_TEST";
    map["OUT_OF_RANGE"] = "RERUN_TEST";
    map["NO_RESPONSE"] = "RERUN_TEST";
    map["NO_THRESHOLD"] = "RERUN_TEST";
    map["NO_RESPONSE_1K"] = "REINSTRUCT_SUBJECT";
    map["NO_THRESHOLD_1K"] = "REINSTRUCT_SUBJECT";
    map["VERIFY_FAILED_1K"] = "REINSTRUCT_SUBJECT";
    map["HANDSWITCH_ERROR"] = "REINSTRUCT_SUBJECT";
    map["RESPONSE_NO_TONE"] = "REINSTRUCT_SUBJECT";
    map["NO_THRESHOLD_AGAIN"] = "REINSTRUCT_SUBJECT";
    map["TOO_MANY_FAILURES"] = "REINSTRUCT_SUBJECT";
    map["EQUIPMENT_ERROR"] = "CONTACT_SERVICE";
    return map;
}

QMap<int,QString> HearingMeasurement::initFrequencyLookup()
{
    QMap<int,QString> map;
    map[0] = "1 kHz";
    map[1] = "500 Hz";
    map[2] = "1 kHz";
    map[3] = "2 kHz";
    map[4] = "3 kHz";
    map[5] = "4 kHz";
    map[6] = "6 kHz";
    map[7] = "8 kHz";
    return map;
}

HearingMeasurement::HearingMeasurement(const QString& side, const int& index, const QString& code) {
    fromCode(side,index,code);

    qDebug() << "HearingMeasurement";
    qDebug() << toJsonObject();
}


HearingMeasurement::HearingMeasurement(const QString& side, const QString& test, const int level, const bool pass) {
    setAttribute("side", side.toLower());
    setAttribute("test", test);
    setAttribute("level", level, "dB");
    setAttribute("pass", pass);
    setAttribute("outcome", "");
    setAttribute("error", "");
}

// index is the position 0-7 in the returned HTL string from
// the RA300 RS232 port data
//
void HearingMeasurement::fromCode(const QString& side, const int& index, const QString& code) {
    qDebug() << "HearingMeasurement::fromCode" << side << index << code;

    reset();

    if (frequencyLookup.contains(index)) {
        setAttribute("side", side.toLower());
        setAttribute("test", frequencyLookup[index]);

        if (codeLookup.contains(code)) {
            QString err = codeLookup[code];

            setAttribute("error", err);
            setAttribute("outcome", outcomeLookup[err]);
        }
        else {
            QRegExp r("\\d*");

            if (r.exactMatch(code)) {
               setAttribute("level", code.toInt(), "dB");
            }
        }
    }
}

bool HearingMeasurement::isValid() const {
    qInfo() << "HearingMeasurement::isValid";

    // side test: level (units) OR error (outcome)
    const bool hasRequiredFields = hasAttribute("side") && hasAttribute("test");
    const bool hasResultOrError = hasAttribute("level") || (hasAttribute("error") && hasAttribute("outcome"));

    const bool valid = hasRequiredFields && hasResultOrError;

    return valid;
}

QString HearingMeasurement::toString() const {
    QString str;

    str = QString("%1 %2").arg(getAttribute("side").toString(), getAttribute("test").toString());
    if (hasAttribute("level"))
        str = QString("%1 %2").arg(str,getAttribute("level").toString());
    else
        str = QString("%1 %2 %3").arg(str, getAttribute("error").toString(), getAttribute("outcome").toString());

    return str;
}

QDebug operator<<(QDebug dbg, const HearingMeasurement& item) {
    const QString str = item.toString();

    if(str.isEmpty())
        dbg.nospace() << "Hearing Measurement()";
    else
        dbg.nospace() << "Hearing Measurement(" << str << " ...)";

    return dbg.maybeSpace();
}
