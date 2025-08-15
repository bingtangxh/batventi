#pragma once
#define HELP_TEXT_MAX_LEN 114514
#define HELP_TEXT_CONSOLE 1
#define HELP_TEXT_MSGBOX 2
#include <stdio.h>
#include "btvenlib.h"

// ����ļ��� batventi.c ������Դ�����һ�����ù��ܣ�Ӧ��ֻ����һ������ help()

void help(int type) {

	const char *helpText =
		"BatVenti pre-Alpha\n"
		"Copyright (C) Bingtangxh.\n";

	switch (type)
	{
	case HELP_TEXT_MSGBOX:
	{
		putsLFHy(helpText);
		LPCWSTR canOnlyConsole =
			L"������ֻ���������������С�\n"
			L"ʹ�� /? �� --help �鿴�÷���\n"
			L"This program can only be run in console mode.\n"
			L"Use --help or /? to see about the usage.\n"
			L"\n"
			L"Venti will let your Windows baten fly.";
		/*LPWSTR wCanOnlyConsole = (LPWSTR)malloc(sizeof(wchar_t)*(strlen(canOnlyConsole) + 1));
		if (wCanOnlyConsole == NULL) {
		putsHyphen("Error from func help case HELP_TEXT_MSGBOX: malloc for wCanOnlyConsole returned a NULL");
		break;
		}*/
		//size_t convertedCharsNum = 0;
		//// ����������� mbstowcs �洢��¼�ɹ�д����ٸ��ַ��õ�
		//mbstowcs_s(&convertedCharsNum,wCanOnlyConsole, strlen(helpText) + 1,helpText, (strlen(helpText) + 1));
		MessageBox(NULL, canOnlyConsole, L"Batventi Double-clicked Help", MB_OK | MB_ICONINFORMATION);
		break;
	}
	case HELP_TEXT_CONSOLE:
	{
		putsHyphen2(helpText);
		puts("- Usage:\n");
		puts("- batventi functionName [otherParameters] [--quiet] [--help] [--version]\n");
		puts("- functionName can be the names below:");
		puts("- plugin               Launch a existing plugin with parameters given.");
		puts("- pluginMgr            List available plugins, and operate with them.");
		puts("- input                An advanced, robust and smart input method.");
		puts("- msgbox               A encapsulation of MessageBox.");
		puts("- inputbox             A input dialog box.");
		puts("- coloredEcho          Display a sentence with specific color.");
		puts("- ntraiseharderror     A encapsulation of NtRaiseHardError.");
		puts("- GUIDGen              Generate a GUID. No parameters will be received.");
		puts("- setErrorLevel        Just exit with a gievn number used for errorlevel.");


		//puts("- ");
		//puts("- **********************************************************");
		//puts("- --- Some help text about more parameter switches here. ---");
		//puts("- **********************************************************");

		puts("- ");
		puts("- You can use 'batventi functionName help' to see about help info of other functions.");
		puts("- ");
		puts("- --help        Show this help text (Equal to /?).");
		puts("- --version     See about version infomation.");



		break;
	}
	default:
	{
		printf("- Error from func help in header file help.h: How do you mean type %d? %dte nandayo?\n", type, type);
		// �˴��� %dte nandayo? ����ԭ�����ɵġ����G�١���ʲô��˼����������������ؤá��Ƥʤ���衭������
		break;
	}
	}
}

