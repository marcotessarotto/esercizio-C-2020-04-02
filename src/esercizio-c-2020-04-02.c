#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define NUM_CHARS 8

char Max(char *char_array, int size);
char Min(char *char_array, int size);
char mostFrequent(char *arr, int size);
char lessFrequent(char *arr, int size);
void bubble_sort(char * array, int array_dimension);
void swap_int(char *x, char *y);


int main(int argc, char** argv) {
    int c, max, min,most,less;
    char *char_array;
    int i = 0;
    int child_counter = 0;

    char_array = calloc(NUM_CHARS, sizeof (char));
    if (char_array == NULL) {
        perror("calloc error!");
        exit(EXIT_FAILURE);
    }
    printf("Inserire dei caratteri e terminare con EOF: ");
    while ((c = getchar()) != EOF) {
        if (i == NUM_CHARS) {
            switch (fork()) {
                case 0:// Processo figlio
                    child_counter++;
                    max = Max(char_array, NUM_CHARS);
					printf("\nProcesso figlio %d\n",child_counter);
                    printf("Il Carattere  Massimo è: (%c) codice (%d)\n", max, max);
                    min = Min(char_array, NUM_CHARS);
                    printf("Il Carattere  Minimo è: (%c) codice (%d)\n", min, min);
                    most=mostFrequent(char_array,NUM_CHARS);
                    printf("Il Carattere  più frequente è: (%c) codice (%d)\n", most , most);
                    less=lessFrequent(char_array,NUM_CHARS);
                    printf("Il Carattere  meno frequente è: (%c) codice (%d)\n", less , less);
                    printf("---------------------------------\n");
                    exit(0);
                    break;
                case -1:
                    printf("fork() ha fallito! niente processo figlio!\n");
                    exit(1);
                    break;
                default:
					child_counter++;
                    if (wait(NULL) == -1) {
                        perror("wait error");
                    } else {
                        memset(char_array, 0, NUM_CHARS * sizeof (char));
                        i = 0;
                    }
                    break;
            }
        }else{
					if(c > 31 && c < 127){
            char_array[i] = c;
						i++;
					}
        }
    }
    printf("Sono stati lanciati %d processi figli", child_counter);
    return (EXIT_SUCCESS);
}

char Max(char *char_array, int size) {
    char max = 0;
    for (int i = 0; i < size; i++) {
        if (char_array[i] > max) {
            max = char_array[i];
        }
    }
    return max;
}

char Min(char *char_array, int size) {
    char min = char_array[0];
    for (int i = 1; i < size; i++) {
        if (char_array[i] < min) {
            min = char_array[i];
        }
    }
    return min;
}


char mostFrequent(char *arr, int size){
	bubble_sort(arr,size);

    // find the max frequency using linear traversal
    int max_count = 1;
    char res = arr[0];
    int curr_count = 1;

    for (int i = 1; i < size; i++) {
        if (arr[i] == arr[i - 1])
            curr_count++;
        else {
            if (curr_count > max_count) {
                max_count = curr_count;
                res = arr[i - 1];
            }
            curr_count = 1;
        }
    }

    // If last element is most frequent
    if (curr_count > max_count)
    {
        max_count = curr_count;
        res = arr[size - 1];
    }

    return res;
}

char lessFrequent(char *arr, int size){
	bubble_sort(arr,size);

    // find the max frequency using linear traversal
    int min_count = 100;
    char res = arr[0];
    int curr_count = 1;

    for (int i = 1; i < size; i++) {
        if (arr[i] == arr[i - 1])
            curr_count++;
        else {
            if (curr_count < min_count) {
                min_count = curr_count;
                res = arr[i - 1];
            }
            curr_count = 1;
        }
    }

    // If last element is most frequent
    if (curr_count < min_count)
    {
        min_count = curr_count;
        res = arr[size - 1];
    }

    return res;
}

void swap_int(char *x, char *y) {
	char t = *x;
	*x = *y;
	*y = t;
}

void bubble_sort(char * array, int array_dimension) {
	int n, newn;
	n = array_dimension;
	while (n > 1) {
		newn = 0;
		for (int i = 1; i < n; i++) {
			if (array[i-1] > array[i]) {
				swap_int(&array[i-1], &array[i]);
				newn = i;
			}
		}

		n = newn;
	}
}


