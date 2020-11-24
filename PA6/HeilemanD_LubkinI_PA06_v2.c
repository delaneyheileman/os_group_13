#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <pthread.h>
#include <string.h>
#include "random437.h"

//// Global Variables ////
FILE *output, *outputcsv;
int totArrive, totRiders, totReject, avgWaitTime, lineSize; //variables for tracking
int curTime = 0; // global for checking time
int carNum, carSize; // globals to store data passed to main
pthread_t *carId;
pthread_mutex_t line_mutex = PTHREAD_MUTEX_INITIALIZER; //global mutex to regulate access to cs
pthread_mutex_t car_mutex = PTHREAD_MUTEX_INITIALIZER;
char buffer [255];
char txtname[255], csvname[255];


//// Function Declarations ////
void *lineAdder(); 
void *carFunction();


int main(int argc, char** argv){

	carNum = atoi( argv[1] ); 
	carSize = atoi( argv[2] );

	char str[100];

	printf("Please enter the date:");
	scanf("%s", str);

	sprintf(buffer, "PA06_out_%d-%d-%s", carNum, carSize, str);
	strcpy(txtname, buffer);
	strcpy(csvname, buffer);
	strcat(txtname, ".txt");
	strcat(csvname, ".csv");
	
	outputcsv = fopen(csvname, "a");
	fprintf(outputcsv, "curTime, newWaiters, rejected, lineSize\n");
	fclose(outputcsv);

	pthread_t lineId;
	carId = (pthread_t*)malloc(carNum*sizeof(pthread_t));
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	while(curTime < 599) {
			//printf("\nCurrent Time: %d\n", curTime);
			pthread_create( &lineId, &attr, lineAdder, NULL);
			pthread_join( lineId, NULL );
			usleep(1400); //wait for car loading, 7ms = 7sec
			for(int i = 0; i < carNum; i++) {
				pthread_create( &(carId[i]), &attr, carFunction, NULL);
			}
			for(int i = 0; i < carNum; i++) {
				pthread_join( (carId[i]), NULL);
			}
			usleep(10600); //wait for car travel, 53 ms = 53 sec
			printf("\rRunning... time: %d", curTime);
			fflush(stdout);
			
			curTime++;
	}
	
	output = fopen(txtname, "a");
	fprintf(output, "Summary: Total Arrived: %d, Total Rejected: %d\n"
			, totArrive, totReject);//need to add wait time and max line size
	fclose(outputcsv);
	printf("\rexecution finished sucessfully\n");
	pthread_exit(NULL);
	return 0;
}


void *lineAdder(){
  pthread_mutex_lock(&line_mutex);
  pthread_mutex_lock(&car_mutex);
    int meanArrival = 0, newWaiters = 0, newLine = 0, rejected = 0;
    //printf("time : %d \n", curTime);
    
    if (curTime >= 0 && curTime < 120) { //time is between 9 - 10:59:59
    meanArrival = 25;
    //printf("%d mean arrival \n", meanArrival);
    }
    if (curTime >= 120 && curTime < 300) { //time is between 11 - 13:59:59
    meanArrival = 45;
    //printf("%d mean arrival \n", meanArrival);
    }
    if (curTime >= 300 && curTime < 420) { //If time is between 14 - 15:59:59
    meanArrival = 35;
    //printf("%d mean arrival \n", meanArrival);
    }
    if (curTime >= 420 && curTime < 600) { //  If time is between 16 - 18:59:59
    meanArrival = 25;
    //printf("%d mean arrival \n", meanArrival);
    }
    
    
    newWaiters = poissonRandom(meanArrival);
    //printf("%d added to line \n", newWaiters);
    
    totArrive = totArrive + newWaiters;
    newLine = newWaiters + lineSize;
    //printf("new line: %d \n", newLine);
    
    if (newLine <=800) {
    lineSize = newLine;
    //printf("%d -> valid line size \n", lineSize );
    }
    else if (newLine > 800) {
    rejected = newLine - 800;
    lineSize = 800;
    //printf("%d people in line - sorry return when the ride is less busy \n", lineSize );
    totReject = totReject + rejected;
    }
    //printf("Line adder ran. Line size: %d\n", lineSize);
      output = fopen(txtname, "a");
      outputcsv = fopen(csvname, "a");
     // output = fopen("PA06_out.txt", "a");
      fprintf(output, "%d arrive %d reject %d wait-line %d at %d\n", curTime, newWaiters, rejected, lineSize, curTime); //will need to change the last var here to give HH:MM:SS
      fprintf(outputcsv, "%d, %d, %d, %d\n", curTime, newWaiters, rejected, lineSize);
      fclose(outputcsv);
      fclose(output); 
 
  pthread_mutex_unlock(&line_mutex);
  pthread_mutex_unlock(&car_mutex);
  
  return 0;
}

void *carFunction() {
		pthread_mutex_lock(&car_mutex);
		pthread_mutex_lock(&line_mutex);
		int threadIndex = 0;
		for(int i = 0; i < carNum; i++) {
			if(carId[i] == pthread_self()) {
				threadIndex = i;
			}
		}
				if (lineSize > carSize)
					lineSize -= carSize;
				else
					lineSize = 0;
			//printf("Car %d arrives; it takes <= %d people from line of %d people\n", ,threadIndex, carSize, lineSize);
		pthread_mutex_unlock(&car_mutex);
		pthread_mutex_unlock(&line_mutex);
		//printf("Car %d Done\n", threadIndex);
}
