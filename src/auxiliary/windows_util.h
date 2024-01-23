#ifndef WINDOWSUTIL_H
#define WINDOWSUTIL_H

#include <string>

#include <Windows.h>
#include <TlHelp32.h>

class WindowsUtil
{
public:
    static DWORD findProcessId(const std::wstring &processName);
    static void killProcessByName(const std::wstring &processName);
};

#endif // WINDOWSUTIL_H
