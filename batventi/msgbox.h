#pragma once
#include <WinUser.h>
#include "btvenlib.h"

// 返回值∈[1,7]∪{10,11}
int _MessageBox(int argc, const char *argv[]);

int _MessageBox(int argc, const char *argv[])
{
	HWND hWnd = NULL;
	LPTSTR lpText = NULL;
	LPTSTR lpCation = NULL;
	UINT uType = 0;
	unsigned destLen = 0;
	if (argc < 2) {
		putsHyphen("Error from func _MessageBox in header file msgbox.h: Why argc < 2 ? I could not handle this.");
		return BAD_ARGC;
	}
	switch (argc)
	{
	case 2:
	case 3:
	{
		lpText = L"";
		lpCation = L"";
		if (argc == 2 || (argc == 3 && !_strnicmp(argv[2], "help", 5))) {
			const char *helpText =
				"A encapsulation of MessageBox.\n"
				"\n"
				"Usage:\n"
				"batventi msgbox [Text] [Title] [TypeNumber] [ --CodePage=n | --CodePage:n | /CodePage n | -CodePage n ]\n"
				"\n"
				"argv[2] is always treated as the message Text (window content).\n"
				"\n"
				"argv[3] is always treated as the Title of the message box.\n"
				"\n"
				"Starting from argv[4], you can provide any number of numeric values, all of which will be summed up and passed as the uType parameter to the MessageBox function. If no numbers here, it will be 0 as default.\n"
				"\n"
				"A CodePage parameter(e.g., --CodePage = 65001) can be inserted anywhere among the numeric values, or omitted entirely(default is CP_ACP).\n"
				"\n"
				"The Title argument may be not given, but in that case, no further arguments can be specified.\n"
				"If you want to specify type flags or a code page while leaving the title empty, pass "" explicitly.\n"
				"\n"
				"If Text is \"help\" or no arguments are provided, the program will display this help text.\n"
				"\n"
				"uType can be numbers below:\n"
				"\n"
				"0\t0x0\tA OK button.\n"
				"1\t0x1\tA OK and a Cancel.\n"
				"2\t0x2\tAbort, Retry and Ignore.(Using 6 is more recommended than 2)\n"
				"3\t0x3\tYes, No and Cancel.\n"
				"4\t0x4\tYes and No.\n"
				"5\t0x5\tRetry and Cancel.\n"
				"6\t0x6\tCancel, Retry and Continue.\n"
				"16384\t0x4000\tAdd a help button (Currently non-functional)\n"
				"\n"
				"48\t0x30\tA warning or exclamation icon.\n"
				"64\t0x40\tA blue icon with a lower case letter 'i'.\n"
				"32\t0x20\tA '?' icon (Not recommended anymore from Microsoft, due to not distinguished with help info).\n"
				"16\t0x10\tA stop icon.\n"
				"\n"
				"0\t0x0\tThe first button is the default.\n"
				"256\t0x100\tThe second button is the default.\n"
				"512\t0x200\tThe third button is the default.\n"
				"768\t0x300\tThe fourth button is the default.\n"
				"\n"
				"4096\t0x1000\tThe window will contain WS_EX_TOPMOST style.\n"
				"\n"
				"524288\t0x80000 \tText will be right-aligned.\n"
				"1048576\t0x100000\tMessages are displayed using a right-to-left reading order (Especially used on Hebrew and Arabic systems).\n"
				"\n"
				"The return value could be the button listed below which is pressed:\n"
				"\n"
				"1\tOK\n"
				"2\tCancel\n"
				"3\tAbort\n"
				"4\tRetry\n"
				"5\tIgnore\n"
				"6\tYes\n"
				"7\tNo\n"
				"10\tRetry\n"
				"11\tContinue\n"
				"";
			putsHyphen(helpText);
			return 0;
		}
		else {
			lpText = _MultiByteToWideChar(CP_ACP, argv[2]);
			lpCation = L"";
			break;
		}         
	}
	default:
	{
		int current = 0, elemsGotten = 0;
		UINT CodePage = CP_ACP;
		for (int i = 4; i < argc; i++) {
			if (!(_strnicmp(argv[i], "--CodePage=", 11) && _strnicmp(argv[i], "--CodePage:", 11))) {
				const char *CodePageValStr = argv[i] + 11;
				elemsGotten = sscanf(CodePageValStr,"%i",&CodePage);
				if (elemsGotten == 0) {
					putsHyphen("Error from func _MessageBox in header file msgbox.h: Could not scan for CodePage from encoding parameter, I will use CP_ACP.");
					CodePage = CP_ACP;
				}
				continue;
			}
			if (!(_stricmp(argv[i], "-CodePage") && _stricmp(argv[i], "/CodePage"))) {
				// 这个条件语句显然是一个与非门，只要有一个是 0，结果就是 1
				i++;
				elemsGotten = sscanf(argv[i], "%i", &CodePage);
				if (elemsGotten == 0) {
					putsHyphen("Error from func _MessageBox in header file msgbox.h: Could not scan for CodePage from encoding parameter, I will use CP_ACP.");
					CodePage = CP_ACP;
				}
				continue;
			}
			elemsGotten = sscanf(argv[i], "%i", &current);
			if (elemsGotten == 1) {
				uType += current;
				continue;
			}
			else continue;
			
		}

		lpText = _MultiByteToWideChar(CodePage, argv[2]);
		lpCation = _MultiByteToWideChar(CodePage, argv[3]);

		break;
	}
	}





	return MessageBox(hWnd, lpText, lpCation, uType);
}