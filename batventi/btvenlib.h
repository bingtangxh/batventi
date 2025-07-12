#pragma once

// �������һЩ������ѭ batventi ��ƹ淶�ĺ����ͱ�Ķ���
// ������б���ļ����� include �����ӵ��������������ͷ�ļ���Ӻ����������ԣ�������д����
// ����ע��
// ���ͷ�ļ�������ı���ļ���Ӧ�ö�����д
// ��Ӧ��������������������߶���ļ�֮�仹�������
// ������������������ļ������������� btvenlib.h
// �ǲ��͵ݹ�����ˣ����벻������

#define BAD_ARGC 200
#define MALLOC_FAILED 201
// ע�⣬��Ӧ�� main �������ء��ַ���ת�������ķ���ֵ��
// �����Ļ���û���жϷ��ص��ǳɹ�ת�����ַ������ȣ���������궨�塣
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
	// ����΢���ĵ�����������ķ���ֵֻ������ {0,22,34}
}

char *specifyParameter(const char *switchN, const char *currPara, const char *nextPara) {

}

char *specifyParameter_multiple(const char **switchNs, int count, const char *currPara, const char *nextPara) {
	
}