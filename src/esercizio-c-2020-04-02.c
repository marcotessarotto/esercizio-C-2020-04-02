//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


// specifiche:
// https://github.com/marcotessarotto/esercizio-C-2020-04-02/blob/master/specifiche.txt

#define NUM_CHARS 80

char char_array[NUM_CHARS];

void fork_me();
void perform_analysis();

int main(int argc, char *argv[]) {

	int child_process_counter = 0;
	int ch;

	char * ptr;
	char * end_of_array;

	end_of_array = &char_array[NUM_CHARS-1];

	ptr = char_array; // equivalente a: ptr = &char_array[0];
	// per chi ha dubbi sui puntatori, vedere questo esempio:
	// https://repl.it/@MarcoTessarotto/esempio-calloc-realloc-free

	while ((ch = getchar()) != EOF) {
		*ptr = ch;

		if (ptr == end_of_array) {
			fork_me();

#ifdef DEBUG
			sleep(1);
#endif

			child_process_counter++;

			ptr = char_array;

			memset(char_array, 0, NUM_CHARS * sizeof(char));
		} else {
		    ptr++;
		}


	}

	printf("ho eseguito %d processi figli.\nbye!\n", child_process_counter);

	return 0;
}

void fork_me() {

	switch (fork()) {
		case 0: // processo figlio
			perform_analysis();
			break;
		case -1: // errore!
			perror("fork()");

			exit(EXIT_FAILURE);

			break;
		default:
			//
			break;
	}

}

void perform_analysis() {

	int min_char = 256;
	int max_char = -1;
	int ch;

	char most_frequent_char;
	char less_frequent_char;
	int most_frequent_char_frequency = -1;
	int less_frequent_char_frequency = 81;

	char char_frequency[256] = { 0 };

	for (int i = 0; i < NUM_CHARS; i++) {

		// ch è il carattere che sto esaminando
		ch = (int)char_array[i] & 0xFF; // rendo positivo il carattere (char è 'signed' in gcc)

		if (ch < min_char) {
			min_char = ch; // nuovo carattere minimo
		}

		if (ch > max_char) {
			max_char = ch; // nuovo carattere massimo
		}

		char_frequency[ch]++; // incremento la frequenza del carattere ch

//		if (char_frequency[ch] > most_frequent_char_frequency) {
//			most_frequent_char_frequency = char_frequency[ch];
//			most_frequent_char = ch;
//		}
	}

	for (int i = 0; i < 256; i++) {
		if (char_frequency[i] > most_frequent_char_frequency) {
			most_frequent_char_frequency = char_frequency[i];
			most_frequent_char = i;
		}

		if (char_frequency[i] < less_frequent_char_frequency) {
			less_frequent_char_frequency = char_frequency[i];
			less_frequent_char = i;
		}
	}

	printf("\nchild process %u\n", getpid());

#ifdef DEBUG
	for (int i = 0; i < 256; i++) {
		printf("f[%d]=%d ", i, char_frequency[i]);
		if ((i+1) % 16 == 0)
			printf("\n");
	}
	printf("\n");
#endif

	printf("carattere minimo: %u\n",min_char);
	printf("carattere massimo: %u\n",max_char);
	printf("carattere con più occorrenze: %u freq=%d\n",most_frequent_char, most_frequent_char_frequency);
	printf("carattere con meno occorrenze: %u freq=%d\n",less_frequent_char, less_frequent_char_frequency);

	exit(EXIT_SUCCESS);
}


