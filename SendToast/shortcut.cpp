#include <Windows.h>
#include <Windows.UI.Notifications.h>
#include <wrl/client.h>
#include <wrl.h>
#include <wrl/wrappers/corewrappers.h>
#include <windows.data.xml.dom.h>
#include <iostream>
#include <shobjidl.h>
#include <shlguid.h>
#include <propvarutil.h>
#include <propkey.h>
#include <io.h>
#include <versionhelpers.h>
#include <roapi.h>
#include <combaseapi.h>

BOOL __stdcall SendBalloon(const wchar_t* title=L"",const wchar_t* text=L"");
BOOL EnsureShortcutWithAppID(void);
HRESULT CreateShortcutWithAppUserModelID(const wchar_t* shortcutPath,const wchar_t* exePath,const wchar_t* appId);
DWORD WINAPI SendToast(LPVOID messageParam);
BOOL TrySendToastDynamic(const wchar_t* message=L"");
DWORD WINAPI ThreadToast(LPVOID lpParam);
struct ToastParam {
	const wchar_t* message;
	bool result;
};

BOOL EnsureShortcutWithAppID(void)
{
    wchar_t appData[MAX_PATH];
    if (!GetEnvironmentVariable(L"APPDATA",appData,MAX_PATH)) return FALSE;
    wchar_t destPath[MAX_PATH];
#ifdef _MSC_VER
    wcscpy_s(destPath,MAX_PATH,appData);
    wcscat_s(destPath,MAX_PATH,L"\\Microsoft\\Windows\\Start Menu\\Programs\\");
    // wcscat_s(destPath,MAX_PATH,folderName);
#else
    wcscpy(destPath,appData);
    wcscat(destPath,"\\Microsoft\\Windows\\Start Menu\\Programs\\");
    // strcat(destPath,folderName);
#endif
    //DWORD attributes=GetFileAttributesA(destPath);
    //if (attributes==INVALID_FILE_ATTRIBUTES||!(attributes&FILE_ATTRIBUTE_DIRECTORY)) {
    //    if (CreateDirectoryA(destPath,NULL)) {
    //        printf("Successfully created: %s\n",destPath);
    //    } else {
    //        DWORD err=GetLastError();
    //        if (err==ERROR_ALREADY_EXISTS) {
    //            printf("Folder already exists, possible to be a file also: %s\n",destPath);
    //        } else {
    //            printf("Failed to create the folder: %lu\n",err);
    //        }
    //    }
    //} else {
    //    printf("Folder already exists: %s\n",destPath);
    //}
    wchar_t shortcutPath[MAX_PATH];
#ifdef _MSC_VER
    wcscpy_s(shortcutPath,MAX_PATH,destPath);
    // wcscat_s(shortcutPath,MAX_PATH,L"\\");
	wcscat_s(shortcutPath,MAX_PATH,L"BatVenti Toast Mod");
    wcscat_s(shortcutPath,MAX_PATH,L".lnk");
#else
    wcscpy(shortcutPath,destPath);
    // wcscat(shortcutPath,L"\\");
    wcscat(shortcutPath,L"BatVenti Toast Mod");
    wcscat(shortcutPath,L".lnk");
#endif

    DWORD attr=GetFileAttributes(shortcutPath);
    if (attr!=INVALID_FILE_ATTRIBUTES) return TRUE;

    wchar_t exePath[MAX_PATH];
    GetModuleFileName(NULL,exePath,MAX_PATH);
    CreateShortcutWithAppUserModelID(shortcutPath,exePath,L"BingtangXH.BatVentiToastMod");
    return FALSE;
}

HRESULT CreateShortcutWithAppUserModelID(const wchar_t* shortcutPath,const wchar_t* exePath,const wchar_t* appId)
{
    Microsoft::WRL::ComPtr<IShellLink> shellLink=nullptr;
    HRESULT hr=CoCreateInstance(CLSID_ShellLink,nullptr,CLSCTX_INPROC_SERVER,IID_PPV_ARGS(&shellLink));
    if (FAILED(hr)) return hr;
    shellLink->SetPath(exePath);
    wprintf(L"%ls\n", exePath);

    wchar_t workingDir[MAX_PATH]={ 0 };
    DWORD cwdLen=GetCurrentDirectory(MAX_PATH,workingDir);
    if (cwdLen>0&&cwdLen<MAX_PATH) {
        shellLink->SetWorkingDirectory(workingDir); // 设置快捷方式“起始位置”为当前工作目录
    }

    shellLink->SetArguments(L"");

    Microsoft::WRL::ComPtr<IPropertyStore> propStore;
    hr=shellLink.As(&propStore);
    if (FAILED(hr)) return hr;

    PROPVARIANT pv;
    hr=InitPropVariantFromString(appId,&pv);
    if (FAILED(hr)) return hr;

    propStore->SetValue(PKEY_AppUserModel_ID,pv);
    propStore->Commit();
    PropVariantClear(&pv);

    Microsoft::WRL::ComPtr<IPersistFile> persistFile;
    hr=shellLink.As(&persistFile);
    if (FAILED(hr)) return hr;

    hr=persistFile->Save(shortcutPath,TRUE);

    return hr;
}

void container(void) {
		ToastParam param={ L"This is the test Toast text", FALSE };
		if (EnsureShortcutWithAppID()&&IsWindows8OrGreater()) {
			SetCurrentProcessExplicitAppUserModelID(L"BingtangXH.BatVentiToastMod");
			// CreateThread(nullptr, 0, SendToast, const_cast<LPVOID>(static_cast<const void*>(L"Press Alt + Enter for fit\nor F10 for stretch")), 0, nullptr);
			HANDLE h=CreateThread(nullptr,0,ThreadToast,&param,0,nullptr);
			WaitForSingleObject(h,INFINITE);
			CloseHandle(h);
		}
		if (!param.result) {
			SendBalloon(L"BatVenti",L"This is the test Balloon text");
		}
}

#if 1
BOOL TrySendToastDynamic(const wchar_t* message) {
	if (!message) message=L"";

	// 动态加载 combase.dll（包含 RoInitialize 等函数）
	HMODULE hCombase=LoadLibraryW(L"combase.dll");
	if (!hCombase) return FALSE;

	auto pRoInitialize=reinterpret_cast<decltype(&RoInitialize)>(GetProcAddress(hCombase,"RoInitialize"));
	auto pRoUninitialize=reinterpret_cast<decltype(&RoUninitialize)>(GetProcAddress(hCombase,"RoUninitialize"));
	auto pRoGetActivationFactory=reinterpret_cast<decltype(&RoGetActivationFactory)>(GetProcAddress(hCombase,"RoGetActivationFactory"));
	auto pWindowsCreateString=reinterpret_cast<decltype(&WindowsCreateString)>(GetProcAddress(hCombase,"WindowsCreateString"));
	auto pWindowsDeleteString=reinterpret_cast<decltype(&WindowsDeleteString)>(GetProcAddress(hCombase,"WindowsDeleteString"));

	if (!pRoInitialize||!pRoUninitialize||!pRoGetActivationFactory||!pWindowsCreateString||!pWindowsDeleteString) {
		FreeLibrary(hCombase);
		return FALSE;
	}

	const wchar_t* appId=L"BingtangXH.BatVentiToastMod";
	HRESULT hr=E_FAIL;
	bool roInitialized=false;

	HSTRING hToastMgrClass=nullptr;
	HSTRING hToastClass=nullptr;
	HSTRING tagName=nullptr;
	HSTRING data=nullptr;
	HSTRING applicationId=nullptr;

	ABI::Windows::UI::Notifications::IToastNotificationManagerStatics* toastManager=nullptr;
	ABI::Windows::Data::Xml::Dom::IXmlDocument* toastXml=nullptr;
	ABI::Windows::Data::Xml::Dom::IXmlNodeList* textNodes=nullptr;
	ABI::Windows::Data::Xml::Dom::IXmlNode* textNode=nullptr;
	ABI::Windows::Data::Xml::Dom::IXmlText* xmlText=nullptr;
	ABI::Windows::Data::Xml::Dom::IXmlNode* textNodeAppend=nullptr;
	ABI::Windows::Data::Xml::Dom::IXmlNode* appended=nullptr;
	ABI::Windows::UI::Notifications::IToastNotificationFactory* toastFactory=nullptr;
	ABI::Windows::UI::Notifications::IToastNotification* toast=nullptr;
	ABI::Windows::UI::Notifications::IToastNotifier* notifier=nullptr;

	hr=pRoInitialize(RO_INIT_MULTITHREADED);
	if (FAILED(hr)) goto Cleanup;
	roInitialized=true;

	SetCurrentProcessExplicitAppUserModelID(appId);

	hr=pWindowsCreateString(
		RuntimeClass_Windows_UI_Notifications_ToastNotificationManager,
		static_cast<UINT32>(wcslen(RuntimeClass_Windows_UI_Notifications_ToastNotificationManager)),
		&hToastMgrClass);
	if (FAILED(hr)) goto Cleanup;

	hr=pRoGetActivationFactory(hToastMgrClass,__uuidof(*toastManager),reinterpret_cast<void**>(&toastManager));
	if (FAILED(hr)||!toastManager) goto Cleanup;

	hr=toastManager->GetTemplateContent(
		ABI::Windows::UI::Notifications::ToastTemplateType_ToastText01,&toastXml);
	if (FAILED(hr)||!toastXml) goto Cleanup;

	hr=pWindowsCreateString(L"text",static_cast<UINT32>(wcslen(L"text")),&tagName);
	if (FAILED(hr)) goto Cleanup;

	hr=toastXml->GetElementsByTagName(tagName,&textNodes);
	if (FAILED(hr)||!textNodes) goto Cleanup;

	hr=textNodes->Item(0,&textNode);
	if (FAILED(hr)||!textNode) goto Cleanup;

	hr=pWindowsCreateString(message,static_cast<UINT32>(wcslen(message)),&data);
	if (FAILED(hr)) goto Cleanup;

	hr=toastXml->CreateTextNode(data,&xmlText);
	if (FAILED(hr)||!xmlText) goto Cleanup;

	hr=xmlText->QueryInterface(&textNodeAppend);
	if (FAILED(hr)||!textNodeAppend) goto Cleanup;

	hr=textNode->AppendChild(textNodeAppend,&appended);
	if (FAILED(hr)) goto Cleanup;

	hr=pWindowsCreateString(
		RuntimeClass_Windows_UI_Notifications_ToastNotification,
		static_cast<UINT32>(wcslen(RuntimeClass_Windows_UI_Notifications_ToastNotification)),
		&hToastClass);
	if (FAILED(hr)) goto Cleanup;

	hr=pRoGetActivationFactory(hToastClass,__uuidof(*toastFactory),reinterpret_cast<void**>(&toastFactory));
	if (FAILED(hr)||!toastFactory) goto Cleanup;

	hr=toastFactory->CreateToastNotification(toastXml,&toast);
	if (FAILED(hr)||!toast) goto Cleanup;

	hr=pWindowsCreateString(appId,static_cast<UINT32>(wcslen(appId)),&applicationId);
	if (FAILED(hr)) goto Cleanup;

	hr=toastManager->CreateToastNotifierWithId(applicationId,&notifier);
	if (FAILED(hr)||!notifier) goto Cleanup;

	hr=notifier->Show(toast);

Cleanup:
	if (notifier) notifier->Release();
	if (toast) toast->Release();
	if (toastFactory) toastFactory->Release();
	if (appended) appended->Release();
	if (textNodeAppend) textNodeAppend->Release();
	if (xmlText) xmlText->Release();
	if (textNode) textNode->Release();
	if (textNodes) textNodes->Release();
	if (toastXml) toastXml->Release();
	if (toastManager) toastManager->Release();

	if (applicationId) pWindowsDeleteString(applicationId);
	if (data) pWindowsDeleteString(data);
	if (tagName) pWindowsDeleteString(tagName);
	if (hToastClass) pWindowsDeleteString(hToastClass);
	if (hToastMgrClass) pWindowsDeleteString(hToastMgrClass);

	if (roInitialized) pRoUninitialize();
	FreeLibrary(hCombase);

	return SUCCEEDED(hr) ? TRUE : FALSE;
}
#else
BOOL TrySendToastDynamic(const wchar_t* message) {
	// 动态加载 combase.dll（包含 RoInitialize 等函数）
	HMODULE hCombase=LoadLibraryW(L"combase.dll");
	if (!hCombase) return false;
	// 获取函数指针
	auto pRoInitialize=(decltype(&RoInitialize)) GetProcAddress(hCombase,"RoInitialize");
	auto pRoUninitialize=(decltype(&RoUninitialize)) GetProcAddress(hCombase,"RoUninitialize");
	auto pRoGetActivationFactory=(decltype(&RoGetActivationFactory)) GetProcAddress(hCombase,"RoGetActivationFactory");
	auto pWindowsCreateString=(decltype(&WindowsCreateString)) GetProcAddress(hCombase,"WindowsCreateString");
	auto pWindowsDeleteString=(decltype(&WindowsDeleteString)) GetProcAddress(hCombase,"WindowsDeleteString");
	if (!pRoInitialize||!pRoUninitialize||!pRoGetActivationFactory||!pWindowsCreateString||!pWindowsDeleteString) {
		FreeLibrary(hCombase);
		return false;
	}
	// 初始化 WinRT
	HRESULT hr=pRoInitialize(RO_INIT_MULTITHREADED);
	if (FAILED(hr)) {
		FreeLibrary(hCombase);
		return false;
	}
	// 设置 AppUserModelID，要不然 Toast 不被显示
	const wchar_t* appId=L"BingtangXH.BatVentiToastMod";
	SetCurrentProcessExplicitAppUserModelID(appId);
	// 创建字符串用来获取 ToastNotificationManager
	HSTRING hToastMgrClass=nullptr;
	hr=pWindowsCreateString(
		RuntimeClass_Windows_UI_Notifications_ToastNotificationManager,
		(UINT32) wcslen(RuntimeClass_Windows_UI_Notifications_ToastNotificationManager),
		&hToastMgrClass);
	if (FAILED(hr)) {
		pRoUninitialize();
		FreeLibrary(hCombase);
		return false;
	}
	// 获取 IToastNotificationManagerStatics 接口
	ABI::Windows::UI::Notifications::IToastNotificationManagerStatics* toastManager=nullptr;
	hr=pRoGetActivationFactory(hToastMgrClass,__uuidof(toastManager),(void**) &toastManager);
	if (FAILED(hr)||!toastManager) {
		pWindowsDeleteString(hToastMgrClass);
		pRoUninitialize();
		FreeLibrary(hCombase);
		return false;
	}
	// 获取一个 Toast XML 模板
	ABI::Windows::Data::Xml::Dom::IXmlDocument* toastXml=nullptr;
	hr=toastManager->GetTemplateContent(
		ABI::Windows::UI::Notifications::ToastTemplateType_ToastText01,&toastXml);
	if (FAILED(hr)||!toastXml) {
		toastManager->Release();
		pWindowsDeleteString(hToastMgrClass);
		pRoUninitialize();
		FreeLibrary(hCombase);
		return false;
	}
	// 获取文本节点
	ABI::Windows::Data::Xml::Dom::IXmlNodeList* textNodes=nullptr;
	HSTRING tagName=nullptr;
	WindowsCreateString(L"text",wcslen(L"text"),&tagName);
	// hr = toastXml->GetElementsByTagName(Microsoft::WRL::Wrappers::HStringReference(L"text").Get(), &textNodes);
	hr=toastXml->GetElementsByTagName(tagName,&textNodes);
	WindowsDeleteString(tagName);
	if (FAILED(hr)||!textNodes) {
		toastXml->Release();
		toastManager->Release();
		pWindowsDeleteString(hToastMgrClass);
		pRoUninitialize();
		FreeLibrary(hCombase);
		return false;
	}
	ABI::Windows::Data::Xml::Dom::IXmlNode* textNode=nullptr;
	hr=textNodes->Item(0,&textNode);
	if (FAILED(hr)||!textNode) {
		textNodes->Release();
		toastXml->Release();
		toastManager->Release();
		pWindowsDeleteString(hToastMgrClass);
		pRoUninitialize();
		FreeLibrary(hCombase);
		return false;
	}
	// 创建文本节点
	ABI::Windows::Data::Xml::Dom::IXmlText* xmlText=nullptr;
	HSTRING data;
	WindowsCreateString(message,wcslen(message),&data);
	// hr = toastXml->CreateTextNode(Microsoft::WRL::Wrappers::HStringReference(message).Get(), &xmlText);
	hr=toastXml->CreateTextNode(data,&xmlText);
	WindowsDeleteString(data);
	if (FAILED(hr)||!xmlText) {
		textNode->Release();
		textNodes->Release();
		toastXml->Release();
		toastManager->Release();
		pWindowsDeleteString(hToastMgrClass);
		pRoUninitialize();
		FreeLibrary(hCombase);
		return false;
	}
	ABI::Windows::Data::Xml::Dom::IXmlNode* textNodeAppend=nullptr;
	xmlText->QueryInterface(&textNodeAppend);
	if (!textNodeAppend) {
		xmlText->Release();
		textNode->Release();
		textNodes->Release();
		toastXml->Release();
		toastManager->Release();
		pWindowsDeleteString(hToastMgrClass);
		pRoUninitialize();
		FreeLibrary(hCombase);
		return false;
	}
	ABI::Windows::Data::Xml::Dom::IXmlNode* appended=nullptr;
	hr=textNode->AppendChild(textNodeAppend,&appended);
	if (FAILED(hr)) {
		textNodeAppend->Release();
		xmlText->Release();
		textNode->Release();
		textNodes->Release();
		toastXml->Release();
		toastManager->Release();
		pWindowsDeleteString(hToastMgrClass);
		pRoUninitialize();
		FreeLibrary(hCombase);
		return false;
	}
	// 创建 Toast 通知对象
	ABI::Windows::UI::Notifications::IToastNotificationFactory* toastFactory=nullptr;
	HSTRING hToastClass=nullptr;
	pWindowsCreateString(RuntimeClass_Windows_UI_Notifications_ToastNotification,
						 (UINT32) wcslen(RuntimeClass_Windows_UI_Notifications_ToastNotification),
						 &hToastClass);
	hr=pRoGetActivationFactory(hToastClass,__uuidof(toastFactory),(void**) &toastFactory);
	if (FAILED(hr)||!toastFactory) {
		pWindowsDeleteString(hToastClass);
		return false;
	}
	ABI::Windows::UI::Notifications::IToastNotification* toast=nullptr;
	hr=toastFactory->CreateToastNotification(toastXml,&toast);
	if (FAILED(hr)) {
		toastFactory->Release();
		return false;
	}
	ABI::Windows::UI::Notifications::IToastNotifier* notifier=nullptr;
	HSTRING applicationId;
	WindowsCreateString(appId,wcslen(appId),&applicationId);
	// hr = toastManager->CreateToastNotifierWithId(Microsoft::WRL::Wrappers::HStringReference(appId).Get(), &notifier);
	hr=toastManager->CreateToastNotifierWithId(applicationId,&notifier);
	WindowsDeleteString(applicationId);
	if (FAILED(hr)) {
		toast->Release();
		toastFactory->Release();
		return false;
	}
	hr=notifier->Show(toast);
	// 完了都释放掉
	notifier->Release();
	toast->Release();
	toastFactory->Release();
	appended->Release();
	textNodeAppend->Release();
	xmlText->Release();
	textNode->Release();
	textNodes->Release();
	toastXml->Release();
	toastManager->Release();
	pWindowsDeleteString(hToastMgrClass);
	pWindowsDeleteString(hToastClass);
	pRoUninitialize();
	FreeLibrary(hCombase);
	return SUCCEEDED(hr);
}
#endif

DWORD WINAPI ThreadToast(LPVOID lpParam) {
	ToastParam* p=(ToastParam*) lpParam;
	p->result=TrySendToastDynamic(p->message);
	return 0;
}

#if 0
DWORD WINAPI SendToast(LPVOID messageParam=LPVOID(L"")) {
	CoInitializeEx(nullptr,COINIT_MULTITHREADED);
	const wchar_t* message=static_cast<const wchar_t*>(messageParam);
	if (_isatty(_fileno(stdout))) {
		::SetConsoleTextAttribute(::GetStdHandle(STD_OUTPUT_HANDLE),
								  FOREGROUND_INTENSITY|FOREGROUND_RED|FOREGROUND_GREEN);
	}
	std::wcout<<L"Sending toast: "<<message<<std::endl;
	Microsoft::WRL::Wrappers::RoInitializeWrapper initialize(RO_INIT_MULTITHREADED);
	if (FAILED(initialize)) {
		std::wcerr<<L"RoInitialize failed: 0x"<<std::hex<<initialize<<std::endl;
		return 1;
	}
	const wchar_t* appId=L"BingtangXH.BatVentiToastMod";
	::SetCurrentProcessExplicitAppUserModelID(appId);
	Microsoft::WRL::ComPtr<ABI::Windows::UI::Notifications::IToastNotificationManagerStatics> toastManager;
	HRESULT hr=Windows::Foundation::GetActivationFactory(
		Microsoft::WRL::Wrappers::HStringReference(
			RuntimeClass_Windows_UI_Notifications_ToastNotificationManager).Get(),
		&toastManager);
	if (FAILED(hr)) {
		std::wcerr<<L"Failed to get ToastManager: 0x"<<std::hex<<hr<<std::endl;
		return 1;
	}
	Microsoft::WRL::ComPtr<ABI::Windows::Data::Xml::Dom::IXmlDocument> toastXml;
	hr=toastManager->GetTemplateContent(
		ABI::Windows::UI::Notifications::ToastTemplateType_ToastText01,
		&toastXml);
	if (FAILED(hr)) {
		std::wcerr<<L"Failed to get template content: 0x"<<std::hex<<hr<<std::endl;
		return 1;
	}
	Microsoft::WRL::ComPtr<ABI::Windows::Data::Xml::Dom::IXmlNodeList> textNodes;
	hr=toastXml->GetElementsByTagName(
		Microsoft::WRL::Wrappers::HStringReference(L"text").Get(),
		&textNodes);
	if (FAILED(hr)) {
		std::wcerr<<L"Failed to get text nodes: 0x"<<std::hex<<hr<<std::endl;
		return 1;
	}
	UINT32 nodeCount;
	hr=textNodes->get_Length(&nodeCount);
	if (FAILED(hr)||nodeCount==0) {
		std::wcerr<<L"No text nodes found"<<std::endl;
		return 1;
	}
	Microsoft::WRL::ComPtr<ABI::Windows::Data::Xml::Dom::IXmlNode> textNode;
	hr=textNodes->Item(0,&textNode);
	if (FAILED(hr)) {
		std::wcerr<<L"Failed to get first text node: 0x"<<std::hex<<hr<<std::endl;
		return 1;
	}
	Microsoft::WRL::ComPtr<ABI::Windows::Data::Xml::Dom::IXmlText> xmlText;
	hr=toastXml->CreateTextNode(
		Microsoft::WRL::Wrappers::HStringReference(message).Get(),
		&xmlText);
	if (FAILED(hr)) {
		std::wcerr<<L"Failed to create text node: 0x"<<std::hex<<hr<<std::endl;
		return 1;
	}
	Microsoft::WRL::ComPtr<ABI::Windows::Data::Xml::Dom::IXmlNode> textNodeAppend;
	hr=xmlText.As(&textNodeAppend);
	if (FAILED(hr)) {
		std::wcerr<<L"Failed to get text node interface: 0x"<<std::hex<<hr<<std::endl;
		return 1;
	}
	Microsoft::WRL::ComPtr<ABI::Windows::Data::Xml::Dom::IXmlNode> appendedNode;
	hr=textNode->AppendChild(textNodeAppend.Get(),&appendedNode);
	if (FAILED(hr)) {
		std::wcerr<<L"Failed to append text: 0x"<<std::hex<<hr<<std::endl;
		return 1;
	}
	Microsoft::WRL::ComPtr<ABI::Windows::UI::Notifications::IToastNotificationFactory> toastFactory;
	hr=Windows::Foundation::GetActivationFactory(
		Microsoft::WRL::Wrappers::HStringReference(
			RuntimeClass_Windows_UI_Notifications_ToastNotification).Get(),
		&toastFactory);
	if (FAILED(hr)) {
		std::wcerr<<L"Failed to get toast factory: 0x"<<std::hex<<hr<<std::endl;
		return 1;
	}
	Microsoft::WRL::ComPtr<ABI::Windows::UI::Notifications::IToastNotification> toast;
	hr=toastFactory->CreateToastNotification(toastXml.Get(),&toast);
	if (FAILED(hr)) {
		std::wcerr<<L"Failed to create toast: 0x"<<std::hex<<hr<<std::endl;
		return 1;
	}
	Microsoft::WRL::ComPtr<ABI::Windows::UI::Notifications::IToastNotifier> notifier;
	hr=toastManager->CreateToastNotifierWithId(
		Microsoft::WRL::Wrappers::HStringReference(appId).Get(),
		&notifier);
	if (FAILED(hr)) {
		std::wcerr<<L"Failed to create notifier: 0x"<<std::hex<<hr<<std::endl;
		return 1;
	}
	hr=notifier->Show(toast.Get());
	if (FAILED(hr)) {
		std::wcerr<<L"Failed to show toast: 0x"<<std::hex<<hr<<std::endl;
		return 1;
	}
	std::wcout<<L"Toast displayed successfully"<<std::endl;
	wchar_t* currentAppId=nullptr;
	::GetCurrentProcessExplicitAppUserModelID(&currentAppId);
	std::wcout<<L"Current AppID: "<<currentAppId<<std::endl;
	::CoTaskMemFree(currentAppId);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
							FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);
	CoUninitialize();
	return 0;
}
#endif

BOOL __stdcall SendBalloon(const wchar_t* title,const wchar_t* text) {
    wchar_t titleBuf[64]={ L'\0' };
    wchar_t infoBuf[256]={ L'\0' };
    wcsncpy_s(titleBuf,64,title,_TRUNCATE);
    wcsncpy_s(infoBuf,256,text,_TRUNCATE);
    NOTIFYICONDATA nid={ sizeof(nid) };
    wcscpy_s(nid.szInfoTitle,64,titleBuf);
    wcscpy_s(nid.szInfo,256,infoBuf);
    nid.uFlags=NIF_INFO;
    nid.dwInfoFlags=NIIF_INFO;
    return ::Shell_NotifyIcon(NIM_ADD,&nid);
}
