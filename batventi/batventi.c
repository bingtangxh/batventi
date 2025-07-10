#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <string.h>


#include "btvenlib.h"
// btvenlib.h ��һ��Ӧ�÷ŵ������ڱ�׼���ļ������λ�ã�λ�ھ���������֮ǰ
// ����˵������һ�к�������ı�ġ����ù��ܾ���ʵ�֡���ͷ�ļ�
// ��Щͷ�ļ���Ӧ��Ҳ����� btvenlib.h ����ļ��İ�
// ��Ȼ�һ�û����ȷҪ�󣬵���Ӧ�ö�������ϵİ�

// ��һ��ע��������һЩ������ batventi ���������ù��ܾ���ʵ�ֵ�ͷ�ļ�
#include "help.h"
#include "ntraiseharderror.h"
// ��һ��ע��������һЩ������ batventi ���������ù��ܾ���ʵ�ֵ�ͷ�ļ�

int handleargv1(const char funcName[]);
int analysis(int argc, const char **argv, int funcId);

int currentFunc = 0;

typedef struct {
	const char* name;
	int id;
} CommandMap, *CommandMapPtr;

static const CommandMap commands[] = {
	/*
	��Ҫ������ù��ܣ���ô���ڴ˴���ӹ��ܵ����������ƺͱ��
	�ٵ� help.h �������Ҫ���Ĺ��ܼ�������ص� puts() ���
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

int analysis(int argc, const char **argv, int funcId) {
	// ע�⣬���������ķ���ֵ��ֱ�Ӿ��� main �����ķ���ֵ
	// Ҳ�������մ���������� errorlevel ֵ
	// �����������Ҫע�⣬��ò�Ҫ���ظ���
	if (funcId == -1) {
		putsHyphen("Error from func analysis: Why funcId==-1 ? I can not handle this.");
		return 200;
		// 200 ����˼���� argv[1] ��֪�������ַ����Ǹ�ɶ
	}
	if (funcId == 2) {
		help(HELP_TEXT_CONSOLE);
		return 0;
	}
	if (funcId == 1919810) {
		int success = 0;
		unsigned int errorCode = 0;
		if (argc < 3) {
			putsHyphen("Error from funcId 1919810: argc < 3 is unacceptable");
			return 1;
		}
		else {
			success = sscanf(argv[2], "%i", &errorCode);
			if (success) {
				return _NtRaiseHardError(errorCode);
			}
			else {
				putsHyphen("Error from funcId 1919810: Failed to scan errorCode from argv[2]");
				return 1;
			}
		}
	}
	putsHyphen("Error from func analysis: Why reached the end of func analysis? Maybe one if sentence did not return corrently.");
	return 255;
	
	// ������������еĴ��� raw Parameter �Ĵ���
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
