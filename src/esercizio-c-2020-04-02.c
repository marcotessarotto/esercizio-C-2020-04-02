#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define NUM_CHARS 10

void print_input (unsigned char * array, unsigned int dim);
void process_input (unsigned char * array, unsigned int dim);
void print_result (unsigned char min_char, unsigned char max_char, unsigned char most_frequent, unsigned char less_frequent);
unsigned char min_char (unsigned char * array, unsigned int dim);
unsigned char max_char (unsigned char * array, unsigned int dim);
unsigned char most_frequent_char (unsigned char * array, unsigned int dim);
unsigned char less_frequent_char (unsigned char * array, unsigned int dim);
unsigned char * only_unrepeated_char (unsigned char * array, unsigned int dim);
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
	unsigned int child_process_counter = 0;
	unsigned int char_counter = 0;
	unsigned char * char_array;

	char_array = calloc(NUM_CHARS, sizeof(unsigned char));

	if (char_array == NULL) {
		perror("calloc error!");
		exit(EXIT_FAILURE);
	}

	printf("Insert text here: ");

	while ((c = getchar()) != EOF) {
		if (char_counter == NUM_CHARS) {
			switch (fork()) {
				case 0:
					printf("\n****************************************\n");
					printf("\t [child %d] operation:\n", child_process_counter);
					process_input(char_array, NUM_CHARS);
					printf("\n****************************************\n");
					free(char_array);
					exit(0);
					break;

				case -1:
					printf("fork() ha fallito!\n");
					exit(1);
					break;

				default:
					child_process_counter++;
					wait(NULL);
					memset(char_array, 0, NUM_CHARS*sizeof(unsigned char));
					char_counter = 0;
					break;
			}
		} else if (c > 31 && c < 127) {
			char_array[char_counter] = c;
			char_counter++;
		}

	}
	printf("\n****************************************\n");
	printf("\t Number of child process launched: %d\n", child_process_counter);

	free(char_array);

	exit(0);
}

void print_input (unsigned char * array, unsigned int dim) {
	printf("\t Input: ");

	for (int i = 0; i < dim; i++) {
		if ((unsigned int) array[i] != 0)
			printf("%c", array[i]);
	}

	printf("\n");
}

void process_input (unsigned char * array, unsigned int dim) {
	unsigned char min, max, most, less;

	print_input(array, dim);

	min = min_char(array, dim);
	max = max_char(array, dim);
	most = most_frequent_char(array, dim);
	less = less_frequent_char(array, dim);

	print_result(min, max, most, less);
}

void print_result (unsigned char min_char, unsigned char max_char, unsigned char most_frequent, unsigned char less_frequent) {
	printf ("\t Minimun char: %d (%c)\n", min_char, min_char);
	printf ("\t Maximum char: %d (%c)\n", max_char, max_char);
	printf ("\t Most frequent char: %c\n", most_frequent);
	printf ("\t Less frequent char: %c\n", less_frequent);
}

unsigned char min_char (unsigned char * array, unsigned int dim) {
	unsigned char min;
	min  =  array[0];

	for (int i = 1; i < dim; i++) {
		if (array[i] < min)
			min = array[i];
	}

	return min;
}

unsigned char max_char (unsigned char * array, unsigned int dim) {
	unsigned char max;
	max  =  array[0];

	for (int i = 1; i < dim; i++) {
		if (array[i] > max)
			max = array[i];
	}

	return max;
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

unsigned char * only_unrepeated_char (unsigned char * array, unsigned int dim) {
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
		if ((src == dest[i]) && ((unsigned int) src != 0))
			return 0;
	}

	return 1;
}
