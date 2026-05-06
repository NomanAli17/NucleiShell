#include "defs.h"
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include "signals.h"
#include "jobs.h"
//this happens when sigchild is called sigchild is call made by OS to process that child has changed his state
static void sigchld_handler(int sig){
	(void)sig;
	pid_t pid;
	while((pid=waitpid(-1,NULL,WNOHANG))>0){
		remove_job(pid);
	}
}
static void sigint_handler(int sig){
	(void)sig;
	write(STDOUT_FILENO,"\nMy_Shell$ ",11);
}
void setup_signals(void){
	struct sigaction sa_chld;//yeh default action hota hai kisi bhi signal ka
	sa_chld.sa_handler=sigchld_handler;//hum use krte hain apna banaya huwa function to hand this
	sigemptyset(&sa_chld.sa_mask);//is se koi aur signal disturb nhi hota
	sa_chld.sa_flags=SA_RESTART;//is se hum baki jo system calls ruk jati hain unko dubara continue krdete hain
	sigaction(SIGCHLD,&sa_chld,NULL);
	
	struct sigaction sa_int;
	sa_int.sa_handler=sigint_handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags=SA_RESTART;
	sigaction(SIGINT,&sa_int,NULL);
}
