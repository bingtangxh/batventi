#pragma once
#include <stdio.h>
#include <wincon.h>
#include "btvenlib.h"
#include <math.h>
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

void coloredEcho(int argc, const char **argv);
_Bool isANSIColorSupported();
void coloredTest(void);
void coloredEchoHelp(void);
int findClosestConsoleColor(int r, int g, int b);
int grayLevel(int r, int g, int b);
INT8 attr2Num(char attr);

void coloredEcho(int argc, const char **argv) {
	int currentIndex = -1, elemsGotten = 0;
	static _Bool useANSI = FALSE, isFeasibilityGotten = FALSE;
	_Bool isendlReqed = FALSE, gray = FALSE, isSCTAReqed = FALSE, underline = FALSE;
	
	//  这是输出流的句柄
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (!isFeasibilityGotten) {
		useANSI = isANSIColorSupported();
		isFeasibilityGotten = TRUE;
	}
	
	if (argc < 2) {
		putsHyphen("Error from func coloredEcho in header file coloredEcho.h: Why argc < 2?");
		return;
	}
	if (argc == 2 || (argc == 3 && !_stricmp(argv[2], "help"))) {
		coloredEchoHelp();
		return;
	}
	if (!_stricmp(argv[2], "endl")) {
		isendlReqed = TRUE;
		currentIndex = 3;
	}
	else if (!_stricmp(argv[2], "noendl")) {
		isendlReqed = FALSE;
		currentIndex = 3;
	}
	else {
		isendlReqed = TRUE;
		currentIndex = 2;
	}

	if (!_stricmp(argv[currentIndex], "ANSI")) {
		currentIndex++;
	}
	else if (!_stricmp(argv[currentIndex], "ANSI_gray")) {
		currentIndex++;
		gray = TRUE;
	}
	else if (!_stricmp(argv[currentIndex], "SCTA")) {
		currentIndex++;
		isSCTAReqed = TRUE;
	}
	else {
		printf("- Error from func coloredEcho in header file colorEcho.h: Unacceptable argv[%d]: %s\n",currentIndex,argv[currentIndex]);
		return;
	}

	int Rf, Gf, Bf, Rb, Gb, Bb;
	Rf = Gf = Bf = Rb = Gb = Bb = 0;
	int foreg = 0, backg = 0;

	if (!isSCTAReqed) {
		// ANSI 参数情况
		elemsGotten = sscanf(argv[currentIndex], "%i,%i,%i", &Rf, &Gf, &Bf);
		if (elemsGotten != 3) {
			putsHyphen("Error from func coloredEcho in header file coloredEcho.h: Unable to scan R,G,B_Fg");
			return;
		}
		currentIndex++;
		if (!_stricmp(argv[currentIndex], "on")) {
			underline = FALSE;
		}
		else if (!_stricmp(argv[currentIndex], "underline")) {
			underline = TRUE;
		}
		else {
			printf("- Error from func coloredEcho in header file colorEcho.h: Unacceptable argv[%d]: %s\n", currentIndex, argv[currentIndex]);
			return;
		}
		currentIndex++;
		elemsGotten = sscanf(argv[currentIndex], "%i,%i,%i", &Rb, &Gb, &Bb);
		if (elemsGotten != 3) {
			putsHyphen("Error from func coloredEcho in header file coloredEcho.h: Unable to scan R,G,B_Bg");
			return;
		}
		currentIndex++;
	}
	else {
		// SCTA 参数情况
		if (strlen(argv[currentIndex]) < 2) {
			printf("- Error from func coloredEcho in header file colorEcho.h: argv[%d] is too short: %s\n", currentIndex, argv[currentIndex]);
			return;
		}
		backg = attr2Num(argv[currentIndex][0]);
		foreg = attr2Num(argv[currentIndex][1]);
		if (foreg == -1 || backg == -1) {
			printf("- Error from func coloredEcho in header file colorEcho.h: argv[%d] is unacceptable: %s\n", currentIndex, argv[currentIndex]);
			return;
		}
		if (foreg == backg) {
			printf("- Error from func coloredEcho in header file colorEcho.h: argv[%d] contains 2 same digits: %s\n", currentIndex, argv[currentIndex]);
			return;
		}
		if (strlen(argv[currentIndex]) == 3) {
			if ((argv[currentIndex][2] == 'u') || (argv[currentIndex][2] == 'U')) {
				underline = TRUE;
			}
			else underline = FALSE;
		}
		else underline = FALSE;
		currentIndex++;
	}
	// 读入完了，开始处理
	if (useANSI && !isSCTAReqed) {
		// ANSI 被指定并且能用的情况
		char *underline_str = underline ? "4;" : "";
		printf("\x1b[%s38;2;%d;%d;%d;48;2;%d;%d;%dm",underline_str,Rf,Gf,Bf,Rb,Gb,Bb);
		for (; currentIndex < argc; currentIndex++) {
			fputs(argv[currentIndex], stdout);
			if (currentIndex < argc - 1) putchar(' ');
		}
		printf("\x1b[0m");
		if (isendlReqed) {
			putchar('\n');
		}
		return;
	}
	else {
		// 这里是 SCTA 方式的代码

		if (isSCTAReqed) {
			// 参数明确指定为 SCTA 模式
			// 啥都不干
		}
		else {
			// 参数指定了 ANSI 但是用不了，于是来到这里
			// 这里是回退为 SCTA 方式的代码
			if (gray) {
				foreg = grayLevel(Rf, Gf, Bf);
				backg = grayLevel(Rb, Gb, Bb);
			}
			else {
				foreg = findClosestConsoleColor(Rf, Gf, Bf);
				backg = findClosestConsoleColor(Rb, Gb, Bb);
			}
		}
		// 先获取原来的控制台属性，因为输出完了要改回去
		CONSOLE_SCREEN_BUFFER_INFO current_con;
		elemsGotten = GetConsoleScreenBufferInfo(hConsole, &current_con);
		if (elemsGotten == FALSE) {
			putsHyphen("Error from func coloredEcho in header file coloredEcho.h: GetConsoleScreenBufferInfo returned FALSE");
			return;
		}
		WORD attr = foreg | (backg << 4);
		WORD current_attr = current_con.wAttributes;

		if (underline) {
			attr |= COMMON_LVB_UNDERSCORE;
		}
		// 设置属性
		SetConsoleTextAttribute(hConsole, attr);
		// 开始输出
		for (; currentIndex < argc; currentIndex++) {
			fputs(argv[currentIndex], stdout);
			if (currentIndex < argc - 1) putchar(' ');
		}
		// 改回原来
		SetConsoleTextAttribute(hConsole, current_attr);
		// 要不要换行
		if (isendlReqed) {
			putchar('\n');
		}
		// printf("%d", isendlReqed);
		return;
	}

}

_Bool isANSIColorSupported() {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	if (hOut == INVALID_HANDLE_VALUE) return FALSE;
	if (!GetConsoleMode(hOut, &dwMode)) return FALSE;
	// 如果支持 VT 模式，那就试试开不开得开
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	if (!SetConsoleMode(hOut, dwMode)) return FALSE;
	return TRUE;
}

void coloredTest(void)
{
	int colorIndex = 0;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	static _Bool useANSI = FALSE;
	static _Bool isFeasibilityGotten = FALSE;

	if (!isFeasibilityGotten) {
		useANSI = isANSIColorSupported();
		isFeasibilityGotten = TRUE;
	}
	do {
		if (useANSI) {
			// 使用 ANSI 转义序列
			printf("\x1B[%dm%s\x1B[0m\n", 30 + (colorIndex % 8), "This is test str 1");
		}
		else {
			// 使用 SetConsoleTextAttribute
			CONSOLE_SCREEN_BUFFER_INFO csbi;
			WORD original;
			GetConsoleScreenBufferInfo(hConsole, &csbi);
			original = csbi.wAttributes;

			SetConsoleTextAttribute(hConsole, (colorIndex % 8) | 0);
			printf("%s\t%d\t", "This is test str 2", colorIndex % 8);

			SetConsoleTextAttribute(hConsole, (colorIndex % 8) | FOREGROUND_INTENSITY);
			printf("%s\t%d\n", "This is test str 3", (colorIndex % 8) | FOREGROUND_INTENSITY);

			SetConsoleTextAttribute(hConsole, original); // 恢复原色
		}
		colorIndex++;
	} while (_getch() != '\\');

	printf("普通颜色看看\n");

	SetConsoleTextAttribute(hConsole,
		FOREGROUND_GREEN |      // 前景色_绿色
		FOREGROUND_INTENSITY); // 前景色_加强
	printf("设置了浅绿色.\n");
	printf("并且完了之后就一直是浅绿色\n");

	SetConsoleTextAttribute(hConsole,
		FOREGROUND_BLUE |       // 前景色_蓝色
		FOREGROUND_INTENSITY |  // 前景色_加强
		COMMON_LVB_UNDERSCORE); // 添加下划线
	printf("文字蓝色，再加个下划线.\n");

	SetConsoleTextAttribute(hConsole,
		FOREGROUND_RED |        // 前景色_红色
		FOREGROUND_INTENSITY |  // 前景色_加强
		BACKGROUND_BLUE);      // 背景色_蓝色
	printf("设置文字红色，背景蓝色\n");

	SetConsoleTextAttribute(hConsole,
		FOREGROUND_RED |            // 前景色_红色
		FOREGROUND_INTENSITY |      // 前景色_加强
		COMMON_LVB_GRID_LVERTICAL);// 网格_左_竖
	printf("                      加 左 网格\n");

	SetConsoleTextAttribute(hConsole,
		FOREGROUND_RED |            // 前景色_红色
		FOREGROUND_INTENSITY |      // 前景色_加强
		COMMON_LVB_GRID_RVERTICAL);// 网格_右_竖  
	printf("                      加 右 网格\n");

	SetConsoleTextAttribute(hConsole,
		FOREGROUND_RED |    // 前景色_红色
		FOREGROUND_GREEN |  // 前景色_绿色
		FOREGROUND_BLUE);  // 前景色_蓝色
	printf("改回白色\n");

}

void coloredEchoHelp(void) {
	const char helpText_old[] =
		"- A easy method to display a string with applied color without influencing the whole screen.\n"
		"\n"
		"Useage:\n"
		"batventi coloredecho [ endl | Noendl ] [ ANSI | ANSI_Gray ] R,G,B_Fg [ on | underline ] R,G,B_Bg text1 text2 text3 ...\n"
		"\n"
		"or, force defined using SetConsoleTextAttribute function on older Console Host instead of ANSI:\n"
		"\n"
		"batventi coloredecho [ endl | Noendl ] SCTA attr text1 text2 text3 ...\n"
		"\n"
		"[ endl | Noendl]     Could be omitted. Tell me about if I need to append a \\n symbol at the end.\n"
		"[ ANSI | SCTA ]      Use ANSI escape sequences or SetConsoleTextAttribute function.\n"
		"                     If the current console host does not support ANSI escape sequences,\n"
		"                     then it will fallback into using SetConsoleTextAttribute,\n"
		"                     and 16 colors will be used for fitting.\n"
		"                     ANSI_Gray is still not recommended to use.\n"
		"R,G,B_Fg             The color number of foreground color in RGB type, using comma as delimiter.\n"
		"                     For example: 0,255,255 or 0x0,0xFF,0xFF.\n"
		"attr                 Specifies color attribute of console output, composed of 2 hex digits,\n"
		"                     the first corresponds to the background; the second the foreground.\n"
		"                     Each digit can be any of the following values:\n"
		"                     1 means blue, 2 means green, 4 means red, 8 means light.\n"
		"                     0 = Black       8 = Gray\n"
		"                     1 = Blue        9 = Light Blue\n"
		"                     2 = Green       A = Light Green\n"
		"                     3 = Aqua        B = Light Aqua\n"
		"                     4 = Red         C = Light Red\n"
		"                     5 = Purple      D = Light Purple\n"
		"                     6 = Yellow      E = Light Yellow\n"
		"                     7 = White       F = Bright White\n"
		"                     You can also add a \'u\' letter at the end of attr, such as \"0fu\", make it be underlined.\n"
		"R,G,B_Bg             Same to R,G,B_Fg, but this applies to the background, rather than the foreground.\n"
		"[ on | underline ]   on means normal, while underline means it will be underlined.\n"
		"text1 text2 text3 ...means the sentence you want to display, you can contain spaces without quotation marks used.\n"
		"                     All texts you given in one command will be displayed,\n"
		"                     and whether the ending is a line break depends on [ endl | Noendl ] you given.\n"
		"\n"
		"The order of the parameters must be the same as stated in the help information.\n"
		"Mind that the order of background and foreground colors, which are reversed in ANSI and SCTA mode.\n"
		"\n"
		"For example:\n"
		"batventi coloredecho ANSI 0,255,255 underline 128,0,0 The sentence will be cyan/aqua on dark red.\n"
		"batventi coloredecho SCTA 5a The sentence will be light green on dark purple.\n"
		"";
	const char helpText[] =
		"Easily print colored text without affecting the whole screen.\n"
		"\n"
		"Usage:\n"
		"  batventi coloredecho [endl | noendl] [ANSI | ANSI_Gray] R,G,B_Fg [on | underline] R,G,B_Bg text1 text2 ...\n"
		"  batventi coloredecho [endl | noendl] SCTA attr text1 text2 ...\n"
		"\n"
		"Arguments:\n"
		"  [endl | noendl]         Optional. Controls whether to append a line break (\\n) after the text.\n"
		"\n"
		"  [ANSI | ANSI_Gray]      Use ANSI escape sequences for color. If ANSI is not supported on this console,\n"
		"                          the output will automatically fall back to SetConsoleTextAttribute (SCTA) mode.\n"
		"                          'ANSI_Gray' restricts colors to gray tones and is not recommended.\n"
		"\n"
		"  R,G,B_Fg                Foreground color in RGB format, using commas as separators.\n"
		"                          Examples: 0,255,255 or 0x00,0xFF,0xFF\n"
		"\n"
		"  [on | underline]        Text style. 'on' = normal; 'underline' = underlined text.\n"
		"\n"
		"  R,G,B_Bg                Background color in RGB format, same syntax as R,G,B_Fg.\n"
		"\n"
		"  SCTA attr               Use SetConsoleTextAttribute with a 2-digit hex code.\n"
		"                          First digit: background; second digit: foreground.\n"
		"                          You may append 'u' to enable underlining (e.g., 0fu).\n"
		"                          Color digit values:\n"
		"                            0 = Black        8 = Gray\n"
		"                            1 = Blue         9 = Light Blue\n"
		"                            2 = Green        A = Light Green\n"
		"                            3 = Aqua         B = Light Aqua\n"
		"                            4 = Red          C = Light Red\n"
		"                            5 = Purple       D = Light Purple\n"
		"                            6 = Yellow       E = Light Yellow\n"
		"                            7 = White        F = Bright White\n"
		"\n"
		"  text1 text2 ...         Text to display. Spaces between words are preserved.\n"
		"\n"
		"Notes:\n"
		"  - The parameter order must exactly match the format described above.\n"
		"  - In ANSI mode, colors are specified as: foreground first, then background.\n"
		"    In SCTA mode, it's background first, then foreground.\n"
		"\n"
		"Examples:\n"
		"  batventi coloredecho ANSI 0,255,255 underline 128,0,0 Hello World!\n"
		"    → Displays cyan/aqua text on dark red with underline.\n"
		"\n"
		"  batventi coloredecho SCTA 5a This is light green on dark purple.\n";

	putsHyphenApd(helpText);
}

typedef struct {
	int id;
	int r, g, b;
} ConsoleColor;

static const ConsoleColor consoleColors[16] = {
	{ 0, 0, 0, 0 },{ 1, 0, 0, 128 },
	{ 2, 0, 128, 0 },{ 3, 0, 128, 128 },
	{ 4, 128, 0, 0 },{ 5, 128, 0, 128 },
	{ 6, 128, 128, 0 },{ 7, 192, 192, 192 },
	{ 8, 128, 128, 128 },{ 9, 0, 0, 255 },
	{ 10, 0, 255, 0 },{ 11, 0, 255, 255 },
	{ 12, 255, 0, 0 },{ 13, 255, 0, 255 },
	{ 14, 255, 255, 0 },{ 15, 255, 255, 255 }
},
grayColors[4] = {
	{ 0, 0, 0, 0 },
	{ 7, 192, 192, 192 },
	{ 8, 128, 128, 128 },
	{ 15, 255, 255, 255 }
};

int findClosestConsoleColor(int r, int g, int b) {
	int closestId = 0;
	double minDistance = 1e9;
	for (int i = 0; i < 16; ++i) {
		double dr = r - consoleColors[i].r;
		double dg = g - consoleColors[i].g;
		double db = b - consoleColors[i].b;
		double distance = dr * dr + dg * dg + db * db; // 欧氏距离平方
		if (distance < minDistance) {
			minDistance = distance;
			closestId = consoleColors[i].id;
		}
	}
	return closestId;
}

int grayLevel(int r, int g, int b) {
	int l = (r + g + b) / 3;  // 也可用更精确的感知亮度算法
	if (l < 64) return 0;
	else if (l < 160) return 8;
	else if (l < 224) return 7;
	else return 15;
}

INT8 attr2Num(char attr) {
	switch (attr)
	{
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		return attr - '0';
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
		return attr - 'a' + 10;
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
		return attr - 'A' + 10;
	default:
		return -1;
	}
}
