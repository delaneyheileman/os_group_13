#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>

int toggle = 0; //initializing the global toggle value used to toggle the yes function
/*
void child_signalHandler(int sig) {
	if( sig == 2 ) {
		printf("\nIn child signal handler, in ctrl+c condition\n");
		kill(getpid(),19);
	}
	else if( sig == 20 ) {
		printf("\nIn child signal handler, in ctrl+z condition\n");
		if(toggle == 0) {
			kill(getpid(), 19); //pauses the process (stops it)
			toggle = 1;
		}
		else {
			kill(getpid(), 18); //continues the process
			toggle = 0;
		}
	}
}
*/

void parent_signalHandler(int sig) {
	if(sig == 2) {
		printf("\nIn parent signal handler, in ctrl+c condition\n");
		//wait(NULL);
		toggle = 2;
		return;
	}
	else if(sig == 20) {
		printf("\nIn parent signal handler, in catch condition\n");
		toggle = (toggle == 0 ? 1 : 0 );
		return;
	}
	return;
}

int main() {
	//the below lines are used to execute execve, which replaces the child
	//process with a new process 
	//(ideally, a 'null' process and the yes process
	char *args[] = {"/bin/yes", NULL, 0};
	char *env[] = { 0 };

	pid_t pid = fork(); //creates a child process

	if(pid == 0) { //child process
		/*struct sigaction actc; 
		actc.sa_handler = child_signalHandler; 
		sigemptyset(&actc.sa_mask); 
		actc.sa_flags = 0;

		sigaction(SIGINT, &actc, 0);
		struct sigaction actc2;
		actc2.sa_handler = child_signalHandler;
		sigemptyset(&actc2.sa_mask);
		//actc2.sa_flags = SA_NODEFER; 

		sigaction(SIGTSTP, &actc2, 0); 
		*/
		execve("/bin/yes", args, env);
	}	
	else if(pid < 0 ) { //error condition
		printf("error! oops");
	}
	else { //parent process
		struct sigaction actp; 
		actp.sa_handler = parent_signalHandler; 
		sigemptyset(&actp.sa_mask); 
		actp.sa_flags = 0;

		sigaction(SIGINT, &actp, 0);
		struct sigaction actp2;
		actp2.sa_handler = parent_signalHandler;
		sigemptyset(&actp2.sa_mask);
		//actp2.sa_flags = SA_NODEFER; 

		sigaction(SIGTSTP, &actp2, 0); 

		//wait(NULL);	
	
		while(1){
			if(toggle == 0) {
				kill(pid,18);
			}
			else if (toggle == 1) {
				kill(pid, 19);
			}
			else if (toggle == 2) {
				kill(pid, 9);
			}
			wait(NULL);
		}
	}
	

	return 0;
}
