#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include "random437.h"

//// Global Variables ////

int totArrive, totRiders, totReject, avgWaitTime, lineSize; //variables for tracking

//// Function Declarations ////
int lineAdder( int t ); // will this be int or void?
// int carFunction() // what are we passing? Carnum?



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

/*
carFunction()
    wait on mutex variables
    with permission
if lineSize > carSize
        remove carSize from lineSize
else
    lineSize = 0
    release mutex variables
  */
