#pragma once 
#include "defs.h"
extern char history_buf[MAX_HIST][MAX_INPUT];
extern int hist_count;
void add_history(const char *cmd);
const char *recall_history(const char *token);
void print_history(int last_n);
