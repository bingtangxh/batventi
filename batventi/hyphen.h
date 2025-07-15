#pragma once

// 这个头文件包含几个用来输出适配 batventi 功能与插件 stdout 输出规范的函数
// 编写别的文件的时候建议包含 btvenlib.h 这个头文件，别直接包含这个 hyphen.h
// 此文件已被 btvenlib.h 包含，请勿在此文件中包含 btvenlib.h
// 这个 hyphen.h 包含几个用来实现“输出总是以'-'开头”的函数，编写别的文件的时候都可以包含上
// 说起来我这项目对于功能和插件的规范要求应该也就这一个要求了吧
// 就是每一个程序的输出，其中给用户看的都应该以'-'开头，而开头不是'-'的就是用来让 cmd 的 for /f 读取分析的。

// 后续此文件可能直接被合并进 btvenlib.h 当中


#include <stdio.h>

int putsHyphen(const char *str) {
	/*
	这个函数的用处就是首先不管怎么先换行
	然后输出 "- "
	再开始读字符串并进行输出，每读到换行符都再补一个"- "
	最后就和标准的puts()一样，不管怎么最后总得追加一个换行符
	和 putsHyphenApd() 的区别就是首先先换一行再说，然后再开始输出字符串
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
	这个函数的用处就是读字符串并进行输出，每读到换行符都再补一个"- "
	最后就和标准的puts()一样，不管怎么最后总得追加一个换行符
	和 putsHyphen() 的区别就是首先不先换行，直接先输出字符串、
	所以用这个函数的话，最好在调用的代码给的字符串的开头先自己加上 "- "
	以防“居然就是从行首开始输出的”情况，就有可能让 for /f 看到这一行开头不是 - ，然后读不该读的文本
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