#ifndef PDFFORMFILLER_H
#define PDFFORMFILLER_H

#include <QObject>
#include <QJsonObject>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QProcess>

class PDFFormFiller : public QObject
{
    Q_OBJECT
public:
    explicit PDFFormFiller(QObject *parent = nullptr);

    // Returns the output file path
    QString fillPDF(const QString& pdfTemplatePath, const QString& fdfTemplatePath, const QJsonObject& inputData, const QString& outputPath);

    // Returns a string of all data fields and their values
    QString dumpDataFields(const QString& pdfFilename);

    // Parses data fields into key/value pairs
    QJsonObject parseDataFields(const QString& dataFields);

private:
    void updateFieldValue(QString& fileContents, const QString& fieldName, const QString& value);

};

#endif // PDFFORMFILLER_H
