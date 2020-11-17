#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include "random437.h"

//// Global Variables ////

int totArrive, totRiders, totReject, avgWaitTime, lineSize; //variables for tracking
int time; // global for checking time
pthread_mutex_t mutex = PTHREAD_MUTEX_INTIALIZER; //global mutex to regulate access to cs
//pthread_mutex_t mutex = PTHREAD_MUTEX_INTIALIZER;
//pthread_mutex_t mutex = PTHREAD_MUTEX_INTIALIZER;


//// Function Declarations ////
int lineAdder(); // will this be int or void?
int carFunction(); // what are we passing? Carnum?
/* From Ian: The above functions should have void input; they are the thread functions
 * that we'll use to create the threads and can't have inputs (I think... they won't
 * need them anyways, they'll just use the globals)
 */


int main(){ // will pass number of cars and number of people in the car at command line

/*
  integer time
  create shared integer lineSize
  create mutex variables for inter-car communication
  create mutex variables for line adder communication (maybe merge w/ above)
*/

// just for testing line adder
  for (int t = 0; t < 599; t++) { // just dummy time to test lineAdder
    lineAdder(t); //calling lineAdder
  }

/*for( i < carNum )
    create_thread( carFunction() )

create_thread( lineAdder() )

for( i < minutesInDay )
    join_thread( line add thread )
    for( i < carNum )
        join_thread( car )
    wait( wait time ) //or we could use an alarm?
*/

}


int lineAdder( int time ){
  //  check
  //  lock mutex access to lineSize
  int meanArrival, newWaiters, newLine, rejected;
  printf("time : %d \n", time);

  if (time >= 0 && time < 120) { //time is between 9 - 10:59:59
    meanArrival = 25;
    printf("%d mean arrival \n", meanArrival);
  }
  if (time >= 120 && time < 300) { //time is between 11 - 13:59:59
    meanArrival = 45;
    printf("%d mean arrival \n", meanArrival);
  }
  if (time >= 300 && time < 420) { //If time is between 14 - 15:59:59
    meanArrival = 35;
    printf("%d mean arrival \n", meanArrival);
  }
  if (time >= 420 && time < 600) { //  If time is between 16 - 18:59:59
    meanArrival = 25;
    printf("%d mean arrival \n", meanArrival);
  }


  newWaiters = poissonRandom(meanArrival);
  printf("%d added to line \n", newWaiters);

  totArrive = totArrive + newWaiters;
  newLine = newWaiters + lineSize;
  printf("new line: %d \n", newLine);

  if (newLine <=800) {
    lineSize = newLine;
    printf("%d -> valid line size \n", lineSize );
  }
  else if (newLine > 800) {
    rejected = newLine - 800;
    lineSize = 800;
    printf("%d people in line - sorry return when the ride is less busy \n", lineSize );
    totReject = totReject + rejected;
  }

  //need to output to file    output( time, newWaiters, rejected, lineSize ) */

  return 0;
}

carFunction()
	int mutex_checker;
	mutex_checker = pthread_mutex_lock(&mutex);
	if (mutex_checker) { 
    		perror("pthread_mutex_lock");
    		pthread_exit(NULL);
	} //else mutex sucessful
	mutex_checker = pthread_cond_wait( &mutex );
	if( mutex_checker == 0 ) {
		if (lineSize > carSize) //uh-oh - gotta set up shared access to carSize from main...
			lineSize -= carSize;
		else
			lineSize = 0;
	}
	pthread_mutex_unlock(&mutex);
