#pragma once
#include <Windows.h>
#include <stdio.h>
#include "btvenlib.h"
#define MAXLEN 256

#define ID_INPUTBOX 1

LPWSTR promptStr = NULL, defaultStr = NULL;

INT_PTR CALLBACK InputBoxProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	WCHAR input[MAXLEN];
	char result[MAXLEN * sizeof(WCHAR)];
	switch (uMsg) {
	case WM_INITDIALOG:
		// 初始化对话框时，可以搁这做点预处理
		// 但是现在却想不到该干什么
		// 要不就弄成设置输入框的初始字符串数值？
		SetDlgItemText(hwndDlg, IDC_STATIC, promptStr);
		SetDlgItemText(hwndDlg, IDC_EDIT1, defaultStr);
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			// 这个用来获取输入框中的文本
			GetDlgItemText(hwndDlg, IDC_EDIT1, input, sizeof(input));
			//MessageBox(hwndDlg, input, L"你输入的内容", MB_OK);
			int result_len = WideCharToMultiByte(CP_ACP, 0, input, -1, result, sizeof(result), NULL, NULL);
			if (result_len > 0) {
				// 输出转换后的内容
				puts(result);  // 使用 puts 输出转换后的字符串
			}
			else {
				putsHyphen("Error from func InputBoxProc in header file inputbox.h: Why did unc WideCharToMultiByte return 0 ?");
				return FALSE;
			}
			EndDialog(hwndDlg, IDOK);
			return TRUE;

		case IDCANCEL:
			EndDialog(hwndDlg, IDCANCEL);
			return TRUE;
		}
		break;
	}
	return FALSE;
}



int _inputbox(int argc, char **argv) {
	// 用户点击确定时，返回值是 1，点击取消时，返回值是 2
	HWND hWnd = NULL;
	UINT uType = 0;
	unsigned destLen = 0;
	if (argc < 2) {
		putsHyphen("Error from func _inputbox in header file inputbox.h: Why argc < 2 ? I could not handle this.");
		return BAD_ARGC;
	}
	switch (argc)
	{
	case 2:
	case 3:
	{
		if (argc == 2 || (argc == 3 && !_strnicmp(argv[2], "help", 5))) {
			const char *helpText =
				"A input dialog box.\n"
				"\n"
				"Usage:\n"
				"batventi inputbox [promptStr] [defaultStr] [ --CodePage=n | --CodePage:n | /CodePage n | -CodePage n ]\n"
				"\n"
				"argv[2] is always treated as the prompt string (window content).\n"
				"\n"
				"argv[3] is always treated as the default answer string.\n"
				"\n"
				"The code page is 0 as default, you can change it using --CodePage or --Encoding.\n"
				"\n"
				"There is nothing like inputbox of VB/VBScript in C Runtime, this is just a imitation.\n"
				"It is currently not supported to use another string as the title of the dialog box.\n"
				"\n"
				"If Text is \"help\" or no arguments are provided, the program will display this help text.\n"
				"\n"
				"At last, the string user typed will be put into the stdout, you can use 'for /f' to catch it.\n"
				"";
			putsHyphen(helpText);
			return 0;
		}
		else {
			promptStr = _MultiByteToWideChar(CP_ACP, argv[2]);
			defaultStr = L"";
			break;
		}
	}
	default:
	{
		/*int current = 0, elemsGotten = 0;
		UINT CodePage = CP_ACP;
		int errCode = 0;
		char *specResult = NULL;
		const char *codePage_Alias[] = { "encoding","codepage" };


		for (int i = 4; i < argc; i++) {
			specResult = specifyParameter_multiple(codePage_Alias, 2, argv[i], argv[i + 1], &errCode);
			if (specResult != NULL) {
				switch (errCode) {
				case 1:
					i++;
				case 0:
					elemsGotten = sscanf(specResult, "%i", &CodePage);
					if (elemsGotten == 0) {
						putsHyphen("Warning from func _inputbox in header file inputbox.h: Could not scan for CodePage from encoding parameter, I will use CP_ACP.");
						CodePage = CP_ACP;
					}
					continue;
				default:
					printf("- Error from func _inputbox in header file inputbox.h: Why errCode == %d while specResult == %p ?\n", errCode, specResult);
					return NOT_FOUND;
				}
			}
			else {
				switch (errCode) {
				case MALLOC_FAILED:
					putsHyphen("Error from func _inputbox in header file inputbox.h: Func specifyParameter_multiple set errCode to MALLOC_FAILED");
					return MALLOC_FAILED;
				case NOT_FOUND:
					continue;
				default:
					printf("- Error from func _inputbox in header file inputbox.h: Why errCode == %d while specResult == NULL ?\n", errCode);
					return NOT_FOUND;
				}
			}
		}*/
		UINT CodePage = getCodePagefromPara(argc, argv);
		promptStr = _MultiByteToWideChar(CodePage, argv[2]);
		defaultStr = _MultiByteToWideChar(CodePage, argv[3]);

		break;
	}
	}
	return DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG1), NULL, InputBoxProc);
}