#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_CHARS 80

int main( ) {
    char * char_array;
    char ch;
    int arrayslot = 0;
    int children = 0;

    char_array = calloc(NUM_CHARS, sizeof(char));

    if (char_array == NULL) {
            perror("Could not allocate!");
            exit(EXIT_FAILURE);
    }

    printf("Type in the characters:\n");

    //Dubbio: come tratto il carattere 10 newline? E' l'unico modo per dare al programma una stringa di caratteri.
    //Quindi un input 123456 + enter viene memorizzato nell'array come 1, 2, 3, 4, 5, 6, 10.
    //Se elimino l'enter a fine riga poi non ho modo di inserire il carattere 10 nel programma.

    while ((ch = getchar()) != EOF) {
            if (arrayslot < NUM_CHARS) {
                char_array[arrayslot] = ch;
                arrayslot++;
            } else {
                children++;
                arrayslot = 0;
                if(!fork()) {

                    char min = char_array[0];
                    char max = char_array[0];
                    char most_frequent = -1;
                    char least_frequent = -1;

                    //Trovo massimi e minimi
                    for (int i = 1; i < NUM_CHARS; i++) {
                        if (char_array[i] < min) {
                            min = char_array[i];
                        }
                        if (char_array[i] > max) {
                            max = char_array[i];
                        }
                    }

                    //Trovo la frequenza di ogni singolo elemento di array
                    int max_freq = 0;
                    int min_freq = NUM_CHARS;
                    for (int i = 0; i < NUM_CHARS; i++) {
                        if(char_array[i] != -1) {
                            int curr_freq = 1;

                            for (int j = i + 1; j < NUM_CHARS; j++) {
                                if (char_array[j] == char_array[i]) {
                                    curr_freq++;
                                    char_array[j] = -1;
                                }
                            }

                            //Verifico se e' piu' frequente
                            if (max_freq < curr_freq) {
                                max_freq = curr_freq;
                                most_frequent = char_array[i];
                            }

                            //Verifico se e' meno frequente, se sono di uguale frequenza prendo il piu' grande.
                            if (min_freq >= curr_freq) {
                                min_freq = curr_freq;
                                least_frequent = char_array[i];
                            } else if ((min_freq == curr_freq) && (char_array[i] > least_frequent)){
                                least_frequent = char_array[i];
                            }
                        }
                    }

                    printf("\nMinimum is %c. Maximum is %c.", min, max);
                    printf("\nThe most frequent is %c with a frequency of %d.\nThe least frequent is %c with a frequency of %d.\n\n", most_frequent, max_freq, least_frequent, min_freq);
                    exit(0);
                }
            }
//          memset(char_array, 0, NUM_CHARS * sizeof(char));
    }

    printf("You launched %d child processes", children);

    return 0;

}
