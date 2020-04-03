#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/types.h>
#include <string.h>

#define NUM_CHARS 80
#define ASCII_SIZE 256

/*
 * il programma svolge le attività sotto descritte fino a quando incontra EOF su stdin
 *
 * 1: il programma legge caratteri da stdin
 * Quando incontra EOF, il programma scrive quanti processi figli ha lanciato in tutto e poi termina
 *
 * Il programma accumula i caratteri letti da stdin in un array di dimensione 80 chiamato char_array
 * (introdurre nel codice un #define NUM_CHARS 80)
 *
 * Quando char_array è pieno, passa tutto l'array ad un nuovo processo figlio
 * che fa delle analisi che scrive su stdout (analisi sotto riportate)
 *
 * Dopo aver lanciato il processo figlio, il processo principale azzera char_array ed il programma continua da 1:
 *
 * *analisi svolte dai processi figlio (sull'array di 80 caratteri ricevuto)*
 * carattere minimo
 * carattere massimo
 * carattere con più occorrenze
 * carattere con meno occorrenze
 *
 * fatta l'analisi e scritto i risultati, il processo figlio termina.
 *
 */

int find_minimum_char(int* A)
{
	int min = A[0];
	for(int i=1;i<NUM_CHARS;i++)
	{
		if(A[i] < min) min = A[i];
	}
	return min;
}

int find_maximum_char(int* A)
{
	int max = A[0];
	for(int i=1;i<NUM_CHARS;i++)
	{
		if(A[i] > max) max = A[i];
	}
	return max;
}

int find_max_occur(int* A)
{
	int count[ASCII_SIZE] = {0};
	int max = 0;
	int result;

	for(int i=0;i<NUM_CHARS;i++)
	{
		count[A[i]]++;
		if (max < count[A[i]])
		{
			max = count[A[i]];
			result = A[i];
		}
	}
	return result;
}

int find_min_occur(int* A)
{
	int count[ASCII_SIZE] = {0};
	int min = 0;

	for(int i=0;i<NUM_CHARS;i++)
	{
		count[A[i]]++;
	}
	for(int i=0;i<ASCII_SIZE;i++)
	{
		if(count[i] != 0)
		{
			if(count[min] == 0 || count[i] < count[min])
			{
				min = i;
			}
		}
	}
	return min;
}

int main(int argc, char** argv)
{
	unsigned int child_proc_counter = 0;
	unsigned int chars_letti_da_stdin = 0;
	int c_from_stdin;
	int* char_array;

	char_array = calloc(NUM_CHARS, sizeof(int));
	if(char_array == NULL)
	{
		perror("calloc");
		exit(EXIT_FAILURE);
	}

	int minimum_char, maximum_char, max_occur_char, min_occur_char;

	while((c_from_stdin = getchar()) != EOF)
	{
		// Posso eliminare il carattere 'line feed' per un puro fattore estetico
		// if(c_from_stdin == 10) continue;

		char_array[chars_letti_da_stdin++] = c_from_stdin;

		if (chars_letti_da_stdin == NUM_CHARS)
		{
			pid_t child_pid;

			switch(fork())
			{
			case 0:
				// Child
				for(int i=0;i<NUM_CHARS;i++)
				{
					printf("%c",char_array[i]);
				}
				printf("\n");

				minimum_char = find_minimum_char(char_array);
				maximum_char = find_maximum_char(char_array);
				max_occur_char = find_max_occur(char_array);
				min_occur_char = find_min_occur(char_array);

				printf("[Child] Carattere minimo: %c\n", minimum_char);
				printf("[Child] Carattere massimo: %c\n", maximum_char);
				printf("[Child] Carattere con più occorrenze: %c\n", max_occur_char);
				printf("[Child] Carattere con meno occorrenze: %c\n", min_occur_char);

				printf("[Child] Ora termino...\n\n");
				free(char_array);
				exit(EXIT_SUCCESS);
				break;

			case -1:
				perror("fork()");
				exit(EXIT_FAILURE);
				break;

			default:
				// Parent
				child_proc_counter++;

				child_pid = wait(NULL);
				if (child_pid == -1)
				{
					perror("wait()");
					exit(EXIT_FAILURE);
				}

				// Reset
				chars_letti_da_stdin = 0;
				memset(char_array, 0, NUM_CHARS * sizeof(int));
				break;
			}
		}
	}

	free(char_array);

	printf("[Parent] Ho lanciato %u processi figli\n", child_proc_counter);
	printf("[Parent] Ora termino...\n");

	return 0;
}
