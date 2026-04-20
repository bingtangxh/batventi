#pragma once

#ifndef COBJMACROS
#define COBJMACROS
#endif

#include <Windows.h>
#include <shobjidl.h>
#include <shlguid.h>
#include <propsys.h>
#include <propvarutil.h>
#include <propkey.h>
#include <combaseapi.h>
#include <objidl.h> 

void shortcut_h(void);
int handleShortcutParameters(int argc,char* argv[]);
BOOL EnsureShortcutWithAppID(const wchar_t* folderName,const wchar_t* title,const wchar_t* exePath,const wchar_t* appId);
HRESULT CreateShortcutWithAppUserModelID(const wchar_t* shortcutPath,const wchar_t* exePath,const wchar_t* appId);
static HRESULT InitPropVariantFromStringCompat(const wchar_t* value,PROPVARIANT* pv);

int handleShortcutParameters(int argc,char* argv[]){
    if(argc<4) {
        putsLFHy("Error from func handleShortcutParameters: Not enough arguments. Please provide at least the title and exePath for the shortcut.");
        return BAD_ARGC;
    }
    if(!_stricmp(argv[1],"--help")||!_stricmp(argv[1],"/?")||!_stricmp(argv[1],"-?")||!_stricmp(argv[1],"/h")||!_stricmp(argv[1],"-h")||!_stricmp(argv[1],"help")) {
        shortcut_h();
        return 0;
    }
    wchar_t *folderName=NULL;
    size_t folderNameLen=MultiByteToWideChar(CP_ACP,0,argv[1],-1,NULL,0);
    folderName=(wchar_t*) malloc(folderNameLen*sizeof(wchar_t));
    if(folderName==NULL) {
        putsLFHy("Error from func handleShortcutParameters: Failed to allocate memory for folderName.");
        return ERROR_IPSEC_IKE_OUT_OF_MEMORY;
    }
    MultiByteToWideChar(CP_ACP,0,argv[1],-1,folderName,(int) folderNameLen);
    wchar_t* title=NULL;
    size_t titleLen=MultiByteToWideChar(CP_ACP,0,argv[2],-1,NULL,0);
    title=(wchar_t*) malloc(titleLen * sizeof(wchar_t));
    if(title==NULL) {
        free(folderName);
        putsLFHy("Error from func handleShortcutParameters: Failed to allocate memory for title.");
        return ERROR_IPSEC_IKE_OUT_OF_MEMORY;
    }
    MultiByteToWideChar(CP_ACP,0,argv[2],-1,title,(int)titleLen);
    wchar_t* exePath=NULL;
    size_t exePathLen=MultiByteToWideChar(CP_ACP,0,argv[3],-1,NULL,0);
    exePath=(wchar_t*) malloc(exePathLen*sizeof(wchar_t));
    if(exePath==NULL) {
        free(folderName);
        free(title);
        putsLFHy("Error from func handleShortcutParameters: Failed to allocate memory for exePath.");
        return ERROR_IPSEC_IKE_OUT_OF_MEMORY;
    }
    MultiByteToWideChar(CP_ACP,0,argv[3],-1,exePath,(int) exePathLen);


    wchar_t* appId=NULL;
    if(argc>=5) {
        size_t appIdLen=MultiByteToWideChar(CP_ACP,0,argv[4],-1,NULL,0);
        appId=(wchar_t*) malloc(appIdLen * sizeof(wchar_t));
        if(appId==NULL) {
            free(folderName);
            free(title);
            free(exePath);
            putsLFHy("Error from func handleShortcutParameters: Failed to allocate memory for appId.");
            return ERROR_IPSEC_IKE_OUT_OF_MEMORY;
        }
        MultiByteToWideChar(CP_ACP,0,argv[4],-1,appId,appIdLen);
    } else {
        appId=L"";
    }
    BOOL result=EnsureShortcutWithAppID(folderName, title, exePath, appId);
    free(folderName);
    free(title);
    free(exePath);
    if (argc>=5) free(appId);
    return result;
}

BOOL EnsureShortcutWithAppID(const wchar_t* folderName,const wchar_t* title,const wchar_t* exePath,const wchar_t* appId)
{
    wchar_t appData[MAX_PATH];
    if (!GetEnvironmentVariable(L"APPDATA",appData,MAX_PATH)) return FALSE;
    wchar_t destPath[MAX_PATH];
#ifdef _MSC_VER
    wcscpy_s(destPath,MAX_PATH,appData);
    wcscat_s(destPath,MAX_PATH,L"\\Microsoft\\Windows\\Start Menu\\Programs\\");
    if (_wcsicmp(folderName,L"\\")) {
        // 仅在 folderName 不等于 "\" 时才追加 folderName，允许用户通过传入 "\" 来表示直接放在 Programs 目录下
        wcscat_s(destPath,MAX_PATH,folderName);
    }
#else
    wcscpy(destPath,appData);
    wcscat(destPath,"\\Microsoft\\Windows\\Start Menu\\Programs\\");
    if (wcsicmp(folderName,L"\\")) {
        strcat(destPath,folderName);
    }
#endif
    DWORD attributes=GetFileAttributesA(destPath);
    if (attributes==INVALID_FILE_ATTRIBUTES||!(attributes&FILE_ATTRIBUTE_DIRECTORY)) {
        if (CreateDirectoryA(destPath,NULL)) {
            printf("Successfully created: %s\n",destPath);
        } else {
            DWORD err=GetLastError();
            if (err==ERROR_ALREADY_EXISTS) {
                printf("Folder already exists, possible to be a file also: %s\n",destPath);
            } else {
                printf("Failed to create the folder: %lu\n",err);
            }
        }
    } else {
        printf("Folder already exists: %s\n",destPath);
    }
    wchar_t shortcutPath[MAX_PATH];
#ifdef _MSC_VER
    wcscpy_s(shortcutPath,MAX_PATH,destPath);
    if (_wcsicmp(folderName,L"\\")) {
        // 仅在 folderName 不等于 "\" 时才追加 "\", 允许用户通过传入 "\" 来表示直接放在 Programs 目录下)
        wcscat_s(shortcutPath,MAX_PATH,L"\\");
    }
    wcscat_s(shortcutPath,MAX_PATH,title);
    wcscat_s(shortcutPath,MAX_PATH,L".lnk");
#else
    wcscpy(shortcutPath,destPath);
    if (wcsicmp(folderName,L"\\")) {
        wcscat(shortcutPath,L"\\");
    }
    wcscat(shortcutPath,title);
    wcscat(shortcutPath,L".lnk");
#endif

    DWORD attr=GetFileAttributes(shortcutPath);
    if (attr!=INVALID_FILE_ATTRIBUTES) return TRUE;

    // 这个本来用于获取当前可执行文件路径的，但现在要让用户指定自己的 exePath ，所以就改成了直接从参数传入 exePath
    // GetModuleFileName(NULL,exePath,MAX_PATH);
    CreateShortcutWithAppUserModelID(shortcutPath,exePath,appId);
    return FALSE;
}

HRESULT CreateShortcutWithAppUserModelID(const wchar_t* shortcutPath,const wchar_t* exePath,const wchar_t* appId)
{
    HRESULT hr=S_OK;
    HRESULT hrInit=CoInitializeEx(NULL,COINIT_MULTITHREADED);

    IShellLinkW* shellLink=NULL;
    IPropertyStore* propStore=NULL;
    IPersistFile* persistFile=NULL;
    PROPVARIANT pv;
    PropVariantInit(&pv);

    hr=CoCreateInstance(&CLSID_ShellLink,NULL,CLSCTX_INPROC_SERVER,&IID_IShellLinkW,(void**) &shellLink);
    if (FAILED(hr)) goto Cleanup;

    hr=IShellLinkW_SetPath(shellLink,exePath);
    if (FAILED(hr)) goto Cleanup;

    wprintf(L"%ls\n",exePath);

    wchar_t workingDir[MAX_PATH]={ 0 };
    DWORD cwdLen=GetCurrentDirectoryW(MAX_PATH,workingDir);
    if (cwdLen>0&&cwdLen<MAX_PATH)
    {
        hr=IShellLinkW_SetWorkingDirectory(shellLink,workingDir);
        if (FAILED(hr)) goto Cleanup;
    }

    hr=IShellLinkW_SetArguments(shellLink,L"");
    if (FAILED(hr)) goto Cleanup;

    hr=IShellLinkW_QueryInterface(shellLink,&IID_IPropertyStore,(void**) &propStore);
    if (FAILED(hr)) goto Cleanup;

    hr=InitPropVariantFromStringCompat(appId,&pv);
    if (FAILED(hr)) goto Cleanup;

    hr=IPropertyStore_SetValue(propStore,&PKEY_AppUserModel_ID,&pv);
    if (FAILED(hr)) goto Cleanup;

    hr=IPropertyStore_Commit(propStore);
    if (FAILED(hr)) goto Cleanup;

    hr=IShellLinkW_QueryInterface(shellLink,&IID_IPersistFile,(void**) &persistFile);
    if (FAILED(hr)) goto Cleanup;

    hr=persistFile->lpVtbl->Save(persistFile,shortcutPath,TRUE);

Cleanup:
    PropVariantClear(&pv);

    if (persistFile) persistFile->lpVtbl->Release(persistFile);
    if (propStore) IPropertyStore_Release(propStore);
    if (shellLink) IShellLinkW_Release(shellLink);

    if (SUCCEEDED(hrInit)) CoUninitialize();

    return hr;
}

static HRESULT InitPropVariantFromStringCompat(const wchar_t* value, PROPVARIANT* pv)
{
    size_t cch = 0;
    wchar_t* buf = NULL;

    if (!pv) return E_POINTER;
    PropVariantInit(pv);

    if (!value) value = L"";
    cch = wcslen(value) + 1;

    buf = (wchar_t*)CoTaskMemAlloc(cch * sizeof(wchar_t));
    if (!buf) return E_OUTOFMEMORY;

    memcpy(buf, value, cch * sizeof(wchar_t));
    pv->vt = VT_LPWSTR;
    pv->pwszVal = buf;
    return S_OK;
}


void shortcut_h(void) {
    char helpText[]="Usage: shortcut <folderName> <title> <exePath> [appId]\n\nThis command creates a shortcut with the specified title in the Start Menu's Programs folder. \n\n"
        "folderName specifies the subfolder under Programs where the shortcut will be placed. If folderName is \"\\\", the shortcut will be placed directly in the Programs folder. \n\n"
        "If an appId is provided, it will be set as the AppUserModelID of the shortcut, which can affect how the application is grouped in the taskbar and Start Menu.\n\n";
    putsLFHy(helpText);
}