#include "windows_util.h"

#include <QProcess>

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
