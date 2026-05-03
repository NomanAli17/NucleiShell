#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "executor.h"
#include "defs.h"
#include "parser.h"
static void apply_redirection(Command *cmd){
	if(cmd->in_file){
		int fd=open(cmd->in_file,O_RDONLY);
		if(fd<0){
			perror(cmd->in_file);
			exit(1);
		}
		dup2(fd,STDIN_FILENO);
		close(fd);
	}
	if(cmd->out_file){
		int flags=O_WRONLY | O_CREAT | (cmd->append ? O_APPEND : O_TRUNC);
		int fd=open(cmd->out_file,flags,0644);
		if(fd<0){
			perror(cmd->out_file);
			exit(1);
		}
		dup2(fd,STDOUT_FILENO);
		close(fd);
	}
}
void execute_command(Command *cmd){
	pid_t pid=fork();
	if(pid<0){
		perror("fork");
		return;
	}
	if(pid==0){
		/* Child Process replace krdeta hai apne apko execvp ke sath */
		apply_redirection(cmd);
		execvp(cmd->args[0],cmd->args);
		fprintf(stderr,"My_Shell : %s: No command found\n",cmd->args[0]);
		exit(1);
	}
	int status;
	waitpid(pid,&status,0);
}
/* fork se yeh faida huwa ke sirf yeh command fail hogi baki shell work krta rahega */
void execute_pipeline(Pipeline *pipeline){
	int n=pipeline->num_cmds;
	if(n==1){
		execute_command(&pipeline->cmds[0]);
		return;
	}
	int pipes[n-1][2];
	for(int i=0;i<n-1;i++){
		if(pipe(pipes[i])<0){
			perror("pipe");
			return;
		}
	}
	pid_t pids[n];
	for(int i=0;i<n;i++){
		pids[i]=fork();
		if(pids[i]<0){
			perror("fork");
			return;
		}
		if(pids[i]==0){
			if(i>0){
				dup2(pipes[i-1][0],STDIN_FILENO);
			}
			if(i<n-1){
				dup2(pipes[i][1],STDOUT_FILENO);
			}
			//pipe only receive EOF when every other process has closed file descriptor of that pipe
			for(int j=0;j<n-1;j++){
				close(pipes[j][0]);
				close(pipes[j][1]);
			}
			apply_redirection(&pipeline->cmds[i]);
			execvp(pipeline->cmds[i].args[0],pipeline->cmds[i].args);
			fprintf(stderr,"My_Shell : %s: No command found\n",pipeline->cmds[i].args[0]);
		exit(1);
		}
	}
	for(int j=0;j<n-1;j++){
		close(pipes[j][0]);
		close(pipes[j][1]);
	}
	for(int i=0;i<n;i++){
		waitpid(pids[i],NULL,0);
	}		
}
