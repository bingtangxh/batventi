#pragma once

#include "btvenlib.h"



void toast_h(void);
int toast(int argc,char **argv);

int toast(int argc,char **argv)
{
    if (!_stricmp(argv[1], "help")) {
        toast_h();
        return 0;
    }
    putsHyphen2("Error from func toast in header file toast.h: This func is not finished yet.");
    return 0;
}

void toast_h(void)
{
    putsHyphen2("Usage: toast [help]");
    putsHyphen2("This command is not implemented yet.");
}
