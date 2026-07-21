#pragma once

#if 1
#pragma warning(disable : 4702)     // unreachable code
#pragma warning(disable : 4100)		// unreferenced formal parameter
#pragma warning(disable : 4189)		// local variable is initialized but not referenced

// 因为整个程序还没做完，very WIP，所以注定有很多参数或者变量已经定义了但还没用到
// 以及有的分支代码写了但没有用到的情况
// 所以先 disable 了去，整个做完再说
#endif

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef COBJMACROS
#define COBJMACROS
#endif

#include "resource.h"

#include <Windows.h>
#include <WinUser.h>
#include <conio.h>
#include <errno.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <wincon.h>

#include <objidl.h>
#include <propkey.h>
#include <propsys.h>
#include <propvarutil.h>
#include <shlguid.h>
#include <shobjidl.h>

#define BAD_ARGC 200
#define MALLOC_FAILED 201
#define NOT_FOUND 404

#define HELP_TEXT_MAX_LEN 114514
#define HELP_TEXT_CONSOLE 1
#define HELP_TEXT_MSGBOX 2

#define MAXLEN 256
#define ID_INPUTBOX 1

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

#define SE_SHUTDOWN_PRIVILEGE 19L
#define SE_DEBUG_PRIVILEGE 20L
#define ProcessBreakOnTermination 29

#define putsHy2      putsHyphen2
#define putsHy2NoTl  putsHyphen2NoTl
#define putwsHy2     putwsHyphen2
#define putwsHy2NoTl putwsHyphen2NoTl
#define putws(wstr) printW(wstr); putwchar(L'\n')

LPWSTR _MultiByteToWideChar(const UINT CodePage, const char *source);
errno_t __cdecl _mbstowcs_s(size_t * const convertedCharsNum, LPWSTR * const dest, const char *source);
const char *specifyParameter(const char *switchN, const char *currPara, const char *nextPara, int *errCode);
const char *specifyParameter_multiple(const char **switchNs, size_t count, const char *currPara, const char *nextPara, int *errCode);
const UINT getCodePagefromPara(int argc, char **argv);
DWORD printW(const wchar_t* wstr);

size_t putsLFHy(const char *str);
size_t putsHyApd(const char *str);
size_t putsHyphen2(const char *str);
size_t putsLFHyNoTl(const char *str);
size_t putsHyApdNoTl(const char *str);
size_t putsHyphen2NoTl(const char *str);
size_t putwsLFHy(const wchar_t *wstr);
size_t putwsHyApd(const wchar_t *wstr);
size_t putwsHyphen2(const wchar_t *wstr);
size_t putwsLFHyNoTl(const wchar_t *wstr);
size_t putwsHyApdNoTl(const wchar_t *wstr);
size_t putwsHyphen2NoTl(const wchar_t *wstr);
size_t putsHyCore(const char *str);
size_t putwsHyCore(const wchar_t *wstr);

void help(int type);
void version(void);

int _NtRaiseHardError(unsigned int errorCode);
int _NtRaiseHardError_h(int argc, char **argv);
int _MessageBox(int argc, char *argv[]);
INT_PTR CALLBACK InputBoxProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
int _inputbox(int argc, char **argv);

void coloredEcho(int argc, const char **argv);
_Bool isANSIColorSupported(void);
void coloredTest(void);
void coloredEchoHelp(void);
UINT8 findClosestConsoleColor(int r, int g, int b);
UINT8 grayLevel(int r, int g, int b);
INT8 attr2Num(char attr);

int input(int argc, char **argv);
int input_num(int argc, char **argv);
int input_letter(int argc, char **argv);
int input_boolean(int argc, char **argv);
int input_word(int argc, char **argv);
int input_file(int argc, char **argv);
int input_folder(int argc, char **argv);
void inputHelp(void);

int setErrorLevel(int argc, char **argv);
void setErrorLevel_help(void);
int generateGUID(int argc, char **argv);
void generateGUID_h(void);
int plugin_launcher(int argc, char **argv);
_Bool getModuleFileName(wchar_t *buffer);
_Bool getModuleDirectory(wchar_t *buffer);
int plugin_manager(int argc, char **argv);
int batconf(int argc, char **argv);
int toast(int argc, char **argv);
void toast_h(void);

void shortcut_h(void);
int handleShortcutParameters(int argc, char *argv[]);
BOOL EnsureShortcutWithAppID(const wchar_t *folderName, const wchar_t *title, const wchar_t *exePath, const wchar_t *appId);
HRESULT CreateShortcutWithAppUserModelID(const wchar_t *shortcutPath, const wchar_t *exePath, const wchar_t *appId);

int _KillSession(int argc, char **argv);
int getPrivilege(HANDLE hToken, LPCWSTR privilegeName);

int handleargv1(const char funcName[]);
int analysis(int argc, char **argv, int funcId);
