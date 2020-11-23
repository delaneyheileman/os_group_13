#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include "random437.h"

//// Global Variables ////
FILE *output;
int totArrive, totRiders, totReject, avgWaitTime, lineSize; //variables for tracking
int curTime = 0; // global for checking time
int carNum, carSize; // globals to store data passed to main
int linePerm = 0;
int *carPerm;
//int carsGone;
pthread_t *carId;
pthread_mutex_t line_mutex = PTHREAD_MUTEX_INITIALIZER; //global mutex to regulate access to cs
pthread_mutex_t car_mutex = PTHREAD_MUTEX_INITIALIZER;



//// Function Declarations ////
void *lineAdder(); 
void *carFunction();


int main(int argc, char** argv){

	carNum = atoi( argv[1] ); 
	carSize = atoi( argv[2] );

	printf("Car size: %d, Car Number: %d\n", carSize, carNum);

	pthread_t lineId;
	carId = (pthread_t*)malloc(carNum*sizeof(pthread_t));
	carPerm = (int*)malloc(carNum*sizeof(int));
	linePerm = 0;
	/*
	for(int i = 0; i < carNum; i++) {
		carPerm[i] = 0;
		pthread_create( &(carId[i]), NULL, carFunction, NULL);
	}

	pthread_create( &lineId, NULL, lineAdder, NULL);
	*/
	//carsGone = carNum;
	while(curTime < 599) {
		//if( carsGone == carNum ) {
			for(int i = 0; i < carNum; i++) {
				carPerm[i] = 0;
				pthread_create( &(carId[i]), NULL, carFunction, NULL);
			}

			pthread_create( &lineId, NULL, lineAdder, NULL);
			
			pthread_mutex_lock(&line_mutex);
			pthread_mutex_lock(&car_mutex);
			linePerm = 1;
			//carsGone = 0;
			pthread_mutex_unlock(&line_mutex);
			pthread_mutex_unlock(&car_mutex);
			
			usleep(10000);
			printf("Current Time: %d\n", curTime);
			for(int i = 0; i < carNum; i++) {
				pthread_join( (carId[i]), NULL);
			}
			pthread_join( lineId, NULL );
			curTime++;
		//}
	}
}


void *lineAdder(){
  pthread_mutex_lock(&line_mutex);
  pthread_mutex_lock(&car_mutex);
  if(linePerm == 1) {
    int meanArrival, newWaiters, newLine, rejected;
    printf("time : %d \n", curTime);
    
    if (curTime >= 0 && curTime < 120) { //time is between 9 - 10:59:59
    meanArrival = 25;
    printf("%d mean arrival \n", meanArrival);
    }
    if (curTime >= 120 && curTime < 300) { //time is between 11 - 13:59:59
    meanArrival = 45;
    printf("%d mean arrival \n", meanArrival);
    }
    if (curTime >= 300 && curTime < 420) { //If time is between 14 - 15:59:59
    meanArrival = 35;
    printf("%d mean arrival \n", meanArrival);
    }
    if (curTime >= 420 && curTime < 600) { //  If time is between 16 - 18:59:59
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
    linePerm = 0;
    for(int i = 0; i < carNum; i++) {
      carPerm[i] = 1;
      output = fopen("PA06_out.txt", "a");
      fprintf(output, "%d arrive %d reject %d wait-line %d at %d\n", curTime, newWaiters, 
		  rejected, lineSize, curTime); //will need to change the last var here to give HH:MM:SS
      fclose(output); 
 
    }
    usleep(10);
  }
  pthread_mutex_unlock(&line_mutex);
  pthread_mutex_unlock(&car_mutex);
  
 //need to output to file    output( curTime, newWaiters, rejected, lineSize ) 

  return 0;
}

void *carFunction() {
		pthread_mutex_lock(&car_mutex);
		//int mutex_checker = pthread_cond_wait( &car_mutex );
		int threadIndex = 0;
		for(int i = 0; i < carNum; i++) {
			if(carId[i] == pthread_self()) {
				threadIndex = i;
			}
		}
		if(carPerm[threadIndex]) {
			//if(mutex_checker == 0) {
				if (lineSize > carSize)
					lineSize -= carSize;
				else
					lineSize = 0;
			//}
			printf("Car %d arrives; it takes <= %d people from line of %d people\n",
					threadIndex, carSize, lineSize);
			carPerm[threadIndex] = 0;
			//carsGone++;
		}
		pthread_mutex_unlock(&car_mutex);
		usleep(10);
}
