#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define NUM_CHARS 10



void stampa_array (char * char_array){

	for (int i=0;i< NUM_CHARS;++i)
		printf("%c",char_array[i]);

	printf("\n");
}


char minimo (char * char_array){
	char minimo=char_array[0];
	for(int i=1;i<NUM_CHARS;++i){
		if(char_array[i]<minimo)
			minimo=char_array[i];
	}
	return minimo;
}


char massimo (char * char_array){
	char massimo=char_array[0];
	for(int i=1;i<NUM_CHARS;++i){
		if(char_array[i]>massimo)
			massimo=char_array[i];
	}
	return massimo;
}



char more_frequent (char * char_array){

	int freq_result=0;
	char result;
	int counter;

	for(int i=0; i<NUM_CHARS;++i){

		counter=1;

		for(int j=i; j<NUM_CHARS;++j)
			if(char_array[i]==char_array[j])
				++counter;

		if(counter>freq_result){
			freq_result=counter;
			result=char_array[i];
		}

	}

	return result;
}


char less_frequent (char * char_array){

	int freq_result=10;
	char result;
	int counter;

	for(int i=0; i<NUM_CHARS;++i){

		counter=1;

		for(int j=i; j<NUM_CHARS;++j)
			if(char_array[i]==char_array[j])
				++counter;

		if(counter<freq_result){
			freq_result=counter;
			result=char_array[i];
		}

	}

	return result;
}


int main(int argc, char **argv) {

	char * char_array;
	char_array=calloc(NUM_CHARS,sizeof(char));

	int counter=0;
	char c;
	int i=0;

	while ((c=getchar())!=EOF){

		if(i==NUM_CHARS)

			switch (fork()){

				case 0:
					//stampo array
					printf("[child %d] Stampo array passatomi\t",counter+1);
					stampa_array(char_array);
					//minimo
					printf("\n[child %d] Stampo il minimo: %c\n\n",counter+1,minimo(char_array));
					//massimo
					printf("[child %d] Stampo il massimo: %c\n\n",counter+1,massimo(char_array));
					//più frequente
					printf("[child %d] Stampo il più frequente: %c\n\n",counter+1,more_frequent(char_array));
					//meno frequentE
					printf("[child %d] Stampo il meno frequente: %c\n\n",counter+1,less_frequent(char_array));
					printf("\n\n");
					exit(0);

				case -1:
					printf("errore fork");
					exit(1);

				default:
					wait(NULL);
					memset(char_array,'0',NUM_CHARS*sizeof(char));
					char_array[0]=c;
					i=1;
					counter++;

			}//end switch

		else{

			char_array[i]=c;
			++i;

		}//end if-else

	}//end while
	printf("[parent] Ho creato %d figli",counter);






   return 0;
}
