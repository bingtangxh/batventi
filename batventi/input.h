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
	if (argc < 2) {
		putsLFHy("Error from func input in header file input.h: Why argc < 2 ? I could not handle this.");
		return BAD_ARGC;
	}
	putsLFHy("This feature is not finished yet.\nHow about we explore the area ahead of us later?");
	return 0;
	if (argc == 2 || (argc == 3 && !_stricmp(argv[2], "help"))) {
		inputHelp();
		return 0;
	}
	if (!_stricmp(argv[2], "num") || (!_stricmp(argv[2], "num"))) {
		return input_num(argc, argv);
	}
	if (!_stricmp(argv[2], "letter") || (!_stricmp(argv[2], "letter"))) {
		return input_letter(argc, argv);
	}
	if (!_stricmp(argv[2], "bool") || (!_stricmp(argv[2], "bool"))) {
		return input_boolean(argc, argv);
	}
	if (!_stricmp(argv[2], "word") || (!_stricmp(argv[2], "word"))) {
		return input_word(argc, argv);
	}
	if (!_stricmp(argv[2], "file") || (!_stricmp(argv[2], "file"))) {
		return input_file(argc, argv);
	}
	if (!_stricmp(argv[2], "folder") || (!_stricmp(argv[2], "folder"))) {
		return input_folder(argc, argv);
	}
	putsLFHy("Error from func input in header file input.h: Unknown argv[2]");
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