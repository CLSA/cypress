#include "windows_util.h"

#include <QProcess>
#include <QDebug>

bool WindowsUtil::isProcessRunning(const std::wstring &processName) {
    PROCESSENTRY32 entry;

    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (!snapshot) {
        return false;
    }

    if (Process32First(snapshot, &entry)) {
        do {
            if (std::wstring(entry.szExeFile) == processName) {
                CloseHandle(snapshot);
                return true;
            }
        } while(Process32Next(snapshot, &entry));
    }

    CloseHandle(snapshot);

    return false;
}

DWORD WindowsUtil::findProcessId(const std::wstring &processName)
{
    PROCESSENTRY32 processInfo;
    processInfo.dwSize = sizeof(processInfo);

    HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (processesSnapshot == INVALID_HANDLE_VALUE) {
        return 0;
    }

    Process32First(processesSnapshot, &processInfo);
    if (!processName.compare(processInfo.szExeFile)) {
        CloseHandle(processesSnapshot);
        return processInfo.th32ProcessID;
    }

    while (Process32Next(processesSnapshot, &processInfo)) {
        if (!processName.compare(processInfo.szExeFile)) {
        CloseHandle(processesSnapshot);
        return processInfo.th32ProcessID;
        }
    }

    CloseHandle(processesSnapshot);
    return 0;
}

void WindowsUtil::killProcessByName(const std::wstring &processName)
{
    DWORD pid = findProcessId(processName);
    if (pid != 0) {
        QProcess::execute(QStringLiteral("taskkill /F /PID %1").arg(pid));
    }
}
