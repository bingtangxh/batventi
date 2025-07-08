#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <string.h>


#define HELP_TEXT_MAX_LEN 114514
#define HELP_TEXT_CONSOLE 1
#define HELP_TEXT_MSGBOX 2
void help(int type);
int handleargv1(const char funcName[]);
int analysis(int argc, const char **argv, int funcId);
int putsHyphen(const char *str);

int currentFunc = 0;

typedef struct {
	const char* name;
	int id;
} CommandMap, *CommandMapPtr;

static const CommandMap commands[] = {
	/*
	��Ҫ������ù��ܣ���ô���ڴ˴���ӹ��ܵ����������ƺͱ��
	�ٵ� void help() �������Ҫ���Ĺ��ܼ�������ص� puts() ���
	Ȼ���� int analysis() ��������Ӷ�Ӧ�ķ�֧���
	���������ϡ����ڴ����û�ָ���������в������߼����롱
	�Ӿ����������Ҫ��Ҫ�ٿ����������� analysis() ���á�

	����ӵ��ǲ������ô�Ͳ��ö����Դ�����ļ��ˣ����ļ�д�������
	��������ӽ� batventi.exe һ��� plugin �ļ����������
	*/
	{ "plugin", 1 },
	{ "--help", 2 },{ "/?", 2 },{ "-?", 2 },{ "/h", 2 },
	{ "--version", 3 },{ "/v", 3 },{ "-v", 3 },
	{ "input", 17 },
	{ "msgbox", 18 },
	{ "inputbox", 19 },
	{ "coloredEcho", 20 },
	{ "ntraiseharderror", 1919810 },
	{ NULL, -1 }
};

int main(int argc, char **argv)
{
	switch(argc)
	{
	case 0:
		putsHyphen("Error from func main: Why does the argc equal to 0?");
		return 255;

	case 1:
		help(HELP_TEXT_MSGBOX);
		return 0;
	
	default:
		currentFunc = handleargv1(argv[1]);
		// printf("- %d\n", currentFunc);
		analysis(argc, argv, currentFunc);
		return 0;
	}
}

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
			L"This program can only be run in console mode.\n"
			L"Use --help or /? to see about the usage.";
		/*LPWSTR wCanOnlyConsole = (LPWSTR)malloc(sizeof(wchar_t)*(strlen(canOnlyConsole) + 1));
		if (wCanOnlyConsole == NULL) {
			putsHyphen("Error from func help case HELP_TEXT_MSGBOX: malloc for wCanOnlyConsole returned a NULL");
			break;
		}*/
		//size_t convertedCharsNum = 0;
		//// ����������� mbstowcs �洢��¼�ɹ�д����ٸ��ַ��õ�
		//mbstowcs_s(&convertedCharsNum,wCanOnlyConsole, strlen(helpText) + 1,helpText, (strlen(helpText) + 1));
		MessageBox(NULL,canOnlyConsole, L"Batventi Double-clicked Help", MB_OK | MB_ICONINFORMATION);
		break;
	}
	case HELP_TEXT_CONSOLE:
	{
		putsHyphen(helpText);
		puts("Usage:\n");
		puts("batventi functionName [otherParameters] [--quiet] [--help] [--version]\n");
		puts("otherParameters can be the names below:");
		puts("input\tAn advanced, robust and smart input method.");
		puts("msgbox\t A encapsulation of MessageBox.");
		puts("inputbox\t A encapsulation of inputbox.");
		puts("coloredEcho\t Display a sentence with specific color.");
		puts("ntraiseharderror\t A encapsulation of NtRaiseHardError.");


		puts("\n**********************************************************");
		puts("--- Some help text about more parameter switches here. ---");
		puts("**********************************************************\n");

		puts("You can use 'batventi functionName help' to see about help info of other functions.");
		puts("--quiet\tHide all prompt sentences for user to read.");
		puts("--help\tShow this help text (Equal to /?).");
		puts("--version\tSee about version infomation.");


		
		break;
	}
	default:
	{
		printf("- Error from func help: How do you mean type %d? %dte nandayo?\n",type,type);
		// �˴��� %dte nandayo? ����ԭ�����ɵġ����G�١���ʲô��˼����������������ؤá��Ƥʤ���衭������
		break;
	}
	}
}

int handleargv1(const char funcName[]) {
	for (int i = 0; commands[i].name != NULL; i++) {
		if (_stricmp(funcName, commands[i].name) == 0) {
			return commands[i].id;
		}
	}
	return -1;
}

int analysis(int argc, const char **argv, int funcId) {
	if (funcId == -1) {
		putsHyphen("Error from func analysis: Why funcId==-1 ? I can not handle this.");
		return -1;
	}
	if (funcId == 2) {
		help(HELP_TEXT_CONSOLE);
		return 0;
	}
	
		return 0;
	// ������������еĴ��� raw Parameter �Ĵ���
}

int putsHyphen(const char *str) {
	/*
	����������ô��������Ȳ�����ô�Ȼ���
	Ȼ����� "- "
	�ٿ�ʼ���ַ��������������ÿ�������з����ٲ�һ��"- "
	���ͺͱ�׼��puts()һ����������ô����ܵ�׷��һ�����з�
	*/
	int lengthPut = 3;
	putchar('\n');
	putchar('-');
	putchar(' ');
	for (int i = 0; str[i] != '\0'; i++) {
		if (str[i] != '\n') {
			putchar(str[i]);
			lengthPut++;
			continue;
		}
		else {
			putchar(str[i]);
			putchar('-');
			putchar(' ');
			lengthPut += 3;
			continue;
		}
	}
	putchar('\n');
	lengthPut++;
	return lengthPut;
}