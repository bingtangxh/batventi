#include "batventi.h"

// killsession.c
typedef long NTSTATUS;
typedef enum _SHUTDOWN_ACTION
{
    ShutdownNoReboot,
    ShutdownReboot,
    ShutdownPowerOff
} SHUTDOWN_ACTION,* PSHUTDOWN_ACTION;
typedef NTSTATUS(NTAPI* NTSHUTDOWNSYSTEM)(SHUTDOWN_ACTION);
int getPrivilege(HANDLE hToken,LPCWSTR privilegeName);

int killsession(int argc, char** argv) {
    HANDLE hToken=NULL;
    if (argc < 1) {
        putsLFHy("Error from func killsession in source file killsession.c: argc < 1 is unacceptable");
        return BAD_ARGC;
    }
    else {
        if (argc == 1 || (argc == 2 && !_strnicmp(argv[1], "help", 5)))
        {
            const char helpText[] =
                "A easy method to kill the current session in Windows baten.\n"
                "\n"
                "Usage:\n"
                "batventi killsession [method] [mode]\n"
                "\n"
                "method can be one of the following values:\n"
                "\n"
                "ExitWindowsEx\n"
                "ExitWindows\n"
                "NtShutdownSystem\n"
                "\n"
                "mode is a number whose meaning depends on the method you choose, for more details please refer to the official Microsoft documentation for each method.\n"
                "\n"
                "For ExitWindowsEx, mode is the flags parameter.\n"
                "0\t0x0\tLog off.\n"
                "1\t0x1\tShut down the system and hang on."
                "2\t0x2\tShut down the system and then restart it.\n"
                "8\t0x8\tShut down the system and turn off the power. The system must support the power-off feature.\n"
                "You can add values below optionally:\n"
                "16\t0x10\tForce all applications to close if they are hung.\n\tThis parameter is not recommended, because it may cause data loss. For more details, please refer to the official Microsoft documentation.\n"
                "4\t0x4\tIgnore all applications which block the shutdown process.\n"
                "\n"
                "ExitWindows does not require a mode parameter, it will just log off the current session.\n"
                "\n"
                "NtShutdownSystem, mode can be one of the following values:\n"
                "0\tShutdownNoReboot\tShut down the system and do not reboot.\n"
                "1\tShutdownReboot\tShut down the system and reboot.\n"
                "2\tShutdownPowerOff\tShut down the system and turn off the power. The system must support the power-off feature.\n"
                "\n"
                "In special, while the first parameter == help:\n"
                "Then the help text (the text which you are reading) will appear.\n";
            putsLFHy(helpText);
            return 0;
        }
        else if (argc != 2&&0) {
            putsLFHy("Error from func killsession in source file killsession.c: Invalid argc, should be 1 or 2");
            return BAD_ARGC;
        }
        else if (!_strnicmp(argv[1], "ExitWindowsEx", 14)) {
            if(argc!=3) {
                putsLFHy("Error from func killsession in source file killsession.c: Invalid argc for ExitWindowsEx method, should be 2");
                return BAD_ARGC;
            }
            int elemsGotten = 0, flags = 0;
            elemsGotten = sscanf(argv[2], "%d", &flags);
            if (elemsGotten == 0) {
                putsLFHy("Error from func killsession in source file killsession.c: Could not scan for flags parameter, please provide a valid number for flags.");
                return 1;
            }
            if((flags|1)||(flags|2)||(flags|8)) {
                if(getPrivilege(hToken, SE_SHUTDOWN_NAME)) { return 1; }
            }
            if (!ExitWindowsEx(flags, 0)) {
                putsLFHy("Error from func killsession in source file killsession.c: Failed to call ExitWindowsEx");
                return 1;
            }
            else { return 0; }
        
        }
        else if (!_strnicmp(argv[1], "ExitWindows", 11)) {
            if (!ExitWindows(0, 0)) {
                putsLFHy("Error from func killsession in source file killsession.c: Failed to call ExitWindows");
                return 1;
            } else { return 0; }
        }
        else if(!_strnicmp(argv[1],"NtShutdownSystem",16)) {
            // printf("%d\n", argc);
            if(argc!=3) {
                putsLFHy("Error from func killsession in source file killsession.c: Invalid argc for NtShutdownSystem method, should be 2");
                return BAD_ARGC;
            }
            int elemsGotten=0,mode=0;
            elemsGotten=sscanf(argv[2],"%d",&mode);
            if(elemsGotten==0) {
                putsLFHy("Error from func killsession in source file killsession.c: Could not scan for mode parameter, please provide a valid number for mode.");
                return 1;
            }
            if(getPrivilege(hToken,SE_SHUTDOWN_NAME)) { return 1; }
            HMODULE hModule=GetModuleHandleW(L"ntdll.dll");
            if(hModule) {
                // FARPROC proc=(NTSHUTDOWNSYSTEM)GetProcAddress(hModule,"NtShutdownSystem");
                FARPROC proc=GetProcAddress(hModule,"NtShutdownSystem");
                if(proc) {
                    proc(mode);
                    return 0;
                }

            }

            putsLFHy("Error from func killsession in source file killsession.c: Failed to GetProcAddress for NtShutdownSystem");
            return 1;

        }
        else {
            putsLFHy("Error from func killsession in source file killsession.c: Invalid parameter");
            return 1;
        }
    }
}

int getPrivilege(HANDLE hToken, LPCWSTR privilegeName) {
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY | TOKEN_ADJUST_PRIVILEGES, &hToken))
    {
        putsLFHy("Error from func killsession in source file killsession.c: Failed to OpenProcessToken");
        return 1;
    }
    TOKEN_PRIVILEGES tkp;
    if (!LookupPrivilegeValue(NULL, privilegeName, &tkp.Privileges[0].Luid)) {
        putsLFHy("Error from func killsession in source file killsession.c: Failed to LookupPrivilegeValue for SE_SHUTDOWN_NAME");
        return 1;
    }
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, NULL, 0)) {
        putsLFHy("Error from func killsession in source file killsession.c: Failed to AdjustTokenPrivileges");
        return 1;
    }
    CloseHandle(hToken);
    return 0;
}
