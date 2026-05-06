#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "history.h"
#include "jobs.h"
#include "defs.h"
#include "builtins.h"
/* Builtin commands ko child process ke andar nhi chalana cause they effect shell environment */
int is_builtin(char **args){
	if(strcmp(args[0],"cd")==0) return 1;
	if(strcmp(args[0],"pwd")==0) return 1;
	if(strcmp(args[0],"export")==0) return 1;
	if(strcmp(args[0],"echo")==0) return 1;
	if(strcmp(args[0],"fg")==0) return 1;
	if(strcmp(args[0],"jobs")==0) return 1;
	if(strcmp(args[0],"history")==0) return 1;
	return 0;
}
void builtin_history(char **args){
	if(args[1]!=NULL){
		int n=atoi(args[1]);
		print_history(n);
	}else{
		print_history(-1);
	}
}
void builtin_fg(char **args){
	if(args[1]==NULL){
		fprintf(stderr,"fg:missing argument. Use fg #PID\n");
		return;
	}
	char *pid_str=args[1];
	if(pid_str[0]=='#') pid_str++;
	pid_t pid=atoi(pid_str);
	if(kill(pid,0)!=0){
		fprintf(stderr,"fg: no such process %d\n",pid);
		return;
	}
	for(int i=0;i<njobs;i++){
		if(jobs[i].pid==pid){
			printf("%s\n",jobs[i].cmd);
			break;
		}
	}
	int status;
	waitpid(pid,&status,0);
	remove_job(pid);
}
void builtin_cd(char **args){
	const char* path;
	if(args[1]==NULL){
		path=getenv("HOME");
	}else{
		path=args[1];
	}
	if(chdir(path)!=0){
		perror(path);
		return;
	}
	char cwd[MAX_INPUT];
	if(getcwd(cwd,sizeof(cwd))!=NULL){
		setenv("PWD",cwd,1); //systen variable main cwd ko store krta hai by overwriting
	}
}
void builtin_pwd(void){
	char cwd[MAX_INPUT];
	if(getcwd(cwd,sizeof(cwd))!=NULL){
		printf("%s\n",cwd);
	}else{
		perror("getcwd");
	}
}
/* export VAR=value */
void builtin_export(char **args){
	if(args[1]==NULL){
		fprintf(stderr,"export: missing argument\n");
		return;
	}
	char *eq=strchr(args[1],'=');
	if(eq==NULL){
		fprintf(stderr,"export: use format VAR=value\n");
		return;
	}
	*eq='\0';
	char *key=args[1];
	char *value=eq+1;
	setenv(key,value,1);
}
void builtin_echo(char **args){
	for(int i=1;args[i]!=NULL;i++){
		if(args[i][0]=='$'){
			char *val=getenv(args[i]+1);
			printf("%s",val?val:"");	
		}else{
			printf("%s",args[i]);
		}
		if(args[i+1]!=NULL) printf(" ");
	}
	printf("\n");
}
void handle_builtin(char **args){
	if(strcmp(args[0],"cd")==0) builtin_cd(args);
	else if(strcmp(args[0],"pwd")==0) builtin_pwd();
	else if(strcmp(args[0],"export")==0) builtin_export(args);
	else if(strcmp(args[0],"echo")==0) builtin_echo(args);
	else if(strcmp(args[0],"fg")==0) builtin_fg(args);
	else if(strcmp(args[0],"jobs")==0) list_jobs();
	else if(strcmp(args[0],"history")==0) builtin_history(args);
}
