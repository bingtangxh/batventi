#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <string.h>
#include "resource.h"

#include "btvenlib.h"
// btvenlib.h ��һ��Ӧ�÷ŵ������ڱ�׼���ļ������λ�ã�λ�ھ���������֮ǰ
// ����˵������һ�к�������ı�ġ����ù��ܾ���ʵ�֡���ͷ�ļ�
// ��Щͷ�ļ���Ӧ��Ҳ����� btvenlib.h ����ļ��İ�
// ��Ȼ�һ�û����ȷҪ�󣬵���Ӧ�ö�������ϵİ�

// The `btvenlib.h` line should be placed right after the standard library headers, before most other statements.
// That said, the other "built-in feature implementation" headers included later
// will probably include `btvenlib.h` as well.
// I haven't explicitly required this yet, but they probably all do include it anyway.


// ��һ��ע��������һЩ������ batventi ���������ù��ܾ���ʵ�ֵ�ͷ�ļ�
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
// ��һ��ע��������һЩ������ batventi ���������ù��ܾ���ʵ�ֵ�ͷ�ļ�

int handleargv1(const char funcName[]);
int analysis(int argc, char **argv, int funcId);

int currentFunc = 0;

typedef struct {
	const char* name;
	int id;
} CommandMap, *CommandMapPtr;

static const CommandMap commands[] = {
	/*
	��Ҫ������ù��ܣ���ô���ڴ˴���ӹ��ܵ����������ƺͱ��
	��[-6,16]��Ԥ�������ó�פ���ܵģ���Ҫռ�ã����� 404 ���Ҳ�������˼��
	�ٵ� help.h �������Ҫ���Ĺ��ܼ�������ص� puts() ���
	Ȼ���� int analysis() ��������Ӷ�Ӧ�ķ�֧���
	���������ϡ����ڴ����û�ָ���������в������߼����롱
	�Ӿ����������Ҫ��Ҫ�ٿ����������� analysis() ���á�
	�����д���ǣ��û����봦����Ϊһ������������ _h ��׺
	�������û������Ĳ��������յó�һ����ȫ�ϸ�Ĳ�����������ʵ�ֹ��ܵĺ���
	Ȼ�󲻴� _h ��׺�ľ�������ʵ�ֹ��ܵĺ���
	���ͬʱ�����ܾ����������Ҳ�Ž� _h ������
	������Բ��� ntraiseharderror.h �������ʾ��

	����ӵ��ǲ������ô�Ͳ��ö����Դ�����ļ��ˣ����ļ�д�������
	��������ӽ� batventi.exe һ��� plugin �ļ����������
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
	�������ķ���ֵ��ֱ����Ϊ main �ķ���ֵ��Ҳ������������� errorlevel ֵ
	��������ҽ��飺����ֵ 0 ��ʾ�������쳣����ֵӦΪ����������ֵԽ���ʾ����Խ����
	��Ϊ cmd �Ķ������֮���õ� `&&` ��������������һ����� 0 ʱ����ִ����һ����
	����ֻ�� 0 �����������
	��ʹ���������� `if not %errorlevel%==0` ������󣬵���д������������չ������ͨ��
	��ͨ�õĳ���д���� `if errorlevel N` �ж��Ƿ���ڵ��� N ����˲��Ƽ����ظ���
	�����뷵�ظ���
	������ȷ������ 0 ��ʾһ�������ɹ����
	���ȷ�������á��쳣��ֵ�ȡ�������ֵС���Ӷ��� if �����ж�
	�����Ļ����Ǿ����õ����������ĸù��ܵ�������ʹ�ø���ϸ�Ķ��� if errorlevel N ���
	��Ҫ�� if errorlevel 1 @echo Error happened! �����ӣ���©�����������
	��Ҫ�����յ������������������ڵ��ñ�����ķ���ֵ�����и��������ʱ���������������
	if errorlevel 1 goto error
	if errorlevel 0 goto success
	goto error
	�����Ӳ���
	���������ڵ���1�ڵ�һ�������Ϊ����Ȼ����ڵ���0���ڵڶ�����Ϊ�ɹ�����ʵֻ��0�ܻ�ڶ�������������ڵ�������Ϊ����
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
	
	// ������������еĴ��� raw Parameter �Ĵ���
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
