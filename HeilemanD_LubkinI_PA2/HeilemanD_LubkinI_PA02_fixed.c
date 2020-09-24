/*Authors: Delaney Heileman and Ian Lubkin
 *Last edit: September 23 2020 at 7:30 pm EST
 *Purpose: This program creates a child process which begins the linux yes command.
 *It also defines handlers for SIGTSTP and SIGINT, which will toggle the child process,
 *and terminate both, respectively.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

int toggle = 1; //initializing the global toggle value used to toggle the yes function
pid_t pid;
void parent_signalHandler(int sig) {
		printf("\nctrl+c caught, terminating both processes\n");
		kill(getpid(), SIGKILL);
}

void toggle_signalHandler(int sig) {
		toggle = (toggle == 0 ? 1 : 0 );
		if( toggle == 0 ) {
			printf("\nctrl+z caught, stopping child process\n");
			kill(pid, SIGTSTP);
		}
		else {
			printf("\nctrl+z caught, resuming child process\n");
			kill(pid, SIGCONT);
		}
}

int main() {
	signal( SIGINT, parent_signalHandler );
	signal( SIGTSTP, toggle_signalHandler );

	char* const arg1[] = {"yes", NULL};

	pid = fork();

	if(pid == 0) { //child process
		//execve("/bin/yes", args, env);
		execv("/usr/bin/yes",arg1); 
	}	
	else if(pid < 0 ) { //error condition
		perror("Fork error");
	}
	else { //parent process
		wait(NULL);l
	}
	while(1){}

	return 0;
}
