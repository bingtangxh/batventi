#pragma once
#include "btvenlib.h"

const char verNum[] = "0.5.1-preAlpha";

void version()
{
	printf("- BatVenti Version %s\n", verNum);
	printf("\n");
	printf("- Written by BingtangXH <goat@bingtangxh.moe>\n");
	printf("\n");
	printf("- Copyright (C) 2025 BingtangXH.\n");
	printf("- \n");
	printf("- This is free software; see the source for copying conditions.\n");
	printf("- \n");
	printf("- Current ANSI CodePage of the system: %u\n",GetACP());
}