#pragma once
#include "defs.h"
typedef struct{
	char *args[MAX_INPUT];
	int argc;
	char *in_file; //input file for <
	char *out_file; //outpute file for > 
	int append; // 1 for >> append, 0 for > overwrite
} Command;
typedef struct{
	Command cmds[MAX_ARGS];
	int num_cmds;
	int background;
	char raw_input[MAX_INPUT];
} Pipeline;
int parse_input(char *input,Pipeline *pipeline);
