#pragma once 
#include <sys/types.h>
#include "defs.h"
typedef struct{
	pid_t pid;
	char cmd[MAX_INPUT];
}Job;

extern Job jobs[MAX_JOBS];
extern int njobs;

void add_job(pid_t pid,const char *cmd);
void remove_job(pid_t pid);
void list_jobs(void);
