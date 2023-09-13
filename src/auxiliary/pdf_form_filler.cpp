#include "pdf_form_filler.h"

#include <QTemporaryFile>
#include <QDesktopServices>
#include <QException>

PDFFormFiller::PDFFormFiller(QObject *parent)
    : QObject{parent}
{

}

QString PDFFormFiller::fillPDF(const QString& pdfTemplatePath, const QString& fdfTemplatePath, const QJsonObject& inputData, const QString& outputPath)
{
    // Read the FDF content from file
    QFile fdfTemplateFile(fdfTemplatePath);
    if (!fdfTemplateFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw QException();
    }

    QTextStream in(&fdfTemplateFile);
    QString content = in.readAll();
    fdfTemplateFile.close();

    for (const QString &key : inputData.keys()) {
        QJsonValue value = inputData.value(key);
        updateFieldValue(content, key, value.toString());
    }

    // Write the updated content to a temp FDF file
    QTemporaryFile tempFile;
    if (tempFile.open())
    {
        QTextStream out(&tempFile);
        out << content;
    }

    QProcess pdftkProcess;
    QStringList arguments;

    arguments << pdfTemplatePath << "fill_form" << tempFile.fileName() << "output" << outputPath;

    pdftkProcess.start("C:/Program Files (x86)/PDFtk Server/bin/pdftk.exe", arguments);
    pdftkProcess.waitForFinished();

    // Read the standard output if needed
    QByteArray stdoutData = pdftkProcess.readAllStandardOutput();
    QByteArray stderrData = pdftkProcess.readAllStandardError();

    tempFile.close();
    tempFile.remove();

    return outputPath;
}

void PDFFormFiller::updateFieldValue(QString& fileContents, const QString& fieldName, const QString& value)
{
    QRegularExpression re(QString("/V (?:\\(([^)]*)\\)|/)\\s+/T \\(%1\\)").arg(QRegularExpression::escape(fieldName)));
    QString replacement;

    replacement = QString("/V (%1) /T (%2)").arg(value).arg(fieldName); // For regular fields

    fileContents.replace(re, replacement);

    ////QRegularExpression re(QString("/V \\(([^)]*)\\)\\s+/T \\(%1\\)").arg(QRegularExpression::escape(fieldName)));
    //QRegularExpression re(QString("/V (?:\(([^)]*)\\)|\\/)\\s+/T \((%1)\\))").arg(QRegularExpression::escape(fieldName)));
    //QString replacement = QString("/V (%1) /T (%2)").arg(value).arg(fieldName);

    //fileContents.replace(re, replacement);
}
