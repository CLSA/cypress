#include "session_manager.h"

Session::Session(QObject *parent)
    : QObject{parent}
{


}

void Session::start(Params& params)
{
    m_params = params;
}

void Session::end()
{

}

QString Session::getId()
{
    return m_params.m_uuid;
}

QString Session::getAnswerId()
{
    return m_params.m_answer_id;
}

QString Session::getBarcode()
{
    return m_params.m_barcode;
}

QString Session::getLanguage()
{
    return m_params.m_language;
}
