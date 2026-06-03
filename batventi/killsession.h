#pragma once
#include <stdio.h>
#include <Windows.h>
#include "btvenlib.h"

int _KillSession(int argc, char** argv);

int _KillSession(int argc, char** argv) {
    if (argc < 1) {
        putsLFHy("Error from func _KillSession in header file killsession.h: argc < 1 is unacceptable");
        return BAD_ARGC;
    }
    else {
        if (argc == 1 || (argc == 2 && !_strnicmp(argv[1], "help", 5)))
        {
            const char helpText[] =
                "A easy method to kill the current session in Windows baten.\n"
                "\n"
                "Usage:\n"
                "batventi killsession\n"
                "\n"
                "In special, while the first parameter == help:\n"
                "Then the help text (the text which you are reading) will appear.\n";
            putsLFHy(helpText);
            return 0;
        }
        else if (argc != 2) {
            putsLFHy("Error from func _KillSession in header file killsession.h: Invalid argc, should be 1 or 2");
            return BAD_ARGC;
        }
        else if (!_strnicmp(argv[1], "ExitWindowsEx", 14)) {
            if (!ExitWindowsEx(EWX_LOGOFF, 0)) {
                putsLFHy("Error from func _KillSession in header file killsession.h: Failed to call ExitWindowsEx");
                return 1;
            } else { return 0; }
        }
        else {
            putsLFHy("Error from func _KillSession in header file killsession.h: Invalid parameter");
            return 1;
        }
    }
}