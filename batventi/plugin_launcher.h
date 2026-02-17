#pragma once
#include <Windows.h>
#include "btvenlib.h"

_Bool getModuleFileName(wchar_t *);
_Bool getModuleDirectory(wchar_t *);













int plugin_launcher(int argc, char **argv) {
	// 暂时先硬编码一个静态长度吧
    wchar_t modulePath[MAX_PATH];
	if(getModuleDirectory(modulePath)) {
		putsHyphen2("Error: Failed to get the module directory.");
		return 1;
    }
    wcscat_s(modulePath,MAX_PATH,L"plugin\\");
    MultiByteToWideChar(CP_ACP,0,argv[1],-1,modulePath+wcslen(modulePath),MAX_PATH);
    wcscat_s(modulePath,MAX_PATH,L".exe");
	_putws(modulePath);
	putsHyphen2("This feature is not finished yet.\nHow about we explore the area ahead of us later?");
	return 0;
}

_Bool getModuleFileName(wchar_t *buffer) {
	HMODULE hModule = GetModuleHandleW(NULL);
	if (hModule == NULL) {
		return 1;
	}
	DWORD result = GetModuleFileNameW(hModule, buffer, MAX_PATH);
	if (result == 0 || result == MAX_PATH) {
		return 1;
	}
	return 0;
}

_Bool getModuleDirectory(wchar_t *buffer) {
	if (getModuleFileName(buffer)) {
		return 1;
	}
	wchar_t *lastBackslash = wcsrchr(buffer, L'\\');
	if (lastBackslash == NULL) {
		return 1;
	}
	*(lastBackslash + 1) = L'\0';
	return 0;
}