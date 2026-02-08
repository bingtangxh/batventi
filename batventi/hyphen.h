#pragma once
/*
这个头文件包含几个用来输出适配 batventi 功能与插件 stdout 输出规范的函数
编写别的文件的时候建议包含 btvenlib.h 这个头文件，别直接包含这个 hyphen.h
此文件已被 btvenlib.h 包含，请勿在此文件中包含 btvenlib.h
这个 hyphen.h 包含几个用来实现“输出总是以'-'开头”的函数，编写别的文件的时候都可以包含上
说起来我这项目对于功能和插件的规范要求应该也就这一个要求了吧
就是每一个程序的输出，其中给用户看的都应该以'-'开头，而开头不是'-'的就是用来让 cmd 的 for /f 读取分析的。
后续此文件可能直接被合并进 btvenlib.h 当中。

This header contains several functions to output according to the stdout output specification for BatVenti features and plugins.
When writing other files, it is recommended to include btvenlib.h rather than including this hyphen.h directly.
This file is already included by btvenlib.h, so do not include btvenlib.h within this file.
Hyphen.h contains functions to ensure output always starts with a '-' character, and can be included by other files as needed.
Actually, this is probably the only specification requirement for features and plugins in this project:
every program's output intended for users should start with '-', while outputs not starting with '-' are meant for cmd's for /f parsing.
This file may eventually be merged directly into btvenlib.h.

以下是该文件包含的函数的具体说明：

函数名					是否先换行			是否先输出"- "	是否在末尾追加换行符	推荐用处
----------------+---------------------------+---------------+-------------------+---------------------------------
putsLFHy		|		是，先换行			|	是			|		是			|	运行途中输出文本
putsHyApd		|	否，直接追加在stdout末尾|	否			|		是			|	运行中途输出需要追加的文本
putsHyphen2		|	否，直接追加在stdout末尾|	是			|		是			|	刚开始运行时的输出、帮助文本
putsLFHyNoTl	|		是，先换行			|	是			|		否			|	尚未明确
putsHyApdNoTl	|	否，直接追加在stdout末尾|	否			|		否			|	尚未明确
putsHyphen2NoTl	|	否，直接追加在stdout末尾|	是			|		否			|	尚未明确

前三个函数都和 puts() 一样，会在末尾跟上'\n'，如需最后不跟'\n'的，建议使用 printf() 进行输出。但如果是大段的文本，那就考虑用后三种函数。
NoTl 就是 No Tail，没有追加的换行符小尾巴。

The following is a detailed description of the functions contained in this document:

Function name			New line first?				"- "at the beginning?	'\n' at the end?	Recommended usage
----------------+-----------------------------------+-------------------+-------------------+-----------------------------------------------------------------
putsLFHy		|			Yes						|		Yes			|		Yes			|	Put string while the func is running
putsHyApd		|	No, append at the end of stdout	|		No			|		Yes			|	Append string at the end of stdout while the func is running
putsHyphen2		|	No, append at the end of stdout	|		Yes			|		Yes			|	Put string while the function just started, and help text
putsLFHyNoTl	|			Yes						|		Yes			|		No			|	Not clear yet
putsHyApdNoTl	|	No, append at the end of stdout	|		No			|		No			|	Not clear yet
putsHyphen2NoTl	|	No, append at the end of stdout	|		Yes			|		No			|	Not clear yet

The first three functions are similar to puts(), adding ‘\n’ at the end. If you do not want to add ‘\n’ at the end, we recommend using printf() for output. However, if you are dealing with large amounts of text, consider using the latter three functions.
NoTl stands for No Tail, meaning there is no trailing newline character.

*/

#include <stdio.h>

int putsLFHy(const char *str) {
	/*
	这个函数的用处就是
	首先不管怎么
	先换行
	然后输出 "- "
	再开始读字符串并进行输出，每读到换行符都再补一个"- "
	最后就和标准的puts()一样，不管怎么最后总得追加一个换行符
	和 putsHyApd() 的区别就是首先先换一行再说，然后再开始输出字符串

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
	这个函数的用处就是读字符串并进行输出，每读到换行符都再补一个"- "
	最后就和标准的puts()一样，不管怎么最后总得追加一个换行符
	和 putsLFHy() 的区别就是首先不先换行，直接先输出字符串、
	所以用这个函数的话，最好在调用的代码给的字符串的开头先自己加上 "- "
	以防“居然就是从行首开始输出的”情况，就有可能让 for /f 看到这一行开头不是 - ，然后读不该读的文本

	This function reads and outputs a string, appending "- " after every newline encountered.
	Finally, just like the standard puts(), it always appends a newline at the end.
	Unlike putsLFHy(), this function does not begin with a newline—it starts directly by outputting the string.
	Therefore, when using this function, 
	it’s best to prepend "- " to the beginning of the string manually in the calling code.
	Otherwise, if the output starts right from the beginning of the line without "-", 
	`for /f` might see a line that doesn’t start with "-" and mistakenly parse content that it shouldn't.
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
	这个函数的用处就是读字符串并进行输出，每读到换行符都再补一个"- "
	最后就和标准的puts()一样，不管怎么最后总得追加一个换行符
	和 putsLFHy() 的区别就是首先不先换行，直接先输出"- "，然后是字符串。
	和 putsHyApd() 的区别就是首先会先输出一个"- "。
	因为本文件的旧版本已经用过了 putsHyphen() 这个名字，并且已废弃，所以现在加一个“2”以示区分。

	This function reads and outputs a string, appending "- " after every newline encountered.
	Finally, just like the standard puts(), it always appends a newline at the end.
	Unlike putsLFHy(), this function does not start with a newline—it directly outputs "- " followed by the string.
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
