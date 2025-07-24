#pragma once

// �������һЩ������ѭ batventi ��ƹ淶�ĺ����ͱ�Ķ���
// ������б���ļ����� include �����ӵ��������������ͷ�ļ���Ӻ����������ԣ�������д����
// ����ע��
// ���ͷ�ļ�������ı���ļ���Ӧ�ö�����д
// ��Ӧ��������������������߶���ļ�֮�仹�������
// ������������������ļ������������� btvenlib.h
// �ǲ��͵ݹ�����ˣ����벻������
// ��Ȼ˵�����ͷ�ļ���Ӧ������ #pragma once �����ģ�������Ҫע��

// This file contains some functions and definitions meant to follow the BatVenti design conventions.
// If you want to add other files using `#include` here, or add new functions to this header file, go ahead��just add them below.
// However, be careful:
// Every file included in this header must be written independently.
// Files included here should not include each other.
// More importantly, files included here must never include `btvenlib.h` back.
// Otherwise, you'll get recursive includes, and the code won't compile.
// Although all headers here are expected to use `#pragma once`, it's still important to pay attention.

#define BAD_ARGC 200
#define MALLOC_FAILED 201
// ע�⣬��Ӧ�� main �������ء��ַ���ת�������ķ���ֵ��
// �����Ļ���û���жϷ��ص��ǳɹ�ת�����ַ������ȣ�������Щ������ĺ궨�塣
// Note: Do not make the main function return the result of a "string conversion function".
// Otherwise, it's impossible to tell whether 
// the return value represents the length of a successful conversion
// or one of the error code macros.
#define NOT_FOUND 404
#include <stdbool.h>
#include "hyphen.h"
#pragma comment(lib, "Advapi32.lib")  
// Advapi32 ���� OpenProcessToken, AdjustTokenPrivileges, LookupPrivilegeValueA
#pragma comment(lib, "User32.lib")    
// User32 ���� ExitWindowsEx, ShowWindow, GetForegroundWindow
#pragma comment(lib, "Kernel32.lib")

LPWSTR _MultiByteToWideChar(const UINT CodePage, const char *source);
errno_t __cdecl _mbstowcs_s(size_t * const convertedCharsNum, LPWSTR * const dest, const char *source);
const char *specifyParameter(const char *switchN, const char *currPara, const char *nextPara, int *errCode);
const char *specifyParameter_multiple(const char **switchNs, int count, const char *currPara, const char *nextPara, int *errCode);
const UINT getCodePagefromPara(int argc, char **argv);

LPWSTR _MultiByteToWideChar(const UINT CodePage, const char *source) {
	unsigned int destLen = MultiByteToWideChar(CP_ACP, 0, source, -1, NULL, 0);
	LPWSTR dest = (LPWSTR)malloc(sizeof(WCHAR)*(destLen));
	if (dest == NULL) {
		putsLFHy("Error from func _MultiByteToWideChar in header file btvenlib.h: malloc for dest returned NULL");
		return NULL;
	} else {
		MultiByteToWideChar(CodePage, 0, source, -1, dest, destLen);
		return dest;
	}
}

errno_t __cdecl _mbstowcs_s(size_t * const convertedCharsNum, LPWSTR * const dest, const char *source) {
	*dest = (LPWSTR)malloc(sizeof(WCHAR)*(strlen(source) + 1));
	if (*dest == NULL) {
		putsLFHy("Error from func _mbstowcs_s in header file btvenlib.h: malloc for dest returned a NULL");
		return MALLOC_FAILED;
	}
	return mbstowcs_s(convertedCharsNum, *dest, strlen(source) + 1, source, (strlen(source) + 1));
	// ����΢���ĵ�����������ķ���ֵֻ������ {0,22,34}
}

const char *specifyParameter(const char *switchN, const char *currPara, const char *nextPara, int *errCode) {
	const char *format1Starting[] = { "--",NULL };
	const char *format1Ending[] = { ":","=",NULL };
	const char *format2Starting[] = { "-","/",NULL };
	char *currentVerifyStrB = NULL;
	size_t StrB_len = 0;
	_Bool isSameThing = FALSE;
	// switchN ����˼�ǡ��������ơ���Ҳ�� StrA ��һ�����ã��� StrB ������ʱ��¼��ǰ������֤�Ļ�׼�ַ���
	for (int i1 = 0; format1Starting[i1] != NULL; i1++) {
		for (int i2 = 0; format1Ending[i2] != NULL; i2++) {
			StrB_len = strlen(format1Starting[i1]) + strlen(format1Ending[i2]) + strlen(switchN) + 1;
			currentVerifyStrB = (char *)malloc(sizeof(char)*(StrB_len));
			if (currentVerifyStrB == NULL) {
				putsLFHy("Error from func specifyParameter in header file btvenlib.h: malloc for currentVerifyStrB returned a NULL");
				*errCode = MALLOC_FAILED;
				return NULL;
			}
			strcpy_s(currentVerifyStrB, StrB_len, format1Starting[i1]);
			strcat_s(currentVerifyStrB, StrB_len, switchN);
			strcat_s(currentVerifyStrB, StrB_len, format1Ending[i2]);
			// printf("%s\t%s\t%d\t%s\n",currPara, currentVerifyStrB, StrB_len-1, _strnicmp(currPara, currentVerifyStrB,StrB_len-1)?"FALSE":"TRUE");
			if (!_strnicmp(currPara, currentVerifyStrB,StrB_len-1)) {
				isSameThing = TRUE;
				free(currentVerifyStrB);
				*errCode = 0;
				// const char *temp = currPara + sizeof(char)*(StrB_len-1);
				return currPara + sizeof(char)*(StrB_len-1);
			}
			else {
				free(currentVerifyStrB);
				continue;
			}
		}
		continue;
	}
	for (int i1 = 0; format2Starting[i1] != NULL; i1++) {
		StrB_len = strlen(format2Starting[i1]) + strlen(switchN) + 1;
		currentVerifyStrB = (char *)malloc(sizeof(char)*(StrB_len));
		if (currentVerifyStrB == NULL) {
			putsLFHy("Error from func specifyParameter in header file btvenlib.h: malloc for currentVerifyStrB returned a NULL");
			*errCode = MALLOC_FAILED;
			return NULL;
		}
		strcpy_s(currentVerifyStrB, StrB_len, format2Starting[i1]);
		strcat_s(currentVerifyStrB, StrB_len, switchN);
		if (!_stricmp(currPara, currentVerifyStrB)) {
			isSameThing = TRUE;
			free(currentVerifyStrB);
			*errCode = 1;
			return nextPara;
		}
		else {
			free(currentVerifyStrB);
			continue;
		}
	}
	*errCode = NOT_FOUND;
	return NULL;
}

const char *specifyParameter_multiple(const char **switchNs, int count, const char *currPara, const char *nextPara, int *errCode) {
	/*
	============================================================
	specifyParameter_multiple() ��ѭ����䵱�е��õĻ�Ҫע��
	============================================================
	���������ÿ����ͼ�� argv �л�ȡĳ������ʱ���Ὣ�ɹ�ƥ�䵽�Ĳ���������գ�Ū�� ""���������ظ�ʹ�á�
	��ˣ�����Ӧ�ñ�д��ѭ������ڲ��������á�

	��Ϊ��������һ��ѭ���ж��ʹ��ͬһ�����ձ�����ͬ���Ĳ���ȥ�����������磺
	while (condition) { receiver = specifyParameter_multiple(...); }
	��ô�������ĳ�ε���û��ƥ��ɹ������� NULL���ͻ�� receiver ǰ��ɹ�����ֵ�����ǵ�����

	���⣬�ú����� argv[i] �� argv[i+1] ��ֵΪ���ַ�����""�������ڱ���Ѿ����������Ĳ�����
	���������������� sscanf �Ⱥ�����ȡ�������������ַ�����Ҳ�����Ǵ˺�����ǰ����ˡ�
	������֮��Ӧ�ر�ע�� argv[i] �����ѱ���յ������

	============================================================
	Attention while calling it in a loop
	============================================================
	Do NOT call this function repeatedly inside a loop using the same receiver variable and the same set of paraemters.

	This function consumes matched argv entries by clearing them (setting to "").
	This prevents duplicate detection, but may cause side effects in loops.

	For example:
	while (condition) { receiver = specifyParameter_multiple(...); }

	In this pattern, a failed match in a later iteration returns NULL,
	which will overwrite `receiver`, even if it previously held a valid result.

	Also, since this function clears argv[i] and argv[i+1], any later parsing
	(e.g., via sscanf or strcmp) should expect possibly empty strings.
	*/

	*errCode = 0;
	const char *result = NULL;
	for (int i = 0; i < count; i++) {
		result = specifyParameter(switchNs[i], currPara, nextPara, errCode);
		// printf("- %s\t%d\t%s\t%s\t%d\n", switchNs[i], i, currPara, nextPara, *errCode);
		if (result != NULL) {
			return result;
		}
		else {
			switch (*errCode)
			{
			case MALLOC_FAILED:
				putsLFHy("Error from func specifyParameter_multiple in header file btvenlib.h: func specifyParameter returned a NULL");
				return NULL;
			case NOT_FOUND:
				continue;
			default:
				putsLFHy("Error from func specifyParameter_multiple in header file btvenlib.h: Unexpected errCode to switch");
				return NULL;
			}
		}
	}
	*errCode = NOT_FOUND;
	return NULL;
}

const UINT getCodePagefromPara(int argc, char **argv) {
	int current = 0, elemsGotten = 0;
	UINT CodePage = CP_ACP;
	int errCode = 0;
	const char *specResult = NULL;
	const char *codePage_Alias[] = { "encoding","codepage" };
	
	for (int i = 0; i < argc; i++) {
		specResult = specifyParameter_multiple(codePage_Alias, 2, argv[i], argv[i + 1], &errCode);
		if (specResult != NULL) {
			switch (errCode) {
			case 1:
				strcpy_s(argv[i], strlen(argv[i]) + 1, "");
				i++;
			case 0:
				elemsGotten = sscanf(specResult, "%i", &CodePage);
				// printf("- %d \"%s\"\n", CodePage, specResult);
				strcpy_s(argv[i], strlen(argv[i]) + 1, "");
				switch(elemsGotten)
				{
				case 0:
					putsLFHy("Warning from func getCodePagefromPara in header file btvenlib.h: Could not scan for CodePage from encoding parameter, I will use CP_ACP.");
					CodePage = CP_ACP;
					break;
				case 1:
					// printf("- %d \"%s\"\n", CodePage, specResult);
					return CodePage;
				default:
					printf("- Warning from func getCodePagefromPara in header file btvenlib.h: Why elemsGotten == %d while specResult == %p ? I will use CP_ACP.\n", elemsGotten, specResult);
					return CP_ACP;
				}
				continue;
			default:
				printf("- Warning from func getCodePagefromPara in header file btvenlib.h: Why errCode == %d while specResult == %p ? I will use CP_ACP.\n", errCode, specResult);
				return CP_ACP;
			}
		}
		else {
			switch (errCode) {
			case MALLOC_FAILED:
				putsLFHy("Warning from func getCodePagefromPara in header file btvenlib.h: Func specifyParameter_multiple set errCode to MALLOC_FAILED. I will use CP_ACP.");
				return CP_ACP;
			case NOT_FOUND:
				continue;
			default:
				printf("- Warning from func getCodePagefromPara in header file btvenlib.h: Why errCode == %d while specResult == NULL ? I will use CP_ACP.\n", errCode);
				return CP_ACP;
			}
		}
	}
	return CP_ACP;
}