#include "LogWorker.h"

#include <QFile>
#include <QTextStream>
#include <QThread>

void LogWorker::run() {
    QFile logData("C:/work/clsa/cypress/dcmtk-3.6.7-win64-chocolatey/bin/dcmrecv.exe.log");

    if (logData.open(QFile::ReadOnly))
    {
        QTextStream out(&logData);
        while (!isInterruptionRequested()) {
            QString newLine = out.readLine();
            if (!newLine.isNull())
            {
                emit updateLog(newLine);
            }
        }
        logData.close();
    }
}
