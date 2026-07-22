#pragma once

#define USEEXTERNC 1

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
#ifdef __cplusplus
#include <wrl/client.h>
#include <wrl.h>
#include <wrl/wrappers/corewrappers.h>
#include <windows.data.xml.dom.h>
#include <Windows.UI.Notifications.h>
#include <iostream>
#include <roapi.h>
typedef bool _Bool;
#else
#include <stdbool.h>
#endif

#include <shobjidl.h>
#include <shlguid.h>
#include <propvarutil.h>
#include <propkey.h>
#include <io.h>
#include <versionhelpers.h>
#include <combaseapi.h>
#include <WinUser.h>
#include <conio.h>
#include <errno.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <wincon.h>
#include <objidl.h>
#include <propsys.h>
#include <Windows.h>

#define BAD_ARGC 200
#define MALLOC_FAILED 201
#define NOT_FOUND 404

#define HELP_TEXT_MAX_LEN 114514
#define HELP_TEXT_CONSOLE 1
#define HELP_TEXT_MSGBOX 2

#define MAXLEN 256

#define putsHy2      putsHyphen2
#define putsHy2NoTl  putsHyphen2NoTl
#define putwsHy2     putwsHyphen2
#define putwsHy2NoTl putwsHyphen2NoTl
#define putws(wstr) do {printW(wstr); putwchar(L'\n')} while(0)

#if defined(__cplusplus) && USEEXTERNC
extern "C" {
#endif

int batconf(int argc,char **argv);

LPWSTR MultiByteToWideChar_wrapper(const UINT CodePage,const char *source);
errno_t __cdecl mbstowcs_s_wrapper(size_t * const convertedCharsNum,LPWSTR * const dest,const char *source);
const char *specifyParameter(const char *switchN,const char *currPara,const char *nextPara,int *errCode);
const char *specifyParameter_multiple(const char **switchNs,size_t count,const char *currPara,const char *nextPara,int *errCode);
const UINT getCodePagefromPara(int argc,char **argv);
DWORD printW(const wchar_t* wstr);

void coloredEcho(int argc,const char **argv);

int createshortcut(int argc,char* argv[]);

int guidgen(int argc,char **argv);

void help(int type);

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

int input(int argc,char **argv);

int inputbox(int argc,char **argv);

int killsession(int argc,char **argv);

int msgbox(int argc,char *argv[]);

int NtRaiseHardError_wrapper_handler(int argc,char **argv);

int plugin_launcher(int argc,char **argv);

int plugin_manager(int argc,char **argv);

int sendToast(int argc,char* argv[]);

int setErrorLevel(int argc,char **argv);

void version(void);

#if defined(__cplusplus) && USEEXTERNC
}
#endif