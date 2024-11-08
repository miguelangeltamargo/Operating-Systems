#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 4
#define FILE_NAME 'max.txt'

/* The available amount of each resource */
int available[NUMBER_OF_RESOURCES];

/* The maximum demand of each customer */
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

/* The amount currently allocated to each customer */
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

/* The remaining need of each customer */
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

int main(int argc, char const* argv[]) {

    // Check if both command line arguments are provided
    if (argc != 5) {
        fprintf(stderr, "Please Run The Program Again With Correct Format For Available Resources.\n");
        fprintf(stderr, "Example: %s 10 5 7 8\n", argv[0]);
        return 1;
    }


    int starting_resources[10];
    char cmd[20]; // buffer overloads unaccounted

    printf("Welcome to the Banker's Algorithm Simulation\n\n");
    printf("Initializing system with resources:\n");
    printf("Available: [");
    for (int i = 1; i < argc; i++) {
        if (i == argc - 1) {
            printf("%s]\n", argv[i]);
        } else {
            printf("%s, ", argv[i]);
        }
    }


    FILE *file;

    file = fopen(FILE_NAME, "r");


    while (1) {

        printf("Enter a command: ");

        //RQ Command Function
        if (fgets(cmd, sizeof(cmd), stdin) != NULL) {
            cmd[strcspn(cmd, "\n")] = '\0';

            if (strcmp(cmd, "RQ") == 0) {
                return 1;
            }
            printf("%s\n", cmd);
        }
    }

    // Read arguement lines
    // Read max.txt
    // run a loop for user interactins with commands 'RQ', 'RL', and '*'
    // RQ - request resurces; RL - releases resources; * - outputs values of the data structures
    // Ex; RQ 0 3 1 2 1

    return 0;
}
