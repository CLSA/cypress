#include "spirometer_measurement.h"
#include "auxiliary/Utilities.h"

#include <QDebug>
#include <QRandomGenerator>
#include <QDateTime>

const QStringList SpirometerMeasurement::parameterList = {"DataValue","Unit","PredictedValue","LLNormalValue"};

const q_stringMap SpirometerMeasurement::channelMap = {
    {"SamplingInterval","sampling_interval"},
    //{"SamplingValues","values"},
    {"TimeZeroOffset","time_zero_offset"},
    {"DefaultVTPlotDrawEndIdx","default_vt_plot_draw_end_index"}
};

const q_stringMap SpirometerMeasurement::resultMap = {
    {"AmbHumidity","ambient_humidity"},
    {"AmbPressure","ambient_pressure"},
    {"AmbTemp","ambient_temperature"},
    {"AmbTemp_Fahr","ambient_temperature_fahr"},
    {"BEV","bev"},
    {"BTPSex","btps_ex"},
    {"BTPSin","btps_in"},
    {"EOTV","eotv"},
    {"FEF10","fef_10"},
    {"FEF25","fef_25"},
    {"FEF2575","fef_25_75"},
    {"FEF2575_6","fef_25_75_6"},
    {"FEF2575_FVC","fef_25_75_fvc"},
    {"FEF40","fef_40"},
    {"FEF50","fef_50"},
    {"FEF50_FVC","fef_50_fvc"},
    {"FEF50_VCmax","fef_50_vcmax"},
    {"FEF60","fef_60"},
    {"FEF75","fef_75"},
    {"FEF7585","fef_75_85"},
    {"FEF80","fef_80"},
    {"FET","fet"},
    {"FET2575","fet_25_75"},
    {"FEV1","fev_1"},
    {"FEV1_FEV6","fev_1_fev_6"},
    {"FEV1_FVC","fev_1_fvc"},
    {"FEV1_VCext","fev_1_vcext"},
    {"FEV1_VCmax","fev_1_vcmax"},
    {"FEV3","fev_3"},
    {"FEV3_FVC","fev_3_fvc"},
    {"FEV3_VCmax","fev_3_vcmax"},
    {"FEV6","fev_6"},
    {"FEV_25","fev_25"},
    {"FEV_5","fev_5"},
    {"FEV_5_FVC","fev_5_fvc"},
    {"FEV_75","fev_75"},
    {"FEV_75_FEV6","fev_75_fev_6"},
    {"FEV_75_FVC","fev_75_fvc"},
    {"FEV_75_VCmax","fev_75_vcmax"},
    {"FVC","fvc"},
    {"MEF20","mef_20"},
    {"MEF25","mef_25"},
    {"MEF40","mef_40"},
    {"MEF50","mef_50"},
    {"MEF60","mef_60"},
    {"MEF75","mef_75"},
    {"MEF90","mef_90"},
    {"MMEF","mmef"},
    {"MTC1","mtc_1"},
    {"MTC2","mtc_2"},
    {"MTC3","mtc_3"},
    {"MTCR","mtc_r"},
    {"MsgNo","msg_no"},
    {"PEF","pef"},
    {"PEFT","peft"},
    {"PEF_L_Min","pef_l_min"},
    {"T0","t0"},
    {"VCext","vcext"},
    {"VCmax","vcmax"}
};

const q_stringMap SpirometerMeasurement::trialMap =
{
  {"Date","trial_date"},
  {"Number","trial_number"},
  {"Rank","trial_rank"},
  {"RankOriginal","trial_rank_original"},
  {"Accepted","trial_accepted"},
  {"AcceptedOriginal","trial_accepted_original"},
  {"ManualAmbientOverride","trial_manual_ambient_override"}
};

// two measurement types: one is the best values of the trials
// the other is a trial itself.
// best values: result map keys
// trial values: result, trial and channel map keys
//
bool SpirometerMeasurement::isValid() const
{
    bool okResult = true;
    QList<QString> list = resultMap.values();

    // for the FVC test type, these are always NaN even though
    // LL normal and predicted values are provided by default
    //
    list.removeOne("fev_1_vcext");
    list.removeOne("vcext");

    if(typeBestValues == m_type)
    {
        list.removeOne("default_vt_plot_draw_end_index");
    }

    foreach(const auto key, list)
    {
      if(!hasAttribute(key))
      {
        qDebug() << "trial or best values measurement missing results attribute"<<key;
        okResult = false;
        break;
      }
    }
    if(!okResult)
      return okResult;

    bool okChannel = true;
    bool okTrial = true;

    if(m_type == typeTrial)
    {
        // the channels have prefixes flow_ and volume_
        // only the volume channel has DefaultVTPlotDrawEndIdx
        //
      list = channelMap.values();
      QStringList channelKeys;
      foreach(const auto key, list)
      {
        if("default_vt_plot_draw_end_index" == key)
        {
          //channelKeys << QString("volume_%1").arg(key);
        }
        else
        {
          //channelKeys << QString("flow_%1").arg(key);
          //channelKeys << QString("volume_%1").arg(key);
        }
      }

      foreach(const auto key, channelKeys)
      {
        if(!hasAttribute(key))
        {
          qDebug() << "trial measurement missing channel attribute"<<key;
          okChannel = false;
          break;
        }
      }
      if(!okChannel)
        return okChannel;

      list = trialMap.values();
      foreach(const auto key, list)
      {
        if(!hasAttribute(key))
        {
            qDebug() << "trial measurement missing trial attribute"<<key;
          okTrial = false;
          break;
        }
      }
    }
    return okResult && okChannel && okTrial;
}

// TODO: generate terse output string
//
QString SpirometerMeasurement::toString() const
{
    QString output = "";
    return output;
}

QDebug operator<<(QDebug dbg, const SpirometerMeasurement& item)
{
    const QString measurementStr = item.toString();
    if(measurementStr.isEmpty())
        dbg.nospace() << "Spirometer Measurement()";
    else
        dbg.nospace() << "Spirometer Measurement(" << measurementStr << " ...)";
    return dbg.maybeSpace();
}
