#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){

	char *a = argv[1];
	char *b = argv[2];
	
	//printf("a: %c, b: %c\n", a, b);

	int carNum = atoi(a);
	int carSize = atoi(b);	
/*
	int carNum = (int)*(argv[1]);
        int carSize = (int)*(argv[2]);	
*/
	printf("Car Number: %d\nCar Size: %d\n", carNum, carSize);

	return 0;
}
