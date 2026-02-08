#pragma once

#include "btvenlib.h"
#include <Windows.h>

int input_num(int argc, char **argv);
int input_letter(int argc, char **argv);
int input_boolean(int argc, char **argv);
int input_word(int argc, char **argv);
int input_file(int argc, char **argv);
int input_folder(int argc, char **argv);
void inputHelp(void);

int input(int argc, char **argv) {
	if (argc < 1) {
		putsLFHy("Error from func input in header file input.h: Why argc < 1 ? I could not handle this.");
		return BAD_ARGC;
	}
	putsLFHy("This feature is not finished yet.\nHow about we explore the area ahead of us later?");
	return 0;
	if (argc == 1 || (argc == 2 && !_stricmp(argv[1], "help"))) {
		inputHelp();
		return 0;
	}
	if (!_stricmp(argv[1], "num") || (!_stricmp(argv[1], "num"))) {
		return input_num(argc, argv);
	}
	if (!_stricmp(argv[1], "letter") || (!_stricmp(argv[1], "letter"))) {
		return input_letter(argc, argv);
	}
	if (!_stricmp(argv[1], "bool") || (!_stricmp(argv[1], "bool"))) {
		return input_boolean(argc, argv);
	}
	if (!_stricmp(argv[1], "word") || (!_stricmp(argv[1], "word"))) {
		return input_word(argc, argv);
	}
	if (!_stricmp(argv[1], "file") || (!_stricmp(argv[1], "file"))) {
		return input_file(argc, argv);
	}
	if (!_stricmp(argv[1], "folder") || (!_stricmp(argv[1], "folder"))) {
		return input_folder(argc, argv);
	}
	putsLFHy("Error from func input in header file input.h: Unknown argv[1]");
	return NOT_FOUND;
}

int input_num(int argc, char ** argv)
{
	return 0;
}

int input_letter(int argc, char **argv) {
	return 0;
}
int input_boolean(int argc, char **argv) {
	return 0;
}
int input_word(int argc, char **argv) {
	return 0;
}
int input_file(int argc, char **argv) {
	return 0;
}
int input_folder(int argc, char **argv) {
	return 0;
}

void inputHelp(void) {
	const char helpText[] =
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"\n"
		"\n";
	putsHyphen2(helpText);
}