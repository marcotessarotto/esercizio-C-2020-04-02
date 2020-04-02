//
/*
 **************TESTO ESERCIZIO********************
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

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

#define NUM_CHARS 5

char min_char(char * char_array);
char max_char(char * char_array);
char less_occurrence(char * char_array);
char more_occurrence(char * char_array);

int main(int argc, char * argv[]) {
	char zero_array[NUM_CHARS] = {0};	//array usato in seguito per azzerare char_array
	char char_array[NUM_CHARS] = {0};
	int i = 0;	//indice per char_array
	int n_child = 0;	//numero di processi figli creati

	printf("Inserisci dei caratteri: ");

	while ((char_array[i] = getchar()) != EOF) {
		i++;
		if(i == NUM_CHARS){	//controllo se char_array è pieno
			switch (fork()) {	//genero un nuovo processo figlio
				case 0:
					printf("\nCarattere minimo : %c", min_char(char_array));
					printf("\nCarattere massimo : %c", max_char(char_array));
					printf("\nCarattere con meno occorrenze : %c", less_occurrence(char_array));
					printf("\nCarattere con più occorrenze : %c", more_occurrence(char_array));

					exit(0);
					break;
				case -1:
					printf("fork() ha fallito!\n");

					exit(1);
					break;
				default:
					i = 0;
					memcpy(char_array, zero_array, NUM_CHARS);	//azzeramento di char_array
					n_child++;	//incremento del contatore del numero di figli creati
					printf("sono il processo padre\n");
			}
		}
	}

	printf("\nNumero di processi figli creati = %d", n_child);

	return 0;
}


char min_char(char * char_array){
	char min = CHAR_MAX;

	for(int i = 0; i < NUM_CHARS; i++){
		if(char_array[i] < min)
			min = char_array[i];
	}

	return min;
}

char max_char(char * char_array){
	char max = CHAR_MIN;

	for(int i = 0; i < NUM_CHARS; i++){
		if(char_array[i] > max)
			max = char_array[i];
	}

	return max;
}

char less_occurrence(char * char_array){
	int index;
	int count_min = NUM_CHARS + 1;	//impostazione di un numero fittizio massimo di occorrenze

	for(int i = 0; i < NUM_CHARS; i++){	//scorro tutti i caratteri dell'array per controllarne l'occorrenza
		char temp = char_array[i];
		int count = 0;	//contatore delle occorrenze del carattere corrente
		for(int j = 0; j < NUM_CHARS; j++){
			if(temp == char_array[j])
				count++;
		}
		if(count < count_min){	//controllo se l'occorrenza è la più piccola
			count_min = count;
			index = i;
		}
	}

	return char_array[index];
}

char more_occurrence(char * char_array){
	int index;
	int count_max = -1;	//impostazione di un numero fittizio minimo di occorrenze

	for(int i = 0; i < NUM_CHARS; i++){
		char temp = char_array[i];
		int count = 0;
		for(int j = 0; j < NUM_CHARS; j++){
			if(temp == char_array[j])
				count++;
		}
		if(count > count_max){
			count_max = count;
			index = i;
		}
	}

	return char_array[index];
}
