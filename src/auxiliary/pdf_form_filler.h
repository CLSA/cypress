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

private:
    void updateFieldValue(QString& fileContents, const QString& fieldName, const QString& value);

};

#endif // PDFFORMFILLER_H
