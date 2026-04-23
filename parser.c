#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "parser.h"
int parse_input(char *input,Command *cmd){
	cmd->argc=0;
	cmd->in_file=NULL;
	cmd->out_file=NULL;
	cmd->append=0;
	char *token=strtok(input," \t");
	while(token!=NULL && cmd->argc<MAX_ARGS-1){
		if(strcmp(token,"<")==0){
			// next lafz input file hogi
			token=strtok(NULL," \t"); 
			if(token) cmd->in_file=token;
		}else if(strcmp(token,">>")==0){
			// next lafz output append file hogi
			token=strtok(NULL," \t"); 
			if(token) {
				cmd->out_file=token;
				cmd->append=1;
			}
				
		}else if(strcmp(token,">")==0){
			// next lafz output file hogi
			token=strtok(NULL," \t"); 
			if(token) {
				cmd->out_file=token;
				cmd->append=0;
			}
		}else{
			// Regular argument
			cmd->args[cmd->argc++]=token;
		}
		token=strtok(NULL," \t");
	}
	cmd->args[cmd->argc]=NULL; //execvp ko null terminator chahiye hota hai in the end of arguments
	return cmd->argc;
}
