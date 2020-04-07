#include <stdlib.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <limits.h>

#define NUM_CHARS 80

//Dato un array di caratteri, qual è il carattere più piccolo??
char carattere_minimo(char * array){
	char result=array[0];
	for(int i=1; i<NUM_CHARS; i++){
		if(array[i]<result){
			result=array[i];
		}
	}
	return result;
}

//Datto un array di caratteri, qual è il carattere più grande??
char carattere_massimo(char * array){
	char result=array[0];
	for(int i=1; i<NUM_CHARS; i++){
		if(array[i]>result){
			result=array[i];
		}
	}
	return result;
}

//Dato un array di interi, in quale posizione si trova la cella con il valore più grande??
unsigned int cella_max(unsigned int * array_int, unsigned int celle){
	unsigned int max=0;
	unsigned int cellamax=0;
	for(int t=0; t<celle; t++){
		if(array_int[t]>max){
			max=array_int[t];
			cellamax=t;
		}
	}
	return cellamax;
}

//Dato un array di interi, in quale posizione si trova la cella con il valore più piccolo??
unsigned int cella_min(unsigned int * array_int, unsigned int celle){
	unsigned int min=UINT_MAX;
	unsigned int cellamin=0;
	for(int t=0; t<celle; t++){
		if((array_int[t]<min)&&(array_int[t]!=0)){
			min=array_int[t];
			cellamin=t;
		}
	}
	return cellamin;
}

//Datto un array di caratteri, quale carattere compare meno volte in questo array??
char occorrenza_min(char * array){
	unsigned int noccorrenze[128]={0};
	//aumento di uno la n-esima cella di noccorrenze se trovo il carattere n in array
	for(int t=0; t<80; t++){
		noccorrenze[(int)array[t]]++;
	}
	return cella_min(noccorrenze, 128);
}

//Datto un array di caratteri, quale carattere compare più volte in questo array??
char occorrenza_max(char * array){
	unsigned int noccorrenze[128]={0};
	//aumento di uno la n-esima cella di noccorrenze se trovo il carattere n in array
	for(int t=0; t<80; t++){
		noccorrenze[(int)array[t]]++;
	}
	return cella_max(noccorrenze, 128);
}

int main(int argc, char *argv[]) {

	int contafigli=0;
	char char_array[NUM_CHARS]={0};
	char ch;
	int counter=0;
	pid_t child_pid;

	while((ch=getchar())!=EOF){
		if(counter<NUM_CHARS){
			char_array[counter]=ch;
			counter++;
		}
		else{
			child_pid=fork();
			if(child_pid==-1){
				//errore creazione figlio
				perror("fork()");
				exit(EXIT_FAILURE);
			}
			else if(child_pid==0){
				//sono il figlio
				contafigli++;
				printf("\nSono il figlio %d: controllo array %d\n", contafigli, contafigli);
				printf("Il carattere più piccolo è: '%c'\n", carattere_minimo(char_array));
				printf("Il carattere più grande è: '%c'\n", carattere_massimo(char_array));
				printf("Il carattere con più occorrenze è: '%c'\n", occorrenza_max(char_array));
				printf("Il carattere con meno occorrenze è: '%c'\n", occorrenza_min(char_array));
				printf("Il figlio %d ha finito il suo lavoro\n", contafigli);
				printf("\n\n");
				exit(EXIT_SUCCESS);
			}
			else{
				//sono il padre
				//aspetto il figlio
				if(wait(NULL)==-1){
					perror("wait error");
					exit(EXIT_FAILURE);
				}
				else{
					//svuoto l'array
					contafigli++;
					for(int i=0; i<NUM_CHARS; i++){
						char_array[i]=0;
					}
					//azzero il contatore
					counter=0;
					//ora posso riprendere a leggere char
				}
			}
		}
	}

	//arrivo qua quando incontro EOF
	printf("\nHo finito il mio lavoro: ho creato %d figli\n", contafigli);
	exit(EXIT_SUCCESS);
}
