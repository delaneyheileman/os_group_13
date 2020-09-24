/*Authors: Delaney Heileman and Ian Lubkin
 *Last edit: September 23 2020 at 7:30 pm EST
 *Purpose: This program creates a child process which begins the linux yes command.
 *It also defines handlers for SIGTSTP and SIGINT, which will toggle the child process,
 *and terminate both, respectively. 
 *Notes: The toggling and termination of the child process is not handled directly in the
 *signal handler because it is not possible to pass a variable from main to the signal handler.
 *To resolve this issue, a global variable is used to communicate the effects of calling the
 *signal handler. The effects are as described above.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

int toggle = 1; //initializing the global toggle value used to toggle the yes function
//int signum = 0;
pid_t pid;
void parent_signalHandler(int sig) {
//	if(sig == SIGINT) {
		printf("\nctrl+c caught, terminating both processes\n");
		kill(getpid(), SIGKILL);
		//toggle = 2;
		//signum = 2;
		//exit(1);
//	}
}

void toggle_signalHandler(int sig) {
		//signum = 20;
		toggle = (toggle == 1 ? 0 : 1 );
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
	/*struct sigaction actp; 
	actp.sa_handler = parent_signalHandler; 
	sigemptyset(&actp.sa_mask); 
	actp.sa_flags = 0;

	sigaction(SIGINT, &actp, 0);
	
	struct sigaction actp2;
	actp2.sa_handler = parent_signalHandler;
	sigemptyset(&actp2.sa_mask);

	sigaction(SIGTSTP, &actp2, 0); 
	*/

	signal( SIGINT, parent_signalHandler );
	signal( SIGTSTP, toggle_signalHandler );

	//char *args[] = {"/bin/yes", NULL, 0};
	//char *env[] = { 0 };
	char* const arg1[] = {"yes", NULL};

	//pid_t pid = fork(); //creates a child process
	pid = fork();

	if(pid == 0) { //child process
		//execve("/bin/yes", args, env);
		execv("/usr/bin/yes",arg1); 
		/*while(1){
			if(toggle == 0)
				kill(getpid(),18);
			else if(toggle == 1)
				kill(getpid(),19);
			else if(toggle == 2 && signum == 2)
				exit(1);
		}*/
	}	
	
	else if(pid < 0 ) { //error condition
		perror("Fork error");
	}
	
	else { //parent process
		/*
		while(1){			
			
			if(toggle == 0) {
				//printf("\nctrl+z caught, resuming child process\n");
				kill(pid,18); //18 corresponds to SIGCONT, continue/resume the process
			}
			
			else if (toggle == 1) {
				//printf("\nctrl+z caught, stopping child process\n");
				kill(pid, 19); //19 corresponds to SIGSTOP, stopthe process
			}
			
			else if (toggle == 2) {
				kill(pid, 9); //9 corresponds to SIGKILL, terminate the process
				//wait(NULL);
			}
			
			wait(NULL);
		}*/
		wait(NULL);
	}
	while(1){}
	return 0;
}
