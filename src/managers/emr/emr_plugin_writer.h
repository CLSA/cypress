#ifndef EMR_PLUGIN_WRITER_H
#define EMR_PLUGIN_WRITER_H

#include <QJsonObject>

QT_FORWARD_DECLARE_CLASS(QXmlStreamWriter)

class EMRPluginWriter
{
public:
    EMRPluginWriter() = default;

    void setInputData(const QVariantMap&);

    void write(const QString&, const QString& patientId = "ONYX") const;

private:

    QVariantMap m_input;

    void addHeader(QXmlStreamWriter&) const;
    void addCommand(QXmlStreamWriter&) const;
    void addParameter(QXmlStreamWriter&, const QString&, const QString&) const;
    void addPatients(QXmlStreamWriter&, const QString& patientId = "ONYX") const;
    void addPatientDataAtPresent(QXmlStreamWriter&) const;
};

#endif // EMR_PLUGIN_WRITER_H
