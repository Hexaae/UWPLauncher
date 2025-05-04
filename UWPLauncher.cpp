#include <windows.h>
#include <tlhelp32.h>
#include <string>
#include <iostream>

DWORD FindProcessByName(const std::string& processName) {
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap == INVALID_HANDLE_VALUE) return 0;

    PROCESSENTRY32 pe32 = {0};
    pe32.dwSize = sizeof(PROCESSENTRY32);
    
    if (Process32First(hSnap, &pe32)) {
        do {
            if (processName == pe32.szExeFile) {
                DWORD processID = pe32.th32ProcessID;
                CloseHandle(hSnap);
                return processID;
            }
        } while (Process32Next(hSnap, &pe32));
    }

    CloseHandle(hSnap);
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        MessageBoxA(NULL, "Usage: launchUWP.exe <FullPackageFamilyName> [ProcessName]", "Error", MB_OK | MB_ICONERROR);  // If specified ProcessName, it won't immediately exit
        return 1;
    }

    std::string packageFamilyName = argv[1];
    std::string command = "shell:AppsFolder\\" + packageFamilyName;

    SHELLEXECUTEINFO shExecInfo = {0};
    shExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
    shExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
    shExecInfo.lpVerb = "open";
    shExecInfo.lpFile = "explorer.exe";
    shExecInfo.lpParameters = command.c_str();
    shExecInfo.nShow = SW_SHOWNORMAL;

    if (ShellExecuteEx(&shExecInfo)) {
        if (argc >= 3) {
            std::string processName = argv[2];
            DWORD processID;
            while ((processID = FindProcessByName(processName)) == 0) {
                Sleep(15); // Wait process to start
            }

            HANDLE hProcess = OpenProcess(SYNCHRONIZE, FALSE, processID);
            if (hProcess) {
                WaitForSingleObject(hProcess, INFINITE);
                CloseHandle(hProcess);
            }
        }
    } else {
        MessageBoxA(NULL, "Failed to launch application.", "Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    return 0;
}
