#pragma once

// 这里包含一些用来遵循 batventi 设计规范的函数和别的定义
// 如果你有别的文件想用 include 语句添加到这里，或者想给这个头文件里加函数，都可以，往下面写就行
// 但是注意
// 这个头文件里包含的别的文件都应该独立编写
// 不应该这里面包含的两个或者多个文件之间还互相包含
// 更不能这里面包含的文件还反过来包含 btvenlib.h
// 那不就递归包含了，编译不出来了
// 虽然说这里的头文件都应该是有 #pragma once 参数的，但还是要注意

// This file contains some functions and definitions meant to follow the BatVenti design conventions.
// If you want to add other files using `#include` here, or add new functions to this header file, go ahead—just add them below.
// However, be careful:
// Every file included in this header must be written independently.
// Files included here should not include each other.
// More importantly, files included here must never include `btvenlib.h` back.
// Otherwise, you'll get recursive includes, and the code won't compile.
// Although all headers here are expected to use `#pragma once`, it's still important to pay attention.

#define BAD_ARGC 200
#define MALLOC_FAILED 201
// 注意，不应让 main 函数返回“字符串转换函数的返回值”
// 那样的话，没法判断返回的是成功转换的字符串长度，还是这些错误码的宏定义。
// Note: Do not make the main function return the result of a "string conversion function".
// Otherwise, it's impossible to tell whether 
// the return value represents the length of a successful conversion
// or one of the error code macros.
#define NOT_FOUND 404
#include <stdbool.h>
#include "hyphen.h"
#pragma comment(lib, "Advapi32.lib")  
// Advapi32 用于 OpenProcessToken, AdjustTokenPrivileges, LookupPrivilegeValueA
#pragma comment(lib, "User32.lib")    
// User32 用于 ExitWindowsEx, ShowWindow, GetForegroundWindow
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
	// 根据微软文档，这个函数的返回值只可能是 {0,22,34}
}

const char *specifyParameter(const char *switchN, const char *currPara, const char *nextPara, int *errCode) {
	const char *format1Starting[] = { "--",NULL };
	const char *format1Ending[] = { ":","=",NULL };
	const char *format2Starting[] = { "-","/",NULL };
	char *currentVerifyStrB = NULL;
	size_t StrB_len = 0;
	_Bool isSameThing = FALSE;
	// switchN 的意思是“开关名称”，也是 StrA 的一个作用，而 StrB 用于临时记录当前用于验证的基准字符串
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
	specifyParameter_multiple() 在循环语句当中调用的话要注意
	============================================================
	这个函数它每次试图从 argv 中获取某个参数时，会将成功匹配到的参数内容清空（弄成 ""），避免重复使用。
	因此，它不应该被写在循环语句内部连续调用。

	因为，若你在一个循环中多次使用同一个接收变量和同样的参数去调用它，例如：
	while (condition) { receiver = specifyParameter_multiple(...); }
	那么如果后面某次调用没有匹配成功，返回 NULL，就会把 receiver 前面成功赋的值“覆盖掉”。

	此外，该函数对 argv[i] 或 argv[i+1] 赋值为空字符串（""），用于标记已经读出来过的参数。
	所以如果你后续调用 sscanf 等函数读取参数，遇到空字符串，也可能是此函数提前清空了。
	调用它之后应特别注意 argv[i] 可能已被清空的情况。

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