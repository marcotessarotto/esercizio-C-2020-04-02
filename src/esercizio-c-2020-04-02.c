#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define NUM_CHARS  8

unsigned char get_lowest_char(char * char_array);
unsigned char get_higher_char(char * char_array);
unsigned char get_most_used_char(char * char_array);
unsigned char get_most_unusual_char(char * char_array);

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

				unsigned char lowest_char = get_lowest_char(char_array);
				unsigned char higher_char = get_higher_char(char_array);

				unsigned char most_used_char = get_most_used_char(char_array);
				unsigned char most_unusual_char = get_most_unusual_char(char_array);


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

unsigned char get_lowest_char(char * char_array){
	unsigned char lowest_char = 255;
	for ( int i = 0 ; i < NUM_CHARS ; i++){
		// determinates lowest char
		if (char_array[i] < lowest_char){
			lowest_char = char_array[i];
		}
	}
	return lowest_char;
}

unsigned char get_higher_char(char * char_array){
	unsigned char higher_char = 0;
	for ( int i = 0 ; i < NUM_CHARS ; i++){
		if (char_array[i] > higher_char){
			higher_char = char_array[i];
		}
	}
	return higher_char;
}

unsigned char get_most_used_char(char * char_array){
	unsigned char most_used_char = NULL;

	short int count_null = 0;
	for (int i=0 ; i < NUM_CHARS ; i++){
		if(char_array[i] == NULL){
			count_null++;
		}
	}

	short int repetition_of_the_most_used_char = 0;

	for (int i=0 ; i < NUM_CHARS ; i++){
		short int repetitions = 1;
		for (int j = i+1 ; j < NUM_CHARS ; j++){
			if (char_array[i] == char_array[j]){
				repetitions++;
			}
		}
		if (repetitions > repetition_of_the_most_used_char){
			repetition_of_the_most_used_char = repetitions;
			most_used_char = char_array[i];
		}
	}

	if(repetition_of_the_most_used_char > count_null){
		return most_used_char;
	}else{
		return 0;
	}
}

unsigned char get_most_unusual_char(char * char_array){
	unsigned char most_unusual_char = char_array[0];

	short int count_null = 0;
	for (int i=0 ; i < NUM_CHARS ; i++){
		if(char_array[i] == NULL){
			count_null++;
		}
	}

	short int repetition_of_the_most_unusual_char = NUM_CHARS;

	for (int i=0 ; i < NUM_CHARS ; i++){
		if (char_array[i] != NULL){
			short int repetitions = 1;
			for (int j = i+1 ; j < NUM_CHARS ; j++){
				if (char_array[i] == char_array[j]){
					repetitions++;
					char_array[j] = NULL;
				}
			}
			if (repetitions < repetition_of_the_most_unusual_char){
				repetition_of_the_most_unusual_char = repetitions;
				most_unusual_char = char_array[i];
				//printf("most_unusual_char: %c\n",most_unusual_char);
				//printf("repetition_of_the_most_unusual_char: %hi\n",repetition_of_the_most_unusual_char);
			}
		}
	}

	if(count_null != 0 && count_null < repetition_of_the_most_unusual_char ){
		return 0;
	}else{
		return most_unusual_char;
	}
}
