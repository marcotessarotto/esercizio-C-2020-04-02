#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_CHARS 80


char trova_minimo(char * arr, int arr_size){
	char min = arr[0];
	for(int i= 1; i < arr_size; i++ ){
		if((arr[i] < min) && (arr[i] >= 32)){
			min = arr[i];
		}
	}
	return min;
}


char trova_massimo(char * arr, int arr_size){
	char max = arr[0];
		for(int i = 1; i < arr_size; i++){
			if((arr[i] > max) && (arr[i] >= 32)){
				max = arr[i];
			}
		}
		return max;
	}

char trova_minimo_int(int * arr, int arr_size){
	int min = 10000;  //numero esageratamente grande
	for(int i = 0; i < arr_size; i++ ){
		if((arr[i] < min) && (arr[i]>0)){
			min = arr[i];
		}
	}
	return min;
}

char trova_massimo_int(int * arr, int arr_size){
	int max = arr[0];
		for(int i = 1; i < arr_size; i++){
			if(arr[i] > max){
				max = arr[i];
			}
		}
		return max;
	}

char trova_piu_occ( char * arr, int arr_size){
	int * occ = calloc(95, sizeof(int));
	int char_num;
	for(unsigned int i = 0; i < NUM_CHARS; i++){
		char_num= (int) arr[i];
		occ[char_num - 32]++;
	}
	int max_occ = trova_massimo_int(occ, 95);
	for(unsigned int i = 0; i < 95; i++){
		if (occ[i] == max_occ){
			char res = (char)i + 32;
			free(occ);
			return res;
		}
	}

	// che return mettere???
}


char trova_meno_occ( char * arr, int arr_size){
	int * occ = calloc(95, sizeof(int));
	int char_num;
	for(int i = 0; i < NUM_CHARS; i++){
		char_num= (int) arr[i];
		occ[char_num - 32]++;
	}
	int min_occ = trova_minimo_int(occ, 95);
	for(unsigned int i = 0; i < 95; i++){
			if (occ[i] == min_occ){
				char res = (char) i + 32;
				free(occ);
				return res;
			}
		}
	//che return mettere??
}


int main(int argc, char *argv[]){

	char c;
	pid_t child_pid;
	int conta_figli=0;
	char char_array[NUM_CHARS];

	c = getchar();

	while(c != EOF){
		char_array[0] = c;
		for(int i =1; i<80; i++){
			c = getchar();
			char_array[i] = c;
			}

	child_pid = fork();

	if (child_pid == 0){
		pid_t miopid = getpid();
		char max = trova_massimo(char_array, 80);
		char min = trova_minimo(char_array, 80);
		char piuocc = trova_piu_occ(char_array, 80);
		char menoocc = trova_meno_occ(char_array, 80);

		printf("[Figlio %i]\nIl carattere massimo è '%c'\n",miopid, max);
		printf("Il carattere minimo è '%c'\n", min);
		printf("Il carattere con più occorrenze è '%c'\n", piuocc);
		printf("Il carattere com meno occorrenze è '%c'\n", menoocc);


		exit(EXIT_SUCCESS);

	}

	else if (child_pid>0){

		pid_t pid_figlio = wait(NULL);

		printf("[PADRE] Il processo figlio %i è terminato, passiamo al prossimo array\n", pid_figlio);

		for(int i = 0; i< NUM_CHARS; i++){
			char_array[i] = 0;
		}

		conta_figli++;

		c = getchar();
	}

	else {
		perror(" fork error ");
		exit(EXIT_FAILURE);
	}

	}

	printf("Ho contato %d processi figli\n", conta_figli);

	return 0;

}
