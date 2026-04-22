#include "defs.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "executor.h"
void execute_command(char **args){
	pid_t pid=fork();
	if(pid<0){
		perror("fork");
		return;
	}
	if(pid==0){
		/* Child Process replace krdeta hai apne apko execvp ke sath */
		execvp(args[0],args);
		/* If execvp returns means ke koi command hi nhi hai aisi */
		fprintf(stderr,"My_Shell : %s : No Command Found\n",args[0]);
		exit(1);
	}
	int status;
	waitpid(pid,&status,0);
}
/* fork se yeh faida huwa ke sirf yeh command fail hogi baki shell work krta rahega */
