#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

/*
 * Il programma invoca fork ogni 80 caratteri letti, se ce sono di meno oppure
 * non vengono scritti caretteri in numero multiplo di 80, gli ultimi vengono ignorati.
 */

#define NUM_CHARS 80

void swap_int(char *x, char *y);
void bubble_sort(char * array, int array_dimension);

void conta_caratteri(char char_array[], int size);

int main(int argc, char *argv[]){

	char ch;
	char* char_array;
	int i = 0;
	int num_figli = 0;

	char_array = calloc(NUM_CHARS, sizeof(char));

	ch = getchar();

	while(ch != EOF){

		if(i < NUM_CHARS){
			char_array[i] = ch;
			i++;
			ch = getchar();
		}
		else{
			num_figli++;
			switch(fork()) {
				case 0:
					printf("[figlio %d]", num_figli);
					conta_caratteri(char_array, NUM_CHARS);
					exit(EXIT_SUCCESS);
				case -1:
					perror("fork error");
					exit(EXIT_FAILURE);
				default:
					if(wait(NULL) == -1){
						perror("wait error");
						return(EXIT_FAILURE);
					}

					break;
			}
			i = 0;
			memset(char_array, 0, NUM_CHARS*sizeof(char));
		}
	}
	printf("[padre] Ho generato %d figli\n",num_figli);

	return(EXIT_SUCCESS);
}

void conta_caratteri(char char_array[], int size){

	struct char_occ{
		int oc;
		char ch;
	}char_max_occ, char_min_occ;

	char_max_occ.ch = char_array[0];
	char_max_occ.oc = 1;
	char_min_occ.ch = char_array[0];
	char_min_occ.oc = 1000;

	bubble_sort(char_array, size);

	int n = 0;

	for(int i = 1; i < size; i++){
		n = 1;
		//conta numero di occorrenze
		while(char_array[i] == char_array[i-1]){
			i++;
			n++;
		}
		if(n > char_max_occ.oc){
			char_max_occ.oc = n;
			char_max_occ.ch = char_array[i-1];
		}
		if(n < char_min_occ.oc){
			char_min_occ.oc = n;
			char_min_occ.ch = char_array[i-1];
			//se l'ultimo carattere è diverso dal penultimo compare solo una volta
			if(i == size-1){
				char_min_occ.oc = 1;
				char_min_occ.ch = char_array[i];
			}
		}
	}

	printf("\n\tCarattere massimo: '%c'\n\t Carattere minimo: '%c'\n", char_array[0], char_array[size -1]);
	printf("\tCarattere con più occorrenze: '%c', %dvolte\n", char_max_occ.ch, char_max_occ.oc);
	printf("\tCarattere con meno occorrenze: '%c', %dvolte\n\n", char_min_occ.ch, char_min_occ.oc);

	return;
}


void swap_int(char *x, char *y) {
	int t = *x;
	*x = *y;
	*y = t;
}

void bubble_sort(char * array, int array_dimension){
	int n = array_dimension;
	do{
		int newn = 0;
		for(int i = 1; i < n; i++){
			if(array[i-1] < array[i]){
				swap_int(&array[i-1], &array[i]);
				newn = i;
			}
		}
		n = newn;
	}while(n>=1);

	return;
}


