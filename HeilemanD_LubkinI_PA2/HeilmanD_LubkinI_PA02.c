#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

//sigint is numbered 2 on x86 and sigtstp is numbered 20


int main() {
	pid_t pid = fork();

	if(pid == 0) {
		//child process
		execve("/bin/yes"); //may not need to include /bin
	}	
	else if(pid < 0 ) {
		//error condition
	}
	else {
		//parent process
	};


	return 0;
}
