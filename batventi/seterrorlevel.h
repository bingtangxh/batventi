#pragma once
#include "btvenlib.h"







void setErrorLevel_help(void);


int setErrorLevel(int argc, char **argv) {
	int errorlevel = 0, elemsGotten = 0;
	if (argc == 1 || (argc == 2 && !_stricmp(argv[1], "help"))) {
		setErrorLevel_help ();
		return 0;
	}
	elemsGotten = sscanf(argv[1], "%i", &errorlevel);
	if (elemsGotten == 0) {
		putsLFHy("Error from func setErrorLevel in header file seterrorlevel.h: Could not get errorlevel from argv[1]");
		return 0;
	}
	else {
		return errorlevel;
	}
}

void setErrorLevel_help(void) {
	const char helpText[] =
		"Return a specific errorlevel value.\n"
		"\n"
		"Usage:\n"
		"batventi seterrorlevel value\n"
		"\n"
		"value\tA number will be used for the errorlevel value.\n"
		"\n"
		"If the feature is run in help parameter or a wrong given value, then it will return 0.\n"
		""
		""
		""
		""
		""
		""
		""
		""
		""
		""
		"";
	putsHyphen2(helpText);

}