#pragma once

// ���ͷ�ļ�������������������� batventi �������� stdout ����淶�ĺ���
// ��д����ļ���ʱ������� btvenlib.h ���ͷ�ļ�����ֱ�Ӱ������ hyphen.h
// ���ļ��ѱ� btvenlib.h �����������ڴ��ļ��а��� btvenlib.h
// ��� hyphen.h ������������ʵ�֡����������'-'��ͷ���ĺ�������д����ļ���ʱ�򶼿��԰�����
// ˵����������Ŀ���ڹ��ܺͲ���Ĺ淶Ҫ��Ӧ��Ҳ����һ��Ҫ���˰�
// ����ÿһ���������������и��û����Ķ�Ӧ����'-'��ͷ������ͷ����'-'�ľ��������� cmd �� for /f ��ȡ�����ġ�

// �������ļ�����ֱ�ӱ��ϲ��� btvenlib.h ����


#include <stdio.h>

int putsHyphen(const char *str) {
	/*
	����������ô��������Ȳ�����ô�Ȼ���
	Ȼ����� "- "
	�ٿ�ʼ���ַ��������������ÿ�������з����ٲ�һ��"- "
	���ͺͱ�׼��puts()һ����������ô����ܵ�׷��һ�����з�
	�� putsHyphenApd() ��������������Ȼ�һ����˵��Ȼ���ٿ�ʼ����ַ���
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


int putsHyphenApd(const char *str) {
	/*
	����������ô����Ƕ��ַ��������������ÿ�������з����ٲ�һ��"- "
	���ͺͱ�׼��puts()һ����������ô����ܵ�׷��һ�����з�
	�� putsHyphen() ������������Ȳ��Ȼ��У�ֱ��������ַ�����
	��������������Ļ�������ڵ��õĴ�������ַ����Ŀ�ͷ���Լ����� "- "
	�Է�����Ȼ���Ǵ����׿�ʼ����ġ���������п����� for /f ������һ�п�ͷ���� - ��Ȼ������ö����ı�
	*/
	int lengthPut = 0;
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