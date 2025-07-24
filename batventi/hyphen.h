#pragma once
/*
���ͷ�ļ�������������������� batventi �������� stdout ����淶�ĺ���
��д����ļ���ʱ������� btvenlib.h ���ͷ�ļ�����ֱ�Ӱ������ hyphen.h
���ļ��ѱ� btvenlib.h �����������ڴ��ļ��а��� btvenlib.h
��� hyphen.h ������������ʵ�֡����������'-'��ͷ���ĺ�������д����ļ���ʱ�򶼿��԰�����
˵����������Ŀ���ڹ��ܺͲ���Ĺ淶Ҫ��Ӧ��Ҳ����һ��Ҫ���˰�
����ÿһ���������������и��û����Ķ�Ӧ����'-'��ͷ������ͷ����'-'�ľ��������� cmd �� for /f ��ȡ�����ġ�
�������ļ�����ֱ�ӱ��ϲ��� btvenlib.h ���С�

This header contains several functions to output according to the stdout output specification for BatVenti features and plugins.
When writing other files, it is recommended to include btvenlib.h rather than including this hyphen.h directly.
This file is already included by btvenlib.h, so do not include btvenlib.h within this file.
Hyphen.h contains functions to ensure output always starts with a '-' character, and can be included by other files as needed.
Actually, this is probably the only specification requirement for features and plugins in this project:
every program's output intended for users should start with '-', while outputs not starting with '-' are meant for cmd's for /f parsing.
This file may eventually be merged directly into btvenlib.h.

�����Ǹ��ļ������ĺ����ľ���˵����

������					�Ƿ��Ȼ���			�Ƿ������"- "	�Ƿ���ĩβ׷�ӻ��з�	�Ƽ��ô�
----------------+---------------------------+---------------+-------------------+---------------------------------
putsLFHy		|		�ǣ��Ȼ���			|	��			|		��			|	����;������ı�
putsHyApd		|	��ֱ��׷����stdoutĩβ|	��			|		��			|	������;�����Ҫ׷�ӵ��ı�
putsHyphen2		|	��ֱ��׷����stdoutĩβ|	��			|		��			|	�տ�ʼ����ʱ������������ı�
putsLFHyNoTl	|		�ǣ��Ȼ���			|	��			|		��			|	��δ��ȷ
putsHyApdNoTl	|	��ֱ��׷����stdoutĩβ|	��			|		��			|	��δ��ȷ
putsHyphen2NoTl	|	��ֱ��׷����stdoutĩβ|	��			|		��			|	��δ��ȷ

ǰ������������ puts() һ��������ĩβ����'\n'��������󲻸�'\n'�ģ�����ʹ�� printf() ���������������Ǵ�ε��ı����ǾͿ����ú����ֺ�����
NoTl ���� No Tail��û��׷�ӵĻ��з�Сβ�͡�

The following is a detailed description of the functions contained in this document:

Function name			New line first?				"- "at the beginning?	'\n' at the end?	Recommended usage
----------------+-----------------------------------+-------------------+-------------------+-----------------------------------------------------------------
putsLFHy		|			Yes						|		Yes			|		Yes			|	Put string while the func is running
putsHyApd		|	No, append at the end of stdout	|		No			|		Yes			|	Append string at the end of stdout while the func is running
putsHyphen2		|	No, append at the end of stdout	|		Yes			|		Yes			|	Put string while the function just started, and help text
putsLFHyNoTl	|			Yes						|		Yes			|		No			|	Not clear yet
putsHyApdNoTl	|	No, append at the end of stdout	|		No			|		No			|	Not clear yet
putsHyphen2NoTl	|	No, append at the end of stdout	|		Yes			|		No			|	Not clear yet

The first three functions are similar to puts(), adding ��\n�� at the end. If you do not want to add ��\n�� at the end, we recommend using printf() for output. However, if you are dealing with large amounts of text, consider using the latter three functions.
NoTl stands for No Tail, meaning there is no trailing newline character.

*/

#include <stdio.h>

int putsLFHy(const char *str) {
	/*
	����������ô�����
	���Ȳ�����ô
	�Ȼ���
	Ȼ����� "- "
	�ٿ�ʼ���ַ��������������ÿ�������з����ٲ�һ��"- "
	���ͺͱ�׼��puts()һ����������ô����ܵ�׷��һ�����з�
	�� putsHyApd() ��������������Ȼ�һ����˵��Ȼ���ٿ�ʼ����ַ���

	The purpose of this function is:
	No matter what, first output a newline,
	then output "- ",
	then start reading and outputting the string, adding a "- " after every newline encountered.
	Finally, just like the standard puts(), it always appends a newline at the end.
	The difference from putsHyApd() is that this function first outputs a newline before starting to output the string.
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

int putsHyApd(const char *str) {
	/*
	����������ô����Ƕ��ַ��������������ÿ�������з����ٲ�һ��"- "
	���ͺͱ�׼��puts()һ����������ô����ܵ�׷��һ�����з�
	�� putsLFHy() ������������Ȳ��Ȼ��У�ֱ��������ַ�����
	��������������Ļ�������ڵ��õĴ�������ַ����Ŀ�ͷ���Լ����� "- "
	�Է�����Ȼ���Ǵ����׿�ʼ����ġ���������п����� for /f ������һ�п�ͷ���� - ��Ȼ������ö����ı�

	This function reads and outputs a string, appending "- " after every newline encountered.
	Finally, just like the standard puts(), it always appends a newline at the end.
	Unlike putsLFHy(), this function does not begin with a newline��it starts directly by outputting the string.
	Therefore, when using this function, 
	it��s best to prepend "- " to the beginning of the string manually in the calling code.
	Otherwise, if the output starts right from the beginning of the line without "-", 
	`for /f` might see a line that doesn��t start with "-" and mistakenly parse content that it shouldn't.
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

int putsHyphen2(const char *str) {
	/*
	����������ô����Ƕ��ַ��������������ÿ�������з����ٲ�һ��"- "
	���ͺͱ�׼��puts()һ����������ô����ܵ�׷��һ�����з�
	�� putsLFHy() ������������Ȳ��Ȼ��У�ֱ�������"- "��Ȼ�����ַ�����
	�� putsHyApd() ������������Ȼ������һ��"- "��
	��Ϊ���ļ��ľɰ汾�Ѿ��ù��� putsHyphen() ������֣������ѷ������������ڼ�һ����2����ʾ���֡�

	This function reads and outputs a string, appending "- " after every newline encountered.
	Finally, just like the standard puts(), it always appends a newline at the end.
	Unlike putsLFHy(), this function does not start with a newline��it directly outputs "- " followed by the string.
	Unlike putsHyApd(), this one explicitly begins with a "- ".
	Since the name putsHyphen() was already used and deprecated in an earlier version of this file,
	a "2" is appended to the name to distinguish it.
	*/
	int lengthPut = 2;
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

int putsLFHyNoTl(const char *str) {
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
	return lengthPut;
}

int putsHyApdNoTl(const char *str) {
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
	return lengthPut;
}

int putsHyphen2NoTl(const char *str) {
	int lengthPut = 2;
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
	return lengthPut;
}
