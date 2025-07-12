#pragma once

// 这里包含一些用来遵循 batventi 设计规范的函数和别的定义
// 如果你有别的文件想用 include 语句添加到这里，或者想给这个头文件里加函数，都可以，往下面写就行
// 但是注意
// 这个头文件里包含的别的文件都应该独立编写
// 不应该这里面包含的两个或者多个文件之间还互相包含
// 更不能这里面包含的文件还反过来包含 btvenlib.h
// 那不就递归包含了，编译不出来了

#define BAD_ARGC 200
#define MALLOC_FAILED 201
// 注意，不应让 main 函数返回“字符串转换函数的返回值”
// 那样的话，没法判断返回的是成功转换的字符串长度，还是这个宏定义。
#define NOT_FOUND 404

#include "hyphen.h"

LPWSTR _MultiByteToWideChar(const UINT CodePage, const char *source);
errno_t __cdecl _mbstowcs_s(size_t * const convertedCharsNum, LPWSTR * const dest, const char *source);
char *specifyParameter(const char *switchN, const char *currPara, const char *nextPara);
char *specifyParameter_multiple(const char **switchNs, int count, const char *currPara, const char *nextPara);

LPWSTR _MultiByteToWideChar(const UINT CodePage, const char *source) {
	unsigned int destLen = MultiByteToWideChar(CP_ACP, 0, source, -1, NULL, 0);
	LPWSTR dest = (LPWSTR)malloc(sizeof(WCHAR)*(destLen));
	if (dest == NULL) {
		putsHyphen("Error from func _MultiByteToWideChar in header file btvenlib.h: malloc for dest returned NULL");
		return NULL;
	} else {
		MultiByteToWideChar(CodePage, 0, source, -1, dest, destLen);
		return dest;
	}
}

errno_t __cdecl _mbstowcs_s(size_t * const convertedCharsNum, LPWSTR * const dest, const char *source) {
	*dest = (LPWSTR)malloc(sizeof(WCHAR)*(strlen(source) + 1));
	if (*dest == NULL) {
		putsHyphen("Error from func _mbstowcs_s in header file btvenlib.h: malloc for dest returned a NULL");
		return MALLOC_FAILED;
	}
	return mbstowcs_s(convertedCharsNum, *dest, strlen(source) + 1, source, (strlen(source) + 1));
	// 根据微软文档，这个函数的返回值只可能是 {0,22,34}
}

char *specifyParameter(const char *switchN, const char *currPara, const char *nextPara) {

}

char *specifyParameter_multiple(const char **switchNs, int count, const char *currPara, const char *nextPara) {
	
}