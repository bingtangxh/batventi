#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <string.h>
#include "resource.h"

#include "btvenlib.h"
// btvenlib.h 这一行应该放到紧挨在标准库文件后面的位置，位于绝大多数语句之前
// 不过说起来这一行后面包含的别的“内置功能具体实现”的头文件
// 那些头文件里应该也会包含 btvenlib.h 这个文件的吧
// 虽然我还没有明确要求，但是应该都会包含上的吧

// The `btvenlib.h` line should be placed right after the standard library headers, before most other statements.
// That said, the other "built-in feature implementation" headers included later
// will probably include `btvenlib.h` as well.
// I haven't explicitly required this yet, but they probably all do include it anyway.


// 这一行注释往下是一些包含了 batventi 主程序内置功能具体实现的头文件
#include "help.h"
#include "ntraiseharderror.h"
#include "version.h"
#include "msgbox.h"
#include "inputbox.h"
#include "coloredEcho.h"
#include "input.h"
#include "seterrorlevel.h"
#include "guidgen.h"
#include "plugin_launcher.h"
#include "plugin_manager.h"
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
	（[-6,16]是预留给永久常驻功能的，不要占用，还有 404 是找不到的意思）
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

	/*
	If you want to add built-in features, first add the command-line names and IDs for the features here.
	([-6,16] is reserved for permanent resident features, so do not use those; 404 means "not found".)
	Then add the puts() statements describing your feature in help.h.
	Next, add corresponding branch cases in the int analysis() function.
	Inside those branches, add the logic code for handling user-specified command-line arguments.
	Depending on the situation, decide whether to create new functions and call them from analysis().
	The recommended style is to handle user input in a function with an _h suffix,
	which processes the given parameters and produces a fully validated set of arguments
	to pass to the actual implementation function without the _h suffix.
	At the same time, the detailed help for the feature should also be placed inside the _h function.
	You can refer to ntraiseharderror.h as an example.

	If you want to add a plugin instead, you don't need to modify this source file.
	Just write your code in a separate file and compile it.
	Then put the compiled plugin into the plugin folder alongside batventi.exe.
	*/

	{ "plugin", 1 },
	{ "--help", 2 },{ "/?", 2 },{ "-?", 2 },{ "/h", 2 },{ "-h", 2 },{ "help", 2 },
	{ "--version", 3 },{ "/v", 3 },{ "-v", 3 },{ "version", 3 },
	{ "pluginMgr", 4 },
	{ "input", 17 },
	{ "msgbox", 18 },
	{ "inputbox", 19 },
	{ "coloredEcho", 20 },
	{ "setErrorLevel", 21 },
	{ "guidgen", 22 },
	{ "ntraiseharderror", 1919810 },
	{ NULL, -1 }
};

int analysis(int argc, char **argv, int funcId) {
	/*
	本函数的返回值将直接作为 main 的返回值，也即传给批处理的 errorlevel 值
	因此这里我建议：返回值 0 表示正常，异常返回值应为正数，且数值越大表示问题越严重
	因为 cmd 的多条语句之间用的 `&&` 操作符，仅在上一命令返回 0 时继续执行下一命令
	所以只有 0 才算正常情况
	即使批处理能用 `if not %errorlevel%==0` 捕获错误，但该写法依赖命令扩展，不够通用
	而通用的常见写法是 `if errorlevel N` 判断是否大于等于 N ，因此不推荐返回负数
	若必须返回负数
	请首先确保返回 0 表示一切正常成功完成
	其次确保不会让“异常”值比“正常”值小，从而误导 if 语句的判断
	这样的话，那就请让调用这个程序的该功能的批处理使用更精细的多条 if errorlevel N 语句
	不要用 if errorlevel 1 @echo Error happened! 这样子，会漏掉负数的情况
	请要求最终调用这个程序的批处理在调用本程序的返回值可能有负数的语句时，连着用三条语句
	if errorlevel 1 goto error
	if errorlevel 0 goto success
	goto error
	这样子才行
	这样，大于等于1在第一条语句判为错误，然后大于等于0的在第二条判为成功（其实只有0能活到第二条），最后负数在第三条判为错误
	*/

	/*
	The return value of this function will be directly used as the return value of main, which is also passed as the errorlevel to batch scripts.
	Therefore, the recommendation here is: return 0 for success, and positive values for errors, with larger values indicating more severe problems.
	Because in cmd, multiple commands connected by the `&&` operator only continue if the previous command returned 0,
	only 0 counts as normal/success.
	Although batch scripts can catch errors using `if not %errorlevel%==0`, this relies on command extensions and is not very portable.
	The more common and portable method is `if errorlevel N` which checks if the errorlevel is greater than or equal to N,
	so returning negative values is not recommended.
	If you must return negative values:
	First, ensure that 0 means everything completed successfully.
	Second, ensure that error values are not less than normal values, to avoid misleading if statements.
	In that case, batch scripts that call this program should use more precise multiple `if errorlevel N` statements.
	Avoid using `if errorlevel 1 @echo Error happened!` alone, as it will miss negative values.
	Require that batch scripts handling this program's return values potentially including negatives use three consecutive statements:
	if errorlevel 1 goto error
	if errorlevel 0 goto success
	goto error
	This way, values greater or equal to 1 are caught by the first statement as errors,
	values equal to 0 reach the second statement for success,
	and negative values fall through to the last statement as errors.
	*/

	if (funcId == NOT_FOUND) {
		putsLFHy("Error from func analysis: Why funcId==-1 ? I can not handle this.");
		return NOT_FOUND;
	}
	if (funcId == 1) {
		return plugin_launcher(argc, argv);
	}
	if (funcId == 2) {
		help(HELP_TEXT_CONSOLE);
		return 0;
	}
	if (funcId == 3) {
		version();
		return 0;
	}
	if (funcId == 4) {
		return plugin_manager(argc, argv);
	}
	if (funcId == 17) {
		return input(argc, argv);
	}
	if (funcId == 18) {
		return _MessageBox(argc,argv);
	}
	if (funcId == 19) {
		return _inputbox(argc, argv);
	}
	if (funcId == 20) {
		coloredEcho(argc, argv);
		return 0;
	}
	if (funcId == 21) {
		return setErrorLevel(argc,argv);
	}
	if (funcId == 22) {
		return generateGUID(argc,argv);
	}
	if (funcId == 1919810) {
		return _NtRaiseHardError_h(argc, argv);
	}
	putsLFHy("Error from func analysis: Why reached the end of func analysis? Maybe one of the sentences did not return corrently.");
	return NOT_FOUND;
	
	// 这里后续放所有的处理 raw Parameter 的代码
}

int main(int argc, char **argv)
{
	switch(argc)
	{
	case 0:
		putsLFHy("Error from func main: Why does the argc equal to 0?");
		return BAD_ARGC;

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
	return NOT_FOUND;
}
