#include "defs.h"
#include <string.h>
#include "parser.h"
int parse_input(char *input,char **args){
	int argc=0;
	char *token=strtok(input," \t");
	while(token!=NULL && argc<MAX_ARGS-1){
		args[argc++]=token;
		token=strtok(NULL," \t");
	}
	args[argc]=NULL; /* this is critical q ke execvp() ki command need to know where argument ends */
	return argc;
}
