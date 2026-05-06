#include "defs.h"
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "history.h"
char history_buf[MAX_HIST][MAX_INPUT];
int hist_count=0;
void add_history(const char *cmd){
	if(strncmp(cmd,"history",7)==0) return;
	if(cmd[0]=='!') return;
	int idx=hist_count%MAX_HIST;
	strncpy(history_buf[idx],cmd,MAX_INPUT-1);
	history_buf[idx][MAX_INPUT-1]='\0';
	hist_count++;
} 
//-1 to print all
void print_history(int last_n){
	int start=0;
	if(last_n>0 && last_n<hist_count){
		start=hist_count-last_n;
	}
	for(int i=start;i<hist_count;i++){
		int idx=i%MAX_HIST;
		printf("%4d %s\n",i+1,history_buf[idx]);
	}
}
//! to recall command
const char *recall_history(const char *token){
	if(hist_count==0){
		fprintf(stderr,"No Commands in history\n");
		return NULL;
	}
	// !! - last command
	if(strcmp(token,"!!")==0){
		return history_buf[(hist_count-1)%MAX_HIST];
	}
	// !-N -N commands  
	if(token[1]=='-'){
		int n=atoi(token+2);
		if(n<=0 || n>hist_count){
			fprintf(stderr,"No such history entry\n");
			return NULL;
		}
		return history_buf[(hist_count-n)%MAX_HIST];
	}
	// !N -entry number N
	if(isdigit(token[1])){
		int n=atoi(token+1);
		if(n<=0 || n>hist_count){
			fprintf(stderr,"No such history entry\n");
			return NULL;
		}
		return history_buf[(n-1)%MAX_HIST];
	}
	const char *prefix=token+1;
	for(int i=hist_count-1;i>=0;i--){
		if(strncmp(history_buf[i%MAX_HIST],prefix,strlen(prefix))==0){
			return history_buf[i%MAX_HIST];
		}
	}
	fprintf(stderr,"No match found in history\n");
	return NULL;
}
