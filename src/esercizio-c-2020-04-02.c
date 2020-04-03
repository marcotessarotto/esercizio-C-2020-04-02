#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_CHARS 80

void array_elaboration(char * array, int dimension);

int main(int argc, char *argv[]) {
	char c;
	int counter = 0;
	int sonNumber = 0;
	char * char_array = calloc(NUM_CHARS, sizeof(char));

	if(char_array == NULL){
		perror("Errore in calloc");
		exit(1);
	}

	while((c = getchar()) != '\n'){
		char_array[counter] = c;

		counter += 1;

		if(counter == NUM_CHARS){

			switch(fork()){
			case 0:
				array_elaboration(char_array, NUM_CHARS);
				exit(0);
				break;

			case -1:
				printf("fork() ha fallito! niente processo figlio!\n");

				exit(1);
				break;

			default:
				for(int i = 0; i < NUM_CHARS; i++){
					char_array[i] = 0;
				}
				counter = 0;
				sonNumber += 1;
				break;

			}

		}

	}

	printf("Il processo padre ha lanciato %d processi figli.\n\n", sonNumber);
	for(int i = 0; i < sonNumber; i++){
		wait(NULL);
	}
	printf("Tutti i figli hanno terminato, ora chiudo anche il padre.\n");
	return 0;
}


void array_elaboration(char * array, int dimension){
	int pid = getpid();
	char min = array[0];
	char max = array[0];

	int posOfMax = 0;
	int posOfMin = 0;

	int counter = 0;
	char * support_array = calloc(dimension, sizeof(char));
	int * occurrences = calloc(dimension, sizeof(int));


	if(support_array == NULL || occurrences == NULL){
			perror("Errore in calloc");
			exit(1);
		}


	for(int i = 1; i < dimension; i++){
		if(array[i] < min){
			min = array[i];
		}

		if(array[i] > max){
			max = array[i];
		}
	}

	for(int i = 0; i < dimension; i++){
		for(int j = 0; j <= counter; j++){
			if(array[i] == support_array[j]){
				occurrences[j] += 1;
				break;
			}

			if(j == counter){
				support_array[j] = array[i];
				occurrences[j] += 1;
				counter++;
				break;
			}
		}
	}

	for(int i = 0; i < counter; i++){
		if(occurrences[i] < occurrences[posOfMin]){
			posOfMin = i;
		}

		if(occurrences[i] > occurrences[posOfMax]){
			posOfMax = i;
		}
	}

	printf("Il figlio con PID %d ha elaborato le seguenti informazioni:\n", pid);
	printf("Carattere minimo: '%c' \n",min);
	printf("Carattere massimo: '%c' \n",max);
	printf("Carattere con meno occorrenze: '%c' (%d)\n", support_array[posOfMin], occurrences[posOfMin]);
	printf("Carattere con piu` occorrenze: '%c'(%d)\n\n\n", support_array[posOfMax], occurrences[posOfMax]);

}
