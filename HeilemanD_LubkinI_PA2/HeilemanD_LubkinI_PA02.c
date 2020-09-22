#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

//sigint is numbered 2 on x86 and sigtstp is numbered 20
int toggle = 0; //initializing the global toggle value used to toggle the yes function

/*for some reason, the program exits after calling ctrl+z.
 * It correctly reads the signal and enters the elseif
 * but then instead of continuing the child process, it
 * terminates the child process.
 * I have not been able to resolve this or figure out why
 * it is happening.
 */

/*The function userDef_signalHandler is called by the sigaction function
 * in the main program when ctrl+z or ctrl+c are entered in terminal. It 
 * should terminate the child and parent if ctrl+c is pressed and toggle
 * the 'yes' function if ctrl+z is pressed.
 * Currently, the function is entered correctly upon signaling and the correct
 * branch of the conditional is entered for each signal, but upon completion
 * the child process which called the handler terminates, which it should not.
 */
void userDef_signalHandler(int sig) {
	if( sig == 2 ) {
		printf("\nkilling child process from ctrl+c\n");
		exit(1); //when this is commented out ctrl+c still terminates the child (it shouldn't...)
	}
	else if( sig == 20 ) {
		char *args[] = {"/bin/yes", NULL, 0};
		char *env[] = { 0 };
		char *args1[] = {"/bin/sleep", "0.000001", NULL};
		char *env1[] = { 0 };
		printf("\nin sigtstp\n");
		if(toggle == 0) {
			while(1) {
			printf("\nin handler while\n");
			execve("/bin/sleep", args1, env1);	
			toggle = 1;
			}
		}
		else {
			execve("/bin/yes", args, env);	
			toggle = 0;
		}
		
	}
}


int main() {
	/*Lines 49 to 62 define the catching of the SIGINT and SIGTSTP signals. 
	 *The signals are redirect to userDef_signalHandler, above.
	 */
	int wstatus; //this will hold the return status of the waitpid function 
	//code to catch SIGINT
	struct sigaction act; //this struct is used to call the sigaction f'n
	act.sa_handler = userDef_signalHandler; //this defines the new handler
	sigemptyset(&act.sa_mask); //this empties the mask; the mask blocks certain signals from being received *while in the newly defined handler*. This is not needed in our program.
	act.sa_flags = 0;

	sigaction(SIGINT, &act, 0); //catches SIGINT
	//code to catch SIGTSTP
	struct sigaction act2;
	act2.sa_handler = userDef_signalHandler;
	sigemptyset(&act2.sa_mask);
	act2.sa_flags = SA_NODEFER; //this should allow the ctrl+z call to be received while in the user defined handler, allowing us to exit the loop in that handler to toggle.

	sigaction(SIGTSTP, &act2, 0); //catches SIGTSTP
	
	//the below lines are used to execute execve, which replaces the child
	//process with a new process 
	//(ideally, a 'null' process and the yes process
	char *args[] = {"/bin/yes", NULL, 0};
	char *env[] = { 0 };
	char *args1[] = {"/bin/sleep", "0.000001", NULL};
	//char *args1[] = {"/bin/sleep", "10", NULL}; //i was using this to test of execve exits after one spin
	//notably! if toggle is set to 1, \so that the while loop in the child below is entered imediately, the program exits after the first completion of the execve. This is the problem: upon execve completion the child process terminates.  
	char *env1[] = { 0 };

	pid_t pid = fork(); //creates a child process
while(1) {
	if(pid == 0) { //child process
		/*
		//this while loop constantly sleeps for a us, until toggled
		//unfortunately, it does not appear to be entered ever
		while( toggle == 1 ) { 
		execve("/bin/sleep", args1, env1);
		}
		execve("/bin/yes", args, env);//when toggle !=1, yes is executed 
		*/
		while(1){}
	}	
	else if(pid < 0 ) { //error condition
		printf("error! oops");
	}
	else { //parent process
		/*This code is intended to wait for the child state to change
		 *(which is what waitpid does) and only exit if the state
		 *change was from an exit call. Signals count as state changes.
		 */
		waitpid(pid, &wstatus, 0);
		printf("parent process exiting\n");
		if(WIFEXITED(wstatus))
			break;
	}
}

	return 0;
}
