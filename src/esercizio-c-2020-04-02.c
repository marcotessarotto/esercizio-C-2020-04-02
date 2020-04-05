#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define NUM_CHARS  80

int main(int argc, char *argv[]) {

	short int numbers_of_childs = 0;

	char * char_array;
	char_array = calloc(NUM_CHARS, sizeof(char));
	if (char_array == NULL) {
		perror("calloc error!");
		exit(EXIT_FAILURE);
	}

	printf("[father] Write some chars:\n");

	char c = getchar();
	int counter = 0;

	while (c != EOF) {
		char_array[counter] = c;

		if(counter >= NUM_CHARS -1 ){
			pid_t child_pid;
			child_pid = fork();

			if ( child_pid == 0){
				// child

				unsigned char lowest_char = 255;
				unsigned char higher_char = 0;

				unsigned char most_used_char = char_array[0];
				short int repetition_of_the_most_used_char = 1;
				unsigned char most_unusual_char = char_array[0];
				short int repetitions_of_the_most_unusual_char = NUM_CHARS;

				/*
				printf("[child] print all elements of char_array that I recived:\n");
				for(int i=0 ; i<NUM_CHARS ; i++){
					printf("%d ", char_array[i]);
				}
				printf("\n");
				*/

				for ( int i = 0 ; i < NUM_CHARS ; i++){
					// determinates lowest char
					if (char_array[i] < lowest_char){
						lowest_char = char_array[i];
					}
					// determinates higher char
					if (char_array[i] > higher_char){
						higher_char = char_array[i];
					}
				}

				/*
				 * This block of code is useful only if char_array has some NULL values
				 * that are the most_used_char/most_unusual_char/lowest_char/higher_char
				*/
				short int count_null=0;
				for (int i=0 ; i < NUM_CHARS ; i++){
					if(char_array[i] == NULL){
						count_null++;
					}
				}
				if(count_null >= 1){
					most_used_char = NULL;
					repetition_of_the_most_used_char = count_null;
					most_unusual_char = NULL;
					repetitions_of_the_most_unusual_char = count_null;
				}

				// determinate most unusual char & most usual char
				for (int i=0 ; i < NUM_CHARS ; i++){
					if (char_array[i] != NULL){
						short int repetitions = 1;
						for (int j = i+1 ; j < NUM_CHARS ; j++){
							if (char_array[i] == char_array[j]){
								repetitions++;
								char_array[j] = NULL;
							}
						}
						if (repetitions > repetition_of_the_most_used_char){
							repetition_of_the_most_used_char = repetitions;
							most_used_char = char_array[i];
						}
						if (repetitions < repetitions_of_the_most_unusual_char){
							repetitions_of_the_most_unusual_char = repetitions;
							most_unusual_char = char_array[i];
						}
					}
				}

				printf("\n[child] lowest char: %c\t\t\tin ascii code is: %d\n",lowest_char, lowest_char);
				printf("[child] higher char: %c\t\t\tin ascii code is: %d \n",higher_char, higher_char);
				printf("[child] most_used_char: %c\t\tin ascii code is: %d \n", most_used_char, most_used_char);
				printf("[child] most_unusual_char: %c\t\tin ascii code is: %d \n", most_unusual_char, most_unusual_char);

				exit(0);
			}
			if ( child_pid == -1){
				perror("fork has fail\n");
				exit(1);
			}
			if ( child_pid > 0){
				// father
				counter = -1;
				char_array = calloc(NUM_CHARS, sizeof(char));
				numbers_of_childs++;
				//printf("[parent] waiting for my child process to end\n");
				c = 10;
				if (wait(NULL) == -1) {
					perror("wait error");
				} else {
					//printf("[parent] the child process has ended\n");
				}
			}
		}

		c = getchar();
		counter++;
	}

	printf("\n[father] I threw %d proces\n", numbers_of_childs);

	return 0;
}
