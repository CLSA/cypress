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

QString PDFFormFiller::dumpDataFields(const QString& pdfFilePath) {
    qDebug() << "dumpDataFields" << pdfFilePath;

    if (!QFileInfo::exists(pdfFilePath)) {
        qDebug() << pdfFilePath << "does not exist";
        throw QException();
    }

    if (!QFileInfo(pdfFilePath).isFile()) {
        qDebug() << pdfFilePath << "is not a file";
        throw QException();
    }

    if (!QFileInfo(pdfFilePath).isReadable()) {
        qDebug() << pdfFilePath << "is not readable";
        throw QException();
    }

    QProcess pdftkProcess;
    QStringList arguments;
    arguments << QDir::toNativeSeparators(pdfFilePath)
              << "dump_data_fields";

    qDebug() << "pdftk: " << arguments;

    pdftkProcess.start("C:/Program Files (x86)/PDFtk Server/bin/pdftk.exe", arguments);
    pdftkProcess.waitForFinished();

    QString output { QString(pdftkProcess.readAllStandardOutput()) };

    return output;
}

QJsonObject PDFFormFiller::parseDataFields(const QString& dataFields) {
    QJsonObject output {};

    QRegExp fieldNameRegex("^FieldName:(.*)$");
    QRegExp fieldValueRegex("^FieldValue:(.*)$");

    const QStringList lines = dataFields.split("\n");
    QString fieldName {};
    QString fieldValue {};

    foreach (const QString& line, lines) {
        // Found a field name
        if (fieldNameRegex.exactMatch(line.trimmed())) {
            QStringList captures = fieldNameRegex.capturedTexts();
            if (captures.length() > 1) {
                // Clean up the field name ex. User.fullName\t => user_fullname
                fieldName = captures[1].trimmed();
                fieldName.replace(".", "_");
            } else {
                qDebug() << "regex did not capture value for" << line;
            }
        }

        // Found the corresponding field value for last visited field name
        if (fieldValueRegex.exactMatch(line.trimmed()) && !fieldName.isNull() && !fieldName.isEmpty()) {
            QStringList captures = fieldValueRegex.capturedTexts();
            if (captures.length() > 1) {
                fieldValue = captures[1].trimmed();
                fieldValue.replace("\\", "");

                if (fieldValue == "Yes")
                    output[fieldName] = true;
                else if (fieldValue == "No")
                    output[fieldName] = false;
                else if (fieldValue.isNull() || fieldValue.isEmpty())
                    output[fieldName] = QJsonValue(); // null
                else
                    output[fieldName] = fieldValue;

                // key/value pair was found, reset and continue
                qDebug() << fieldName << output[fieldName];
                fieldName = "";
                fieldValue = "";
            } else {
                qDebug() << "regex did not capture value for" << line;
            }
        }
    }

    return output;
}

void PDFFormFiller::updateFieldValue(QString& fileContents, const QString& fieldName, const QString& value)
{
    static QRegularExpression re(QString("/V (?:\\(([^)]*)\\)|/)\\s+/T \\(%1\\)").arg(QRegularExpression::escape(fieldName)));
    QString replacement;

    replacement = QString("/V (%1) /T (%2)").arg(value).arg(fieldName); // For regular fields
    fileContents.replace(re, replacement);
}
