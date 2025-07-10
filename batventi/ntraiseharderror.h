#pragma once
#include <stdio.h>
#include <Windows.h>
#pragma comment(lib, "Advapi32.lib")  // 用于 OpenProcessToken, AdjustTokenPrivileges, LookupPrivilegeValueA
#pragma comment(lib, "User32.lib")    // 用于 ExitWindowsEx, ShowWindow, GetForegroundWindow

#ifndef SE_SHUTDOWN_PRIVILEGE
#define SE_SHUTDOWN_PRIVILEGE 19L
#endif
#ifndef SE_DEBUG_PRIVILEGE
#define SE_DEBUG_PRIVILEGE 20L
#endif
#define ProcessBreakOnTermination 29

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
		putsHyphen("Error from func _NtRaiseHardError in header file ntraiseharderror.h: Failed to GetProcAddress from ntdll");
		return 1;
	}
}