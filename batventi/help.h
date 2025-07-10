#pragma once
#define HELP_TEXT_MAX_LEN 114514
#define HELP_TEXT_CONSOLE 1
#define HELP_TEXT_MSGBOX 2
#include <stdio.h>
#include "btvenlib.h"

// ����ļ��� batventi.c ������Դ�����һ�����ù��ܣ�Ӧ��ֻ����һ������

void help(int type) {

	const char *helpText =
		"BatVenti pre-Alpha\n"
		"Copyright (C) Bingtangxh.\n";

	switch (type)
	{
	case HELP_TEXT_MSGBOX:
	{
		putsHyphen(helpText);
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
		putsHyphen(helpText);
		puts("- Usage:\n");
		puts("- batventi functionName [otherParameters] [--quiet] [--help] [--version]\n");
		puts("- functionName can be the names below:");
		puts("- input\t\t\t An advanced, robust and smart input method.");
		puts("- msgbox\t\t A encapsulation of MessageBox.");
		puts("- inputbox\t\t A encapsulation of inputbox.");
		puts("- coloredEcho\t\t Display a sentence with specific color.");
		puts("- ntraiseharderror\t A encapsulation of NtRaiseHardError.");

		puts("- ");
		puts("- **********************************************************");
		puts("- --- Some help text about more parameter switches here. ---");
		puts("- **********************************************************");

		puts("- ");
		puts("- You can use 'batventi functionName help' to see about help info of other functions.");
		puts("- ");
		puts("- --quiet\t\t Hide all prompt sentences for user to read.");
		puts("- --help\t\t Show this help text (Equal to /?).");
		puts("- --version\t\t See about version infomation.");



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

