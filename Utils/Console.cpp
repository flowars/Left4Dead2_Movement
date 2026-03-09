#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <string>
#include "Console.h"

void Console::Attach_Console()
{
    AllocConsole();
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);
}

void Console::Detach_Console()
{
    HWND consoleWindow = GetConsoleWindow();
    FreeConsole();
    if (consoleWindow != NULL) {
        PostMessage(consoleWindow, WM_CLOSE, 0, 0);
    }
}