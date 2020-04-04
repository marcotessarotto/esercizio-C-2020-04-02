	/*
	il programma svolge le attività sotto descritte fino a quando incontra EOF su stdin.

	1: il programma legge caratteri da stdin.
	Quando incontra EOF, il programma scrive quanti processi figli ha lanciato in tutto e poi termina.

	Il programma accumula i caratteri letti da stdin in un array di dimensione 80 chiamato char_array.
	(introdurre nel codice un #define NUM_CHARS 80).

	Quando char_array è pieno, passa tutto l'array ad un nuovo processo figlio
	che fa delle analisi che scrive su stdout (analisi sotto riportate).

	Dopo aver lanciato il processo figlio, il processo principale azzera char_array ed il programma continua da 1:

	analisi svolte dai processi figlio (sull'array di 80 caratteri ricevuto)*
	carattere minimo
	carattere massimo
	carattere con più occorrenze
	carattere con meno occorrenze

	fatta l'analisi e scritto i risultati , il processo figlio termina.
 	*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_CHARS 80

void swap (char *a, char *b);
void bubble_sort(char * array, unsigned long size);
void counter(char * array, unsigned int size);
char massimo(char *array, unsigned int size);
char minimo(char *array, unsigned int size);
char more_frequent(char *array, unsigned int size);
char less_frequent(char *array, unsigned int size);
void stampaArray(char * array, unsigned int size);

int main(int argc, char *argv[]){
	unsigned int i=0;
	char ch;
	char *char_array;
	unsigned int child_count=0;

	char_array =calloc(NUM_CHARS, sizeof(char));

	while((ch = getchar()) != EOF){
		if( i < NUM_CHARS){
			char_array[i] = ch;
			i++;
		}
		else{
			child_count++;
			switch( fork() ) {
			case 0:
				printf("Sono il processo figlio numero: %d", child_count);
				printf("\n");
				counter(char_array, NUM_CHARS);
				printf("\n");
				free(char_array);
			exit(0);
			break;

			case -1:
				printf("fork() ha fallito! niente processo figlio!\n");
				exit(1);
				break;
          
			default:
			//	printf("sono il processo padre\n");
				wait(NULL);
				i=0;
				memset(char_array,0, NUM_CHARS*sizeof(char));
				break;
			}

		}
	}
	printf("Il processo padre è terminato\n");
	printf("Il processo padre ha %u figli", child_count);

	free(char_array);

}

void counter(char * array, unsigned int size){
	printf("Caratteri in oridne:\n");
	bubble_sort(array, size);
	stampaArray(array, size);
	printf("\n");
	char max = massimo(array,size);
	printf("Carattere massimo: %c\n", max);
	char min = minimo(array, size);
	printf("Carattere minimo: %c\n", min);
	printf("Occorrenze:\n");
	char more = more_frequent(array, size);
	printf("carattere: %c\n", more);
	char less = less_frequent(array, size);
	printf("carattere: %c\n", less);

}


char massimo(char *array, unsigned int size){
	char max = array[0];
	for(int i=0; i<size; i++){
		if(array[i] > max){
			max = array[i];
		}
	}
	return max;
}

char minimo(char *array, unsigned int size){
	char min = array[0];
	for(int i=0; i<size; i++){
		if(array[i] < min){
			min = array[i];
		}
	}
	return min;
}

char more_frequent(char *array, unsigned int size){
	unsigned int more = 1;
	unsigned int count = 1;
	char res = array[0];

	for(int i=0; i<size; i++){
		if( array[i] == array[i+1] ){
			count++;
		}
		else{
			if( count > more ){
				more = count;
				res = array[i];
			}
			count = 1;
		}
	}
	printf("numero occorrenze per il carattere più frequente: %u, ", more);
	return res;
}



char less_frequent(char *array, unsigned int size){
	unsigned int less = size;
	unsigned int count = 1;
	char res = array[0];

	for(int i=0; i<size; i++){
		if (array[i] == array[i+1]){
			count++;
		}
		else{
			if(count < less){
				less = count;
				res = array[i];
			}
			count=1;
		}
	}
	printf("numero occorrenze per il carattere meno frequente: %u, ", less);
	return res;
}

void swap (char *a, char *b){
		int t= *a;
		*a = *b;
		*b = t;
	}

void bubble_sort(char * array, unsigned long size){
		int n= size;
		while( n >= 1){
			int newn = 0;
		    for(int i=1; i<n; i++){
			    if( array[i-1] > array[i]){
			    	swap( &array[i-1], &array[i]);
			    	newn = i;
			    }
		    }
		    n = newn;
		}
	}



void stampaArray(char * array, unsigned int size){
	for(int i=0; i<size; i++){
		printf("%c ", array[i]);
	}
}

