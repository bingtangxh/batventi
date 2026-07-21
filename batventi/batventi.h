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

#if defined(__cplusplus) && USEEXTERNC
extern "C" {
#endif

// batconf.c
int batconf(int argc,char **argv);

// batventi.c
extern int currentFunc;
typedef struct {
	const char* name;
	int id;
} CommandMap,*CommandMapPtr;
int analysis(int argc,char **argv,int funcId); 
int main(int argc,char **argv);
int handleargv1(const char funcName[]);

// btvenlib.c
#pragma comment(lib, "Advapi32.lib")  
// Advapi32 用于 OpenProcessToken, AdjustTokenPrivileges, LookupPrivilegeValueA
#pragma comment(lib, "User32.lib")    
// User32 用于 ExitWindowsEx, ShowWindow, GetForegroundWindow
#pragma comment(lib, "Kernel32.lib")
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
LPWSTR _MultiByteToWideChar(const UINT CodePage,const char *source);
errno_t __cdecl _mbstowcs_s(size_t * const convertedCharsNum,LPWSTR * const dest,const char *source);
const char *specifyParameter(const char *switchN,const char *currPara,const char *nextPara,int *errCode);
const char *specifyParameter_multiple(const char **switchNs,size_t count,const char *currPara,const char *nextPara,int *errCode);
const UINT getCodePagefromPara(int argc,char **argv);
DWORD printW(const wchar_t* wstr);

// coloredEcho.c
typedef struct {
	UINT8 id;
	UINT8 r,g,b;
} ConsoleColor;
void coloredEcho(int argc,const char **argv);
bool isANSIColorSupported();
void coloredTest(void);
void coloredEchoHelp(void);
UINT8 findClosestConsoleColor(int r,int g,int b);
UINT8 grayLevel(int r,int g,int b);
INT8 attr2Num(char attr);

// createshortcut.c
void shortcut_h(void);
int handleShortcutParameters(int argc,char* argv[]);
BOOL EnsureShortcutWithAppID(const wchar_t* folderName,const wchar_t* title,const wchar_t* exePath,const wchar_t* appId);
HRESULT CreateShortcutWithAppUserModelID(const wchar_t* shortcutPath,const wchar_t* exePath,const wchar_t* appId);
static HRESULT InitPropVariantFromStringCompat(const wchar_t* value,PROPVARIANT* pv);

// guidgen.c
#pragma comment(lib, "Ole32.lib")
//需要ole32.dll，如果使用MinGW/gcc，那就gcc命令行加上-lole32
void generateGUID_h(void);
int generateGUID(int argc,char **argv);

// help.c
void help(int type);

// hyphen.c
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

// input.c
int input(int argc,char **argv);
int input_num(int argc,char **argv);
int input_letter(int argc,char **argv);
int input_boolean(int argc,char **argv);
int input_word(int argc,char **argv);
int input_file(int argc,char **argv);
int input_folder(int argc,char **argv);
void inputHelp(void);

// inputbox.c
extern LPWSTR promptStr,defaultStr;
INT_PTR CALLBACK InputBoxProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
int _inputbox(int argc,char **argv);

// killsession.c
typedef long NTSTATUS;
typedef enum _SHUTDOWN_ACTION
{
    ShutdownNoReboot,
    ShutdownReboot,
    ShutdownPowerOff
} SHUTDOWN_ACTION,* PSHUTDOWN_ACTION;
typedef NTSTATUS(NTAPI* NTSHUTDOWNSYSTEM)(SHUTDOWN_ACTION);
int _KillSession(int argc,char **argv);
int getPrivilege(HANDLE hToken,LPCWSTR privilegeName);

// msgbox.c
int _MessageBox(int argc,char *argv[]);

// ntraiseharderror.c
#ifndef SE_SHUTDOWN_PRIVILEGE
#endif
#ifndef SE_DEBUG_PRIVILEGE
#endif
int _NtRaiseHardError_h(int argc,char **argv);
int _NtRaiseHardError(unsigned int errorCode);

// plugin_launcher.c
bool getModuleFileName(wchar_t *);
bool getModuleDirectory(wchar_t *);
int plugin_launcher(int argc,char **argv);

// plugin_manager.c
int plugin_manager(int argc,char **argv);

// sendtoast.cpp
#pragma comment(lib, "Shlwapi.lib")
int sendToast(int argc,char* argv[]);
// DWORD WINAPI SendToast(LPVOID messageParam);
DWORD WINAPI ThreadToast(LPVOID lpParam);
DWORD EnsureShortcutWithAppID2(void);
HRESULT CreateShortcutWithAppUserModelID2(const wchar_t* shortcutPath,const wchar_t* exePath,const wchar_t* appId);
typedef struct toastParam {
	const wchar_t* message;
	BOOL result;
} ToastParam;

// seterrorlevel.c
int setErrorLevel(int argc,char **argv);
void setErrorLevel_help(void);

// version.c
void version(void);

#if defined(__cplusplus) && USEEXTERNC
}
#endif