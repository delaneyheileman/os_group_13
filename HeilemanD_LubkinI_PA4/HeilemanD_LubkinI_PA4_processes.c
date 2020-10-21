#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>

struct Banks {int balance[2];} Bank={{100,100}}; //global variable defined
void* MakeTransactions() { //routine for thread execution
        int i, j, tmp1, tmp2, rint;
        double dummy;
        for (i=0; i < 100; i++) {
                rint = (rand()%30)-15;
                if (((tmp1=Bank.balance[0])+rint)>=0 && ((tmp2=Bank.balance[1])-rint)>=0) {
                Bank.balance[0] = tmp1 + rint;
                for (j=0; j < rint*1000; j++) {
                        dummy=2.345*8.765/1.234;
                } // spend time on purpose
                        Bank.balance[1] = tmp2 -rint;
                }
        }
        return NULL;
}

int main(int argc, char **argv) {
	pid_t pid; // variable to store process id's
	//setting up shared memory
	int shmid; //for storing the shared memory id
	key_t key = ftok("shmfile",65); //creates the file to be shared, returns a key
	if( (shmid = shmget(key, 1024, IPC_CREAT | 0666)) < 0 ) { //create shm, return id
		perror("shmget");
		exit(1);
	} //if the shmget fails, call perror and exit with condition 1
	
	struct Banks *b = (struct Banks*) shmat(shmid, NULL, 0); //attach a bank pointer to shared memory
	b = &Bank; //let b point to Bank

	//creating and running processes
	srand(getpid()); //set the rand() seed
	printf("Init balances A:%d + B:%d ==> %d!\n",
	Bank.balance[0],Bank.balance[1],Bank.balance[0]+Bank.balance[1]);
	if ( (pid=fork()) < 0) {
		perror("Error in process creation\n");
		return(1);
	}
	else if( pid == 0 ) { //child process
		MakeTransactions();
	}
	else { //parent process
		MakeTransactions();
	}

	printf("Let's check the balances A:%d + B:%d ==> %d ?= 200\n",
	Bank.balance[0],Bank.balance[1],Bank.balance[0]+Bank.balance[1]);
}

