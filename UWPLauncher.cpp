#include <windows.h>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        MessageBoxA(NULL, "Usage: launchUWP.exe <PackageFamilyName>", "Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    std::string packageFamilyName = argv[1];
    std::string command = "shell:AppsFolder\\" + packageFamilyName;

    // Execute the command silently
    ShellExecuteA(NULL, "open", "explorer.exe", command.c_str(), NULL, SW_HIDE);

    return 0;
}

