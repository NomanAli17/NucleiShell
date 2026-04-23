#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "executor.h"
#include "defs.h"
#include "parser.h"
void execute_command(Command *cmd){
	pid_t pid=fork();
	if(pid<0){
		perror("fork");
		return;
	}
	if(pid==0){
		/* Child Process replace krdeta hai apne apko execvp ke sath */
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
		execvp(cmd->args[0],cmd->args);
		fprintf(stderr,"My_Shell : %s: No command found\n",cmd->args[0]);
		exit(1);
	}
	int status;
	waitpid(pid,&status,0);
}
/* fork se yeh faida huwa ke sirf yeh command fail hogi baki shell work krta rahega */
