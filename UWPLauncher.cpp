#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <string>

DWORD FindProcessByName(const std::string& processName) {
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap == INVALID_HANDLE_VALUE) return 0;

    PROCESSENTRY32 pe32 = {0};
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hSnap, &pe32)) {
        do {
            if (processName == pe32.szExeFile) {
                DWORD pid = pe32.th32ProcessID;
                CloseHandle(hSnap);
                return pid;
            }
        } while (Process32Next(hSnap, &pe32));
    }

    CloseHandle(hSnap);
    return 0;
}

bool IsUWPPath(const std::string& path) {
    return path.find("WindowsApps") != std::string::npos;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        MessageBoxA(NULL,
            "Usage: UWPLauncher.exe <FullPackageFamilyName> [ProcessName] [optional parameters...]",
            "Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    std::string packageFamily = argv[1];
    std::string processToMonitor;
    int paramStartIndex = 2;

    if (argc >= 3 && std::string(argv[2]).find(".exe") != std::string::npos) {
        processToMonitor = argv[2];
        paramStartIndex = 3;
    }

    std::string launchCommand = "shell:AppsFolder\\" + packageFamily;
    for (int i = paramStartIndex; i < argc; ++i) {
        launchCommand += " ";
        launchCommand += argv[i];
    }

    SHELLEXECUTEINFOA execInfo = { 0 };
    execInfo.cbSize = sizeof(SHELLEXECUTEINFOA);
    execInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
    execInfo.lpVerb = "open";
    execInfo.lpFile = "explorer.exe";
    execInfo.lpParameters = launchCommand.c_str();
    execInfo.nShow = SW_SHOWNORMAL;

    if (!ShellExecuteExA(&execInfo)) {
        MessageBoxA(NULL, "Error: can't launch the app.", "Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    if (!processToMonitor.empty()) {
        DWORD pid = 0;
		int waitedMs = 0;
		const int timeoutMs = 30000;
		
		while ((pid = FindProcessByName(processToMonitor)) == 0) {
		    Sleep(30);
		    waitedMs += 30;
		    if (waitedMs >= timeoutMs) {
		        MessageBoxA(NULL,
		            "Error: specified process did not start (or is a UWP app that failed to launch) within 30 seconds.",
		            "Timeout", MB_OK | MB_ICONERROR);
		        return 1;
		    }
		}

        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | SYNCHRONIZE, FALSE, pid);
        if (hProcess) {
            if (argc > paramStartIndex) {
                char exePath[MAX_PATH] = { 0 };
                if (GetModuleFileNameExA(hProcess, NULL, exePath, MAX_PATH)) {
                    if (IsUWPPath(exePath)) {
                        MessageBoxA(NULL,
                            "Error: monitored process appears to belong to a pure UWP app and cannot accept command-line parameters.",
                            "Pure UWP App", MB_OK | MB_ICONERROR);
                        CloseHandle(hProcess);
                        return 1;
                    }
                }
            }
            WaitForSingleObject(hProcess, INFINITE);
            CloseHandle(hProcess);
        }
    }

    return 0;
}

