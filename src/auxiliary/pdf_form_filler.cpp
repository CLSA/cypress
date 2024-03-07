#include "pdf_form_filler.h"

#include <QTemporaryFile>
#include <QDesktopServices>
#include <QException>
#include <QDir>

PDFFormFiller::PDFFormFiller(QObject *parent)
    : QObject{parent}
{

}

QString PDFFormFiller::fillPDF(const QString& pdfTemplatePath, const QString& fdfTemplatePath, const QJsonObject& inputData, const QString& outputPath) {
    qDebug()
        << "FillPDF:"
        << fdfTemplatePath
        << pdfTemplatePath
        << inputData
        << outputPath;

    // Read the FDF content from file
    QFile fdfTemplateFile(fdfTemplatePath);
    if (!fdfTemplateFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "can't open fdf file";
        throw QException();
    }

    QTextStream in(&fdfTemplateFile);
    QString content = in.readAll();
    fdfTemplateFile.close();

    foreach (const QString &key, inputData.keys()) {
        QJsonValue value = inputData.value(key);
        updateFieldValue(content, key, value.toString());
    }

    ////// Write the updated content to a temp FDF file
    QTemporaryFile tempFile;
    if (tempFile.open()) {
        QTextStream out(&tempFile);
        out << content;
    }

    QProcess pdftkProcess;
    QStringList arguments;
    arguments << QDir::toNativeSeparators(pdfTemplatePath)
              << "fill_form"
              << QDir::toNativeSeparators(tempFile.fileName())
              << "output"
              << QDir::toNativeSeparators(outputPath);

    qDebug() << "start pdftk..";
    pdftkProcess.start("C:/Program Files (x86)/PDFtk Server/bin/pdftk.exe", arguments);
    pdftkProcess.waitForFinished();

    qDebug() << "pdftk finished..";
    tempFile.close();
    tempFile.remove();
    qDebug() << "remove temp files..";

    return outputPath;
}

void PDFFormFiller::updateFieldValue(QString& fileContents, const QString& fieldName, const QString& value)
{
    static QRegularExpression re(QString("/V (?:\\(([^)]*)\\)|/)\\s+/T \\(%1\\)").arg(QRegularExpression::escape(fieldName)));
    QString replacement;

    replacement = QString("/V (%1) /T (%2)").arg(value).arg(fieldName); // For regular fields
    fileContents.replace(re, replacement);
}
