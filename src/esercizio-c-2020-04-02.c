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
void bubble_sort(unsigned char * array, unsigned int array_dimension);
void swap_uchar(unsigned char *x, unsigned char *y);

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
					printf("\t [child %d] operation on input: '", child_process_counter);
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
					char_array[char_counter++] = c;
					break;
			}
		} else if (c > 31) {
			char_array[char_counter++] = c;
		}

	}
	printf("\n****************************************\n");
	printf("\t [parent] Terminated:\n");
	printf("\t Number of child process launched: %d", child_process_counter);
	printf("\n****************************************\n");
	free(char_array);

	exit(0);
}

void print_input (unsigned char * array, unsigned int dim) {

	for (int i = 0; i < dim; i++)
		printf("%c", array[i]);

	printf("'\n");
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
	printf ("\t Most frequent char: %c (%d)\n", most_frequent, most_frequent);
	printf ("\t Less frequent char: %c (%d)\n", less_frequent, less_frequent);
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
	bubble_sort(array, dim);

	unsigned int most_frequent_count = 1;
	unsigned int curr_count = 1;
	unsigned char result = array[0];

	for (int i = 1; i < dim; i++) {
		if (array[i] == array[i - 1])
			curr_count++;
		else {
			if (curr_count > most_frequent_count) {
				most_frequent_count = curr_count;
				result = array[i - 1];
			}
			curr_count = 1;
		}
	}

	if (curr_count > most_frequent_count)
	{
		most_frequent_count = curr_count;
		result = array[dim - 1];
	}

	return result;
}

unsigned char less_frequent_char (unsigned char * array, unsigned int dim) {
	bubble_sort(array, dim);

	int less_frequent_count = dim;
	char result = array[0];
	int curr_count = 1;

	for (int i = 1; i < dim; i++) {
		if (array[i] == array[i - 1])
			curr_count++;
		else {
			if (curr_count < less_frequent_count) {
				less_frequent_count = curr_count;
				result = array[i - 1];
			}
			curr_count = 1;
		}
	}

	if (curr_count < less_frequent_count)
	{
		less_frequent_count = curr_count;
		result = array[dim - 1];
	}

	return result;
}

void bubble_sort(unsigned char * array, unsigned int array_dimension) {
	unsigned int dim = array_dimension - 1;

	for (int i = 0; i < dim; i++) {
		for (int j = 0; j < dim - i; j++) {
			if (array[j] > array[j+1])
				swap_uchar(&array[j], &array[j+1]);
		}
	}
}

void swap_uchar(unsigned char *x, unsigned char *y) {
	unsigned char t = *x;
	*x = *y;
	*y = t;
}


