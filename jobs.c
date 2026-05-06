#include "defs.h"
#define _POSIX_C_SOURCE 2000809L
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include "jobs.h"
Job jobs[MAX_JOBS];
int njobs=0;
void add_job(pid_t pid,const char *cmd){
	if(njobs>=MAX_JOBS){
		fprintf(stderr,"job list full\n");
		return;
	}
	jobs[njobs].pid=pid;
	strncpy(jobs[njobs].cmd,cmd,MAX_INPUT-1);
	njobs++;
}
void remove_job(pid_t pid){
	for(int i=0;i<njobs;i++){
		if(jobs[i].pid==pid){
			for(int j=i;j<njobs-1;j++){
				jobs[j]=jobs[j+1];
			}
			njobs--;
			return;
		}
	}
}
void list_jobs(void){
	for(int i=0;i<njobs;i++){
		printf("[%d] %d %s\n",i+1,jobs[i].pid,jobs[i].cmd);
	}
}
