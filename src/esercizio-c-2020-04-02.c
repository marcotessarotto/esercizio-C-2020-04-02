#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_CHARS 80
/**
 * The CPU intensive solution has a n^2 loop hence ~ O(n^2)
 * The MEMORY intensive solution has 2*n loop hence ~ O(n)
 */
#define MEMORY_INTENSIVE_ALGORITHM
#define CHAR_SPACE 256

int worker_analysis(char * array, int len);
void spawn_worker(char * array, int len);

int main(int argc, char **argv) {
	int p_count = 0;
	char last_char;

	char char_array[NUM_CHARS];
	int char_array_size = 0;

	printf("Waiting for your input:\n");

	// test
	/* worker_analysis("aabcdefghi", 10);
	worker_analysis("bcnnnnnnnn", 10);
	worker_analysis("bccnnnnnnn", 10);
	worker_analysis("ccnnnbnnnn", 10); */

	/**
	 * This loop will spawn a process even for the last array (when < 80)
	 * This technically does not follow the specifications but neither violates them,
	 * but it seems more logical to not discard the last chars chunk even if
	 * EOF was reached earlier than filling it up.
	 */
	while (1) {
		last_char = getchar();

		// Spawn worker process
		if (char_array_size == NUM_CHARS || (last_char == EOF && char_array_size > 0)) {
			spawn_worker(char_array, char_array_size);
			p_count++;
			char_array_size = 0;
		}

		if (last_char == EOF) {
			break;
		}

		// Accumulate char
		// we skip \n character only due to confusion while testing from console
		// if (last_char != '\n') {
			char_array[char_array_size++] = last_char;
		// }
	}

	printf("Created %d children", p_count);
	return 0;
}

/**
 * We leverage memory copy of the fork to pass the array to
 * the child process.
 */
void spawn_worker(char * array, int len) {
	pid_t child_pid = fork();
	if (child_pid == 0) {
		// Child process
		exit(worker_analysis(array, len));
	} else if (child_pid > 0) {
		// Parent porcess
		printf("Child started\n");
		if (wait(NULL) == -1) {
			perror("wait()");
		}
		printf("Child finished\n");
	} else {
		// Fail
		perror("fork()");
		exit(-1);
	}
}

int worker_analysis(char * array, int len) {
	// Fail upon no data to process
	if (len <= 0) {
		return -1;
	}

	// Preinit the luggage
	char min = array[0], max = array[0];

	char most_char, least_char;
	unsigned char most_count = 0, least_count = 0;

#ifdef MEMORY_INTENSIVE_ALGORITHM
	unsigned char map[256] = {0};
#endif

	for (int i = 0; i < len; i++) {
		// Min / max
		min = min < array[i] ? min : array[i];
		max = max > array[i] ? max : array[i];


#ifdef MEMORY_INTENSIVE_ALGORITHM
		map[array[i]]++;
#else
		// Most / Least
		if (array[i] != most_char || array[i] != least_char) {
			// todo : can be slightly optimized by a factor of .5
			unsigned char _count = 0;
			for (int j = 0; j < len; j ++) {
				if (array[j] == array[i]) _count ++;
			}

			if (most_count == 0 || _count > most_count){
				most_count = _count;
				most_char = array[i];
			}
			if (least_count == 0 || _count < least_count) {
				least_count = _count;
				least_char = array[i];
			}
		}
#endif

	}

#ifdef MEMORY_INTENSIVE_ALGORITHM
	for (int i = 0; i < CHAR_SPACE; i++) {
		if (most_count == 0 || map[i] > most_count) {
			most_count = map[i];
			most_char = (char)i;
		}
		if (map[i] > 0 && (least_count == 0 || map[i] < least_count)) {
			least_count = map[i];
			least_char = (char)i;
		}
	}
#endif

	printf("Max char is: '%c' (0x%x)\n", max, max);
	printf("Min char is: '%c' (0x%x)\n", min, min);
	printf("Most frequent char is: '%c' (0x%x) that occurs %d times\n", most_char, most_char, most_count);
	printf("Least frequent char is: '%c' (0x%x) that occurs %d times\n", least_char, least_char, least_count);
	return 0;
}
