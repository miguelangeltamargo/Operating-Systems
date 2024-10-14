// Name: Miguelangel Tamargo
// Panther ID: 5866999
// ################################################################################################
// Assignment 1: Parallel Processing
// This program creates user speciufied number of process between 1 and 4.
// It then computes 4 different CPU bounf functions depending on the number of child processes.
// ################################################################################################
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>

// This function computes the factorial of a given number using recursive approach.
unsigned long long find_factorial(int num) {
	if (num == 0) {
		return 1;
	} else {
		return (num * find_factorial(num - 1));
	}
};

// A wrapper function to assist with printing process id and calls the factorial function.
unsigned long long process_factorial(int num) {
	printf(", is finding factorial of %d.\n", num);
	return find_factorial(num);
}

// An implementation of Newtons method for deriving the square root of a number.
double square_root(double num) {
	if (num < 0) {
		return -1; // Error check of negative input
	}
	if (num == 0 || num == 1){ // Square root of 0 or 1 returns itself
		return num;
	}

	double margin = 0.000001; // Precision used for approximation to 0
	double f_x = num / 2; // starting guess
	double new_x = ((f_x + (num/f_x))/2); // new guess using newtons method

	while (1) { // Iterate untill the margin is reached
		if (fabs(f_x - new_x) < margin) {
			return new_x;
		}
		f_x = new_x;
		new_x = ((f_x + (num/f_x))/2);
	}
}

// A wrapper function to assist with printing process id and calls the square_root function.
double process_sqrt(double num){
	printf(", is finding the square root of %d.\n", (int)*(&num));
	return square_root(num);
}

// This function finds the prime numbers in the range of a given number.
// Using a int pointer and 2 arguments to pass in the given number and a pointer to a list count
int* prime_in_range(int num, int *prime_count) {
	printf(", is finding prime numbers in range of %d.\n", num);

	// Allocates space for the prime numbers and malloc error handling.
	int threshold = 10;
	int *prime_list = malloc(threshold * sizeof(int));
	if (prime_list == NULL) {
		perror("Malloc Error");
		exit(EXIT_FAILURE);
	}
	int list_size = 0;
	int pass_flag;
	// Iterate all numbers between 2 and given number
	// checking numbers  for divisibility from 2 to the square root of each number.
	for (int i = 2; i<=num; i++) {
		int x = square_root(i);
		pass_flag = 1;

		for (int j = 2; j<=x; j++) {
			if (i%j == 0) { //is divisible, so the number is not prime and flag is raise
				pass_flag = 0;
				break;
			}
		}
		if (pass_flag==1) {
			if (list_size >= threshold) { // reallocating size when prime lists size outgrows initial size
				threshold *=2;
				prime_list = realloc(prime_list, threshold * sizeof(int));
				if (prime_list == NULL) {
					perror("Malloc Error");
					exit(EXIT_FAILURE);
				}
			}
			prime_list[list_size] = i;
			list_size++;
		}
	}

	*prime_count = list_size; //sets the count variable to size of the prime list
	return prime_list;
};

// Function used for comparuing characters ascii value.
int compare_char(const void *a, const void *b){
	return (*(char *)a-*(char *)b);
}

// This function retrieves the current working directopry of the user and sorts the path as a string.
// And returns a pointer to a memory address of the stored sorted path.
char* sort_cwd(){
	// Allocates space for the users path size and malloc error handling.
	char* path = malloc(512 * sizeof(char));
	if (path == NULL) {
		perror("Malloc Error");
		exit(EXIT_FAILURE);
	}
	getcwd(path, 512); // function used to retrieve users current working directory
	size_t len = strlen(path);
	printf(", is sorting your current working directory: %s\n", path);

   	qsort(path, len, sizeof(char), compare_char); // Quicksort function to sort the path string
    return path;
}

int main() {
	int MAX_CHILD;
	// Starts loop to prompt users for amount of child processes between 1-4
	// usses error handling and input buffer clearing to account for incorrect input
	while (1) {
		printf("Enter your child process limit?\n");
		if (scanf("%d", &MAX_CHILD) != 1 || MAX_CHILD < 1 || MAX_CHILD > 4) {
    		fprintf(stderr, "Error: Enter a number between 1 and 4.\n");
     		while (getchar() != '\n'); // buffer clearing, getchar will discard all chars till /n is reached
		} else {
			break;
		}
	}
	//FOking child processes to assign to different tasks.
	pid_t pid;
	printf("Parent process (%d) is creating %d child processe(s).\n", getpid(), MAX_CHILD);
	for (int i = 0; i< MAX_CHILD; i++){
		pid = fork();
		if (pid < 0) { // forksystem call error handling
			perror("Failed Fork!");
			exit(EXIT_FAILURE);
		} else if (pid == 0) {
			srand(time(NULL)); // seeding random function to differentiate sequence for each child
			switch (i) {
				case 0: // Task instructions for child process calculating factorial
				   	printf("Child %d (%d)", i+1, getpid());
				    unsigned long long factorial = process_factorial(rand() % 15);
				    printf("Child %d (%d) completed factorial. Result: %llu\n", i + 1, getpid(), factorial);
				    break;
				case 1: // Task instructions for child process calculationg square root
				   	printf("Child %d (%d)", i+1, getpid());
				    double square_root = process_sqrt(rand() % 10000);
				    printf("Child %d (%d) completed square root. Result: %f\n", i + 1, getpid(), square_root);
				    break;
				case 2: // Task instructions for calculating primes in range of a number
				   	printf("Child %d (%d)", i+1, getpid());
					int list_size;
				    int *prime_list = prime_in_range(rand() % 200, &list_size);
				    printf("Child %d (%d) completed primes in range. Result: ", i + 1, getpid());
					for (int x = 0; x < list_size; x++) {
						printf("%d ", prime_list[x]);
					}
					printf("\n");
					free(prime_list); // free alocated memory for prime list
				    break;
				case 3: // Task instructions for sorting the current working directory path
				   	printf("Child %d (%d)", i+1, getpid());
				    char *sorted_path = sort_cwd();
				    printf("Child %d (%d) completed directory sort. Result: %s\n", i + 1, getpid(), sorted_path);
				    free(sorted_path); // free allocated memory for sorted path
				    break;
				}
        	exit(0); // Exit call for child process when task gets completed
    	}
	}
	// Parent process waiting for each child process to finish.
	for (int i = 0; i < MAX_CHILD; i++) {
		wait(NULL);
	}
	printf("All child processes have completed. Parent (%d) is displaying the final message.\n", getpid());

  return 0;
}
