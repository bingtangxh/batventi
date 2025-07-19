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
	if (funcId == NOT_FOUND) {
		putsHyphen("Error from func analysis: Why funcId==-1 ? I can not handle this.");
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
		return generateGUID();
	}
	if (funcId == 1919810) {
		return _NtRaiseHardError_h(argc, argv);
	}
	putsHyphen("Error from func analysis: Why reached the end of func analysis? Maybe one of the sentences did not return corrently.");
	return NOT_FOUND;
	
	// ������������еĴ��� raw Parameter �Ĵ���
}

int main(int argc, char **argv)
{
	switch(argc)
	{
	case 0:
		putsHyphen("Error from func main: Why does the argc equal to 0?");
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
