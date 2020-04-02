#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define NUM_CHARS 80

unsigned char min_char (unsigned char * array, unsigned int dim);
unsigned char max_char (unsigned char * array, unsigned int dim);
unsigned char most_frequent_char (unsigned char * array, unsigned int dim);
unsigned char less_frequent_char (unsigned char * array, unsigned int dim);
unsigned char only_unrepeated_char (unsigned char * array, unsigned int dim);
unsigned int is_unrepeated (unsigned char src, unsigned char * dest, unsigned int dim);


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
	-carattere minimo
	-carattere massimo
	-carattere con più occorrenze
	-carattere con meno occorrenze

	fatta l'analisi e scritto i risultati , il processo figlio termina.
 */

int main(int argc, char *argv[]) {

	int c;
	unsigned long char_counter = 0;
	unsigned char * char_array;

	char_array = calloc(NUM_CHARS, sizeof(unsigned char));

	if (char_array == NULL) {
		perror("calloc error!");
		exit(EXIT_FAILURE);
	}

	while ((c = getchar()) != EOF) {
		char_counter++;

		switch (fork()) {
			case 0:
				printf("sono il processo figlio\n");

				exit(0);
				break;
			case -1:
				printf("fork() ha fallito! niente processo figlio!\n");

				exit(1);
				break;
			default: // valore > 0, ovvero il pid del processo figlio

				printf("sono il processo padre\n");

				exit(0);
		}

	}

}

unsigned char min_char (unsigned char * array, unsigned int dim) {
	unsigned int min;
	min  = (unsigned int) array[0];

	for (int i = 1; i < dim; i++) {
		if ((unsigned int) array[i] < min)
			min = (unsigned int) array[i];
	}

	return (unsigned char) min;
}

unsigned char max_char (unsigned char * array, unsigned int dim) {
	unsigned int max;
	max  = (unsigned int) array[0];

	for (int i = 1; i < dim; i++) {
		if ((unsigned int) array[i] > max)
			max = (unsigned int) array[i];
	}

	return (unsigned char) max;
}

unsigned char most_frequent_char (unsigned char * array, unsigned int dim) {
	unsigned char * unrepeated_char;
	unsigned int * occurrences;
	unsigned int size;
	unsigned int most_frequent;

	unrepeated_char = only_unrepeated_char(array, dim);
	size = sizeof(unrepeated_char) / sizeof(unsigned char);

	occurrences = calloc(size, sizeof(unsigned int));

	if (occurrences == NULL) {
		perror("calloc error!");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < size; i++) {
		for (int j = 0; i < dim; j++) {
			if (array[j] == unrepeated_char[i])
				occurrences[i]++;
		}
	}

	most_frequent = occurrences[0];

	for (int i = 1; i < size; i++) {
		if (occurrences[i] > most_frequent)
			most_frequent = occurrences[i];
	}

	return unrepeated_char[most_frequent];
}

unsigned char less_frequent_char (unsigned char * array, unsigned int dim) {
	unsigned char * unrepeated_char;
	unsigned int * occurrences;
	unsigned int size;
	unsigned int less_frequent;

	unrepeated_char = only_unrepeated_char(array, dim);
	size = sizeof(unrepeated_char) / sizeof(unsigned char);

	occurrences = calloc(size, sizeof(unsigned int));

	if (occurrences == NULL) {
		perror("calloc error!");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < size; i++) {
		for (int j = 0; i < dim; j++) {
			if (array[j] == unrepeated_char[i])
				occurrences[i]++;
		}
	}

	less_frequent = occurrences[0];

	for (int i = 1; i < size; i++) {
		if (occurrences[i] < less_frequent)
			less_frequent = occurrences[i];
	}

	return unrepeated_char[less_frequent];
}

unsigned char only_unrepeated_char (unsigned char * array, unsigned int dim) {
	unsigned char * unrepeated_char;
	unsigned int size = 1;
	unsigned int index = 0;
	unrepeated_char = calloc(size, sizeof(unsigned char));

	if (unrepeated_char == NULL) {
		perror("calloc error!");
		exit(EXIT_FAILURE);
	}

	unrepeated_char[index] = array[index];

	for (int i = 1; i < dim; i++) {
		if (is_unrepeated(array[i], unrepeated_char, size)) {
			size++;
			index++;

			unrepeated_char = realloc(unrepeated_char, size*sizeof(unsigned char));

			if (unrepeated_char == NULL) {
				perror("calloc error!");
				exit(EXIT_FAILURE);
			}

			unrepeated_char[index] = array[i];
		}
	}

	return unrepeated_char;
}

unsigned int is_unrepeated (unsigned char src, unsigned char * dest, unsigned int dim) {
	for (int i = 0; i < dim; i++) {
		if (src == dest[i])
			return 0;
	}

	return 1;
}


