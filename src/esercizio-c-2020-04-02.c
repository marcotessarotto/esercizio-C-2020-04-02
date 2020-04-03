#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_CHARS 80
/*
il programma svolge le attività sotto descritte fino a quando incontra EOF su stdin.

1: il programma legge caratteri da stdin.
Quando incontra EOF, il programma scrive quanti processi figli ha lanciato in tutto e poi termina.

Il programma accumula i caratteri letti da stdin in un array di dimensione 80 chiamato char_array.
(introdurre nel codice un #define NUM_CHARS 80).

Quando char_array è pieno, passa tutto l'array ad un nuovo processo figlio
che fa delle analisi che scrive su stdout (analisi sotto riportate).

Dopo aver lanciato il processo figlio, il processo principale azzera char_array ed il programma continua da 1:


*analisi svolte dai processi figlio (sull'array di 80 caratteri ricevuto)*
carattere minimo
carattere massimo
carattere con più occorrenze
carattere con meno occorrenze

fatta l'analisi e scritto i risultati , il processo figlio termina.
*/

void reset_array(char * array) {
	array = memset(array,0,NUM_CHARS*sizeof(char));
}

char find_min (char * array) {
	char min = array[0];
	for(int i=1; i<NUM_CHARS; i++){
		if(array[i] < min)
			min = array[i];
	}
	return min;
}

char find_max (char * array) {
	char max = array[0];
	for(int i=1; i<NUM_CHARS; i++){
		if(array[i] > max){
			max = array[i];
		}
	}
	return max;
}

char find_more (char * array){
	int more_counter = 0;
	char more = array[0];
	for(int i=0; i<NUM_CHARS; i++){
		int counter = 0;
		for(int j=0; j<NUM_CHARS; j++){
			if(array[i]==array[j])
				counter++;
		}
		if(more_counter < counter){
			more_counter = counter;
			more = array[i];
		}
	}

	return more;

}

char find_less (char * array){
	int less_counter = 10000;
	char less = array[0];
	for(int i=0; i<NUM_CHARS; i++){
		int counter = 0;
		for(int j=0; j<NUM_CHARS; j++){
			if(array[i]==array[j])
				counter++;
		}
		if(less_counter > counter){
			less_counter = counter;
			less = array[i];
		}
	}

	return less;

}

int main (int argc, char * argv[]) {
	char input;
	char * char_array = calloc(NUM_CHARS, sizeof(char));
	if(char_array==NULL){
		perror("calloc ha fallito!");
		exit(1);
	}
	unsigned int i=0;
	unsigned int child_process_number=0;
	printf("Immetti quanti caratteri vuoi:\n\n");
	while((input = getchar())!=EOF){
		char_array[i]=input;
		i++;
		if(i == NUM_CHARS){
			switch(fork()) {
				case 0:
					printf("\n");
					printf("\nSono il processo figlio\n");
					char minimum = find_min(char_array);
					printf("\nIl carattere dal valore minore è: %c",minimum);
					char maximum = find_max(char_array);
					printf("\nIl carattere dal valore maggiore è: %c",maximum);
					char more = find_more(char_array);
					printf("\nIl carattere ripetuto più volte è: %c",more);
					char less = find_less(char_array);
					printf("\nIl carattere ripetuto meno volte è: %c",less);
					free(char_array);

					exit(0);
					break;
				case -1:
					printf("fork() ha fallito! niente processo figlio!\n");

					exit(1)
					break;
				default:
					printf("\nSono il processo padre\n");
					child_process_number++;
					if (wait(NULL) == -1)
						perror("wait error");
					reset_array(char_array);
					i=0;

					break;
			}
		}
	}

	free(char_array);
	printf("Sono stati generati %d processi figli!",child_process_number);

	return 0;

}
