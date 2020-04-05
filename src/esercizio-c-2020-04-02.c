//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

#define NUM_CHARS 80


void swap(char *x, char *y) {
	char t = *x;
	*x = *y;
	*y = t;
}

char* most_frequent(char *string,int length){

  	char* result;
  	result=malloc(sizeof(char));
  	int i, len;
  	int max = -1;

  	int freq[256] = {0};
	for(int i = 0; i < length; i++)
	  	{
	  		freq[string[i]]++;
		}

	  	for(int i = 0; i < length; i++)
	  	{
			if(max < freq[string[i]])
			{
				max = freq[string[i]];
				swap(result,&string[i]);
			}
		}

	  	return result;
 }

char* least_frequent(char *string,int length){


	    // find the min frequency using linear traversal
	    int min_count = length+1;
	    char* res;
	    res=malloc(sizeof(char));
	    int curr_count = -1;
	    for (int i = 1; i < length; i++) {
	        if (string[i] == string[i - 1])
	            curr_count++;
	        else {
	            if (curr_count < min_count) {
	                min_count = curr_count;
	                swap(res,&string[i - 1]);
	            }
	            curr_count = 1;
	        }
	    }

	    // If last element is least frequent
	    if (curr_count < min_count)
	    {
	        min_count = curr_count;
	        swap(res,&string[length - 1]);
	    }

	    return res;
 }


int main(int argc, char * argv[]) {
	pid_t pid;
	char* char_array;
	//char* char_array_copy;
	char_array=calloc(NUM_CHARS,sizeof(char));
	//ssize_t no_byte_letti;
	int k=0;
	//int t=0;
	int count=0;

	printf("scrivi caratteri e poi termini il programma con ctrl+d\n");
	while((char_array[k++]= getchar())!=EOF){

		count++;
		pid=fork();

		if(pid>0){

			//printf("\nsono il processo padre\n");
			wait(NULL);
			memset(char_array,0,NUM_CHARS*sizeof(char));

		}
		else if (pid==0){
			char min,max;
			min='a';
		    max='a';
			//printf("\nsono il processo figlio\n");
			/*Quando char_array è pieno, passa tutto l'array ad un nuovo processo figlio
			che fa delle analisi che scrive su stdout (analisi sotto riportate).*/
			if(k>=80){
				if(k>80){
							exit(EXIT_SUCCESS);
						}
				else{
				//MIN MAX CHAR
				//char min,max;
				for(int i=0; i<NUM_CHARS; i++)
						 {
						    if(min>char_array[i])
						    	swap(&min,&char_array[i]);
						 }
				for(int i=0; i<NUM_CHARS; i++){
							if(max<char_array[i])
								swap(&max,&char_array[i]);
						}
				printf("\nmin(char_array)=%c max(char_array)=%c\n",min,max);

				//MOST LEAST FREQ CHAR
				printf("\nleast_freq_char(char_array)=%c\n",*(least_frequent(char_array,NUM_CHARS)));
			    printf("\nmost_freq_char(char_array)=%c\n",*(most_frequent(char_array,NUM_CHARS)));

				}

				exit(EXIT_SUCCESS);
			}else
				exit(EXIT_SUCCESS);
		}
	}
	//	Quando incontra EOF, il programma scrive quanti processi figli ha lanciato in tutto e poi termina.
	printf("\nsono di nuovo il processo padre e ho lanciato in totale %d processi figli\n",count);
	free(char_array);


		return 0;
}
