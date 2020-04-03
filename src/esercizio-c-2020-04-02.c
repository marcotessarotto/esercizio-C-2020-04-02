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

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <limits.h>

#define NUM_CHARS 80

char find_min_char(char* x)
{
	char min;
	min=x[0];

	for(int i=0; i<NUM_CHARS; i++)
	{
		if(x[i]<min)
			min=x[i];
	}

	return min;
}
char find_max_char(char* x)
{
	char max=0;

	for(int i=0; i<NUM_CHARS; i++)
	{
		if(x[i]>max)
			max=x[i];
	}

	return max;
}

char find_max_char_occurrences(char* x)
{
	char* y;
	y = calloc(NUM_CHARS, sizeof(char));

	int a=0, m=0;

	for(int i=0; i<NUM_CHARS; i++)
	{
		for(int j=0; j<NUM_CHARS; j++)
			{
				if(x[i]==x[j])
					a++;
			}

		y[i]=a;
		if(a>m)
			m=a;
		a=0;
	}

	for(int i=0; i<NUM_CHARS; i++)
		{
			if(y[i]==m)
			{
				free(y);
				return x[i];
			}
		}

	return 0;
}

char find_min_char_occurrences(char* x)
{
	char* y;
	y = calloc(NUM_CHARS, sizeof(char));


	int a=0, m=INT_MAX;

	for(int i=0; i<NUM_CHARS; i++)
	{
		for(int j=0; j<NUM_CHARS; j++)
			{
				if(x[i]==x[j])
					a++;
			}

		y[i]=a;
		if(a<m)
			m=a;
		a=0;
	}

	for(int i=0; i<NUM_CHARS; i++)
		{
				if(y[i]==m)
				{
					free(y);
					return x[i];
				}
		}

	return 0;

}


int main(int argc, char *argv[]) {

	int c;
	int min_char, max_char, max_char_occurences, min_char_occurences;
	int i=0,counter=0;

	char * char_array = calloc(NUM_CHARS, sizeof(char));

	if (char_array == NULL) {
			perror("calloc error!");
			exit(EXIT_FAILURE);
		}


	while ((c = getchar()) != EOF)
	{

		if(c==10)
		continue;


		char_array[i]=c;
		i++;



		if(i==NUM_CHARS)
		{
			pid_t child_pid;
			child_pid = fork();
			switch(child_pid)
			{
				case 0:
					min_char = find_min_char(char_array);
					max_char = find_max_char(char_array);
					max_char_occurences = find_max_char_occurrences(char_array);
					min_char_occurences = find_min_char_occurrences(char_array);
					printf("\n[Child] Carattere minimo: %c\n", min_char);
					printf("[Child] Carattere massimo: %c\n", max_char);
					printf("[Child] Carattere con meno occorrenze: %c\n", min_char_occurences);
					printf("[Child] Carattere con più occorrenze: %c\n", max_char_occurences);


					printf("[Child] Ora termino...\n\n");
					free(char_array);
					exit(EXIT_SUCCESS);
					break;

				case -1:
					perror("fork()");
					exit(EXIT_FAILURE);
					break;

				default:
					counter++;
					child_pid = wait(NULL);

					if (child_pid == -1)
					{
							perror("wait() error");
							exit(EXIT_FAILURE);
					}
					i= 0;
					memset(char_array, i, NUM_CHARS * sizeof(char));
					break;

			}
		}

	}

	printf("\n\n[Parent] Ho eseguito %u processi figli\n", counter);
	printf("\n\n[Parent] Ora termino...\n");
	free(char_array);
	return 0;
}

