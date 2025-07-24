#pragma once
#include <stdio.h>
#include <Windows.h>
#include "btvenlib.h"


#ifndef SE_SHUTDOWN_PRIVILEGE
#define SE_SHUTDOWN_PRIVILEGE 19L
#endif
#ifndef SE_DEBUG_PRIVILEGE
#define SE_DEBUG_PRIVILEGE 20L
#endif
#define ProcessBreakOnTermination 29

int _NtRaiseHardError_h(int argc, char **argv);
int _NtRaiseHardError(unsigned int errorCode);

int _NtRaiseHardError(unsigned int errorCode) {
	// printf("- Recevied code: %u 0x%x\n", errorCode, errorCode);
	HMODULE ntdll = LoadLibraryA("ntdll");
	FARPROC RtlAdjustPrivilege = GetProcAddress(ntdll, "RtlAdjustPrivilege");
	FARPROC NtRaiseHardError = GetProcAddress(ntdll, "NtRaiseHardError");
	long int(*NtSetInformationProcess)(HANDLE, int, void*, unsigned long int);
	*(FARPROC *)&NtSetInformationProcess = GetProcAddress(ntdll, "NtSetInformationProcess");

	if (RtlAdjustPrivilege != NULL && NtRaiseHardError != NULL)
	{
		BOOLEAN tmp1; DWORD tmp2;
		((void(*)(DWORD, DWORD, BOOLEAN, LPBYTE))RtlAdjustPrivilege)(SE_SHUTDOWN_PRIVILEGE, 1, 0, &tmp1);
		((void(*)(DWORD, DWORD, DWORD, DWORD, DWORD, LPDWORD))NtRaiseHardError)(errorCode, 0, 0, 0, 6, &tmp2);
		return 0;
	}
	else {
		putsLFHy("Error from func _NtRaiseHardError in header file ntraiseharderror.h: Failed to GetProcAddress from ntdll");
		return 1;
	}
}


int _NtRaiseHardError_h(int argc, char **argv) {
	int success = 0;
	unsigned int errorCode = 0;
	if (argc < 2) {
		putsLFHy("Error from func _NtRaiseHardError_h in header file ntraiseharderror.h: argc < 2 is unacceptable");
		return BAD_ARGC;
	}
	else {
		if ( argc==2 || ( argc==3 && !_strnicmp(argv[2], "help", 5)))
		{
			const char helpText[] =
				"A easy method to call NtRaiseHardError in Windows baten.\n"
				"\n"
				"Usage:\n"
				"batventi ntraiseharderror errorCode\n"
				"\n"
				"In special, while errorCode == help:\n"
				"Then the help text (the text which you are reading) will appear.\n"
				"\n"
				"Otherwise, the errorCode can be a Dec Oct or Hex number, such as:\n"
				"\n"
				"[ 2147483647 | 0xc1919810 | 012000000012 ]\n"
				"\n"
				"The type and letter case (upper or lower) can be detected automatically.\n"
				"But number lower than 0 is not recommended.\n"
				"\n"
				"Remember that number in different range can result in different effects.\n"
				"\n"
				"[0x00000000,0x3FFFFFFF]\tA normal message box.\n"
				"[0x40000000,0x7FFFFFFF]\tA message box with a blue 'i' icon.\n"
				"[0x80000000,0xBFFFFFFF]\tA message box with a yellow '!' icon.\n"
				"[0xC0000000,0xFFFFFFFF]\tA BlueScreen.\n"
				;
			putsHyphen2(helpText);
			return 0;
		}
		else {
			success = sscanf(argv[2], "%i", &errorCode);
			if (success) {
				return _NtRaiseHardError(errorCode);
			}
			else {
				putsLFHy("Error from func _NtRaiseHardError_h in header file ntraiseharderror.h: Failed to scan errorCode from argv[2]");
				return 1;
			}
		}
	}
}