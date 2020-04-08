#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_CHARS	80
#define ALL_CHARS	256

int read_from_stdin(char* dest, int start)
{
	puts("Waiting for input...");
	char c;
	int i = 0;
	while (i < NUM_CHARS - start) {
		c = getchar();
		if (c == EOF)
			return -1;
		if (c == '\n')
			break;

		dest[start + i] = c;
		i++;
	}
	return i;
}

char get_min(char* arr)
{
	char min = arr[0];
	for (int i = 1; i < NUM_CHARS; i++) {
		if (arr[i] < min)
			min = arr[i];
	}

	return min;
}

char get_max(char* arr)
{
	char max = arr[0];
	for (int i = 1; i < NUM_CHARS; i++) {
		if (arr[i] > max)
			max = arr[i];
	}

	return max;
}

char most_frequent(char* arr)
{
	char occurrences[ALL_CHARS] = {0};
	unsigned char most_freq_pos = 0;

	for (int i = 0; i < NUM_CHARS; i++)
		occurrences[(unsigned) arr[i]]++;

	for (int i = 1; i < ALL_CHARS; i++) {
		if (occurrences[i] > occurrences[most_freq_pos])
			most_freq_pos = i;
	}

	return most_freq_pos;
}

char least_frequent(char* arr)
{
	char occurrences[ALL_CHARS] = {0};
	unsigned char least_freq_pos = arr[0];

	for (unsigned int i = 0; i < NUM_CHARS; i++)
		occurrences[(unsigned) arr[i]]++;

	for (int i = 0; i < ALL_CHARS; i++) {
		if (occurrences[i] == 0)
			continue;
		if (occurrences[i] < occurrences[least_freq_pos])
			least_freq_pos = i;
	}

	return least_freq_pos;
}

int main (int argc, char* argv[])
{
	char char_array[NUM_CHARS] = {0};
	int read, count = 0, totalread = 0;

	while ((read = read_from_stdin(&char_array[0], totalread)) >= 0) {
		totalread += read;
		if (totalread < NUM_CHARS)
			continue;

		totalread = 0;
		int ws;

		pid_t child_pid = fork();

		switch (child_pid) {
		case 0:
			printf("Started child process %d...\n", getpid());
			printf("Max char is: %c\n", get_max(&char_array[0]));
			printf("Min char is: %c\n", get_min(&char_array[0]));
			printf("Most frequent char is: %c\n", most_frequent(&char_array[0]));
			printf("Least frequent char is: %c\n", least_frequent(&char_array[0]));
			exit(EXIT_SUCCESS);

		case -1:
			perror("fork() failed. Quitting now.");
			exit(EXIT_FAILURE);

		default:
			printf("Parent process %d going into wait mode...\n", getpid());
			if (wait(&ws) == -1)
				perror("wait() failed");
			puts(WIFEXITED(ws) ? "Child has exited correctly" : "Child has not exited correctly");
		}
		count++;
		memset(char_array, 0, NUM_CHARS);
	}

	printf("Spawned %d child(s)\n", count);

	return 0;
}
