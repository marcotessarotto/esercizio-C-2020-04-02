// Diaa Nehme  IN0500345  esercizio20200402

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

#define NUM_CHARS 80

// Minimum Function
char minChar(char * char_array){
	char minimum = CHAR_MAX;

	for(int i = 0; i < NUM_CHARS; i++){
		if(char_array[i] < minimum)
			minimum = char_array[i];
	}
	return minimum;
}

// Maximum Function
char maxChar(char * char_array){
	char maximum = CHAR_MIN;

	for(int i = 0; i < NUM_CHARS; i++){
		if(char_array[i] > maximum)
			maximum = char_array[i];
	}
	return maximum;
}

// Less Occurrence Functon
char leastOccurrence(char * char_array){
	int currentIndex;
	int minimumCounter = 5 * NUM_CHARS;	//choose a random maximum number of occurrences

	for(int i = 0; i < NUM_CHARS; i++){	//occorrence control
		char temp = char_array[i];
		int counter = 0;	//counter of the occurrences of the current character
		for(int j = 0; j < NUM_CHARS; j++){
			if(temp == char_array[j])
				counter++;
		}
		if(counter < minimumCounter){	//smallest occurrence check
			minimumCounter = counter;
			currentIndex = i;
		}
	}
	return char_array[currentIndex];
}

// More Occurrence Function
char mostOccurrence(char * char_array){
	int currentIndex;
	int maximumCounter = -1;	//choose a random minimum number of occurrences

	for(int i = 0; i < NUM_CHARS; i++){ //occorrence control
		char temp = char_array[i];
		int counter = 0;    //counter of the occurrences of the current character
		for(int j = 0; j < NUM_CHARS; j++){
			if(temp == char_array[j])
				counter++;
		}
		if(counter > maximumCounter){   //biggest occurrence check
			maximumCounter = counter;
			currentIndex = i;
		}
	}
	return char_array[currentIndex];
}

int main(int argc, char * argv[]) {


	char char_array[NUM_CHARS] = {0};
	char zero_array[NUM_CHARS] = {0};	//array of zeros with dimension 80
	int i = 0;	//ndices of char_array
	int childNumber = 0;	//The number of the created child process

	printf("Insert some characters: ");

	while ((char_array[i] = getchar()) != EOF) {
		i++;
		if(i == NUM_CHARS){	//check if the array is full!
			switch (fork()) {
				case -1:
				{
					printf("fork() have no success!!!\n");

					exit(1);
					break;
				}
				case 0:
				{
					printf("\nThe minimum char in the array inserted : %c", minChar(char_array));
					printf("\nThe maximum char in the array inserted : %c", maxChar(char_array));
					printf("\nThe character with the least occurrence : %c", leastOccurrence(char_array));
					printf("\nThe character with the most occurrence : %c", mostOccurrence(char_array));
					exit(0);
					break;
				}
				default:
				{
					i = 0;
					memcpy(char_array, zero_array, NUM_CHARS);	//azzeramento di char_array
					childNumber++;	//incremento del contatore del numero di figli creati
					printf("I am the parent process\nThe number of the created child process = %d\n", childNumber);
				}
			}
		}
	}


	return 0;
}

