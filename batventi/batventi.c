#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <string.h>


#include "btvenlib.h"
// btvenlib.h 这一行应该放到紧挨在标准库文件后面的位置，位于绝大多数语句之前
// 不过说起来这一行后面包含的别的“内置功能具体实现”的头文件
// 那些头文件里应该也会包含 btvenlib.h 这个文件的吧
// 虽然我还没有明确要求，但是应该都会包含上的吧

// 这一行注释往下是一些包含了 batventi 主程序内置功能具体实现的头文件
#include "help.h"
#include "ntraiseharderror.h"
// 这一行注释往上是一些包含了 batventi 主程序内置功能具体实现的头文件

int handleargv1(const char funcName[]);
int analysis(int argc, char **argv, int funcId);

int currentFunc = 0;

typedef struct {
	const char* name;
	int id;
} CommandMap, *CommandMapPtr;

static const CommandMap commands[] = {
	/*
	若要添加内置功能，那么先在此处添加功能的命令行名称和编号
	再到 help.h 里添加你要做的功能及描述相关的 puts() 语句
	然后在 int analysis() 函数中添加对应的分支语句
	里面再塞上“用于处理用户指定的命令行参数的逻辑代码”
	视具体情况决定要不要再开函数，并从 analysis() 调用。
	建议的写法是，用户输入处理作为一个函数，带上 _h 后缀
	负责处理用户给定的参数，最终得出一组完全合格的参数传给真正实现功能的函数
	然后不带 _h 后缀的就是真正实现功能的函数
	与此同时，功能具体帮助建议也放进 _h 函数里
	具体可以参照 ntraiseharderror.h 这个当个示范

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

int analysis(int argc, char **argv, int funcId) {
	// 注意，这个函数里的返回值，直接就是 main 函数的返回值
	// 也就是最终传给批处理的 errorlevel 值
	// 所以这里务必要注意，最好不要返回负数
	if (funcId == -1) {
		putsHyphen("Error from func analysis: Why funcId==-1 ? I can not handle this.");
		return 200;
		// 200 的意思就是 argv[1] 不知道给的字符串是个啥
	}
	if (funcId == 2) {
		help(HELP_TEXT_CONSOLE);
		return 0;
	}
	if (funcId == 1919810) {
		return _NtRaiseHardError_h(argc, argv);
	}
	putsHyphen("Error from func analysis: Why reached the end of func analysis? Maybe one if sentence did not return corrently.");
	return 255;
	
	// 这里后续放所有的处理 raw Parameter 的代码
}

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
		return analysis(argc, argv, currentFunc);
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
