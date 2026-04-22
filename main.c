#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "defs.h"
#include "parser.h"
#include "executor.h"
#include "builtins.h"
int main(void){
	char input[MAX_INPUT];
	char *args[MAX_ARGS];
	while(1){
		printf("My_Shell$ ");
		fflush(stdout);
		if(fgets(input,sizeof(input),stdin)==NULL){
			/* Ctrl+D pressed — exit cleanly */
			printf("\n");
			break;
		}
		input[strcspn(input,"\n")]='\0'; // new line find krke usko delete krdeta hai agar na krta toh for suppose hum ls enter krte to ls and then agli line main chale jate command lost hojati
		if(strlen(input)==0) continue;
		//printf("You Typed: %s\n",input);
		int argc=parse_input(input,args);
		if(argc==0) continue;
		if(strcmp(args[0],"exit")==0) break;
		if(is_builtin(args)){
			handle_builtin(args);
		}else{
			execute_command(args);
		}
	}
	return 0;
}
