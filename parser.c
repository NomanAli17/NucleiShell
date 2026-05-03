#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "parser.h"
//so what first function does it ke yeh aik single command ke liye argument aur uska sara data store krta hai
static void parse_command(char **tokens,int count,Command *cmd){
	cmd->argc=0;
	cmd->in_file=NULL;
	cmd->out_file=NULL;
	cmd->append=0;
	for(int i=0;i<count;i++){
		if(strcmp(tokens[i],"<")==0){
			if(i+1<count) cmd->in_file=tokens[++i];
		}else if(strcmp(tokens[i],">>")==0){
			if(i+1<count) cmd->out_file=tokens[++i];
			cmd->append=1;
		}else if(strcmp(tokens[i],">")==0){
			if(i+1<count) cmd->out_file=tokens[++i];
			cmd->append=0;
		}else{
			cmd->args[cmd->argc++]=tokens[i];
		}
	}
	cmd->args[cmd->argc]=NULL;
}
//Now yahan pe hum | dekhte hi command split krdete cause pipe agaya aur uske piche wale sare us command ke argument honge
int parse_input(char *input,Pipeline *pipeline){
	pipeline->num_cmds=0;
	char *all_tokens[MAX_ARGS];
	int total=0;
	char *token=strtok(input," \t");
	while(token && total<MAX_ARGS-1){
		all_tokens[total++]=token;
		token=strtok(NULL," \t");	
	}
	if(total==0) return 0;
	int start=0;
	for(int i=0;i<=total;i++){
		if(i==total || strcmp(all_tokens[i],"|")==0){
			if(i>start){
				parse_command(all_tokens+start,i-start,&pipeline->cmds[pipeline->num_cmds++]);
			}
			start=i+1;
		}
	}
	return pipeline->num_cmds;
}
