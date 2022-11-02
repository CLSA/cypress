#include "LogWorker.h"

#include <QFile>
#include <QTextStream>
#include <QThread>

void LogWorker::run() {
    //QFile logData("C:/work/clsa/cypress/dcmtk-3.6.7-win64-chocolatey/bin/dcmrecv.exe.log");

    //if (logData.open(QFile::ReadOnly))
    //{
    QTextStream out(stdout);
    while (!isInterruptionRequested()) {
        QString newLine = out.readAll();
        if (!newLine.isNull())
        {
            emit updateLog(newLine);
        }
        sleep(1); // s
    }
    //logData.close();
    //}
}
