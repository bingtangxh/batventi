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
	若要添加内置功能，那么先在此处添加功能的命令行名称和编号
	再到 void help() 里添加你要做的功能及描述相关的 puts() 语句
	然后在 int analysis() 函数中添加对应的分支语句
	里面再塞上“用于处理用户指定的命令行参数的逻辑代码”
	视具体情况决定要不要再开函数，并从 analysis() 调用。

	若添加的是插件，那么就不用动这个源代码文件了，另开文件写代码编译
	编译出来扔进 batventi.exe 一起的 plugin 文件夹里面就行
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
			L"本程序只能在命令行中运行。\n"
			L"This program can only be run in console mode.\n"
			L"Use --help or /? to see about the usage.";
		/*LPWSTR wCanOnlyConsole = (LPWSTR)malloc(sizeof(wchar_t)*(strlen(canOnlyConsole) + 1));
		if (wCanOnlyConsole == NULL) {
			putsHyphen("Error from func help case HELP_TEXT_MSGBOX: malloc for wCanOnlyConsole returned a NULL");
			break;
		}*/
		//size_t convertedCharsNum = 0;
		//// 这个就用来给 mbstowcs 存储记录成功写入多少个字符用的
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
		// 此处的 %dte nandayo? 出自原神派蒙的“‘欸嘿’是什么意思啊？”的日语「『えへっ』てなんだよ…！！」
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
	// 这里后续放所有的处理 raw Parameter 的代码
}

int putsHyphen(const char *str) {
	/*
	这个函数的用处就是首先不管怎么先换行
	然后输出 "- "
	再开始读字符串并进行输出，每读到换行符都再补一个"- "
	最后就和标准的puts()一样，不管怎么最后总得追加一个换行符
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