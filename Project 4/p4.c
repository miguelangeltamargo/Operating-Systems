#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES argc - 1
#define FILE_NAME "max.txt"

int main(int argc, char const* argv[]) {

    /* The available amount of each resource */
    int available[NUMBER_OF_RESOURCES];

    /* The maximum demand of each customer */
    int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

    /* The amount currently allocated to each customer */
    int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

    /* The remaining need of each customer */
    int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];
    // Check if both command line arguments are provided
    if (argc != 5) { // fix this check is hardcoded shittly
        fprintf(
            stderr, "Please Run The Program Again With Correct Format For Available Resources.\n");
        fprintf(stderr, "Example: %s 10 5 7 8\n", argv[0]);
        return 1;
    }

    // printf("%lu\n", sizeof(__LONG_LONG_MAX__));

    // Populating array with int type of availabe resource values from CLA
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        available[i] = atoi(argv[i + 1]);
    }

    printf("\nWelcome to the Banker's Algorithm Simulation\n\n");
    printf("Initializing system with resources:\n");
    printf("Available: [");
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        printf(i != NUMBER_OF_RESOURCES - 1 ? "%d, " : "%d]\n\n", available[i]);
    }

    FILE* file;
    file = fopen(FILE_NAME, "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
    char buffer[512];
    char* token;

    int customer_count, token_count = 0;
    printf("Reading maximum request file...\n");
    printf("Maximum requests initialized:\n");

    while (fgets(buffer, 100, file) != NULL) {
        token = strtok(buffer, ", ");
        while (token != NULL) {
            maximum[customer_count][token_count] = atoi(token);
            need[customer_count][token_count] = atoi(token);
            token_count++;
            token = strtok(NULL, ", ");
        }
        token_count = 0;
        customer_count++;
    }
    fclose(file);

    for (int i = 0; i < customer_count; i++) {
        printf("Customer %d: [", i);
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
            printf(j != NUMBER_OF_RESOURCES - 1 ? "%d, " : "%d]\n", maximum[i][j]);
        }
    }

    char cmd[50]; // buffer overloads unaccounted
    while (1) {
        printf("\nEnter a command: \n");
        printf("- 'RQ customer_num resource_1 resource_2 resource_3 resource_4' to request resources\n");
        printf("- 'RL customer_num resource_1 resource_2 resource_3 resource_4' to release resources\n");
        printf("- '*' to display the current values of the data structures\n");
        printf("- 'exit' to quit\n");
        // RQ Command Function
        if (fgets(cmd, sizeof(cmd), stdin) != NULL) {
            cmd[strcspn(cmd, "\n")] = '\0';
            token = strtok(cmd, " ");
            if (strcmp(token, "RQ") == 0) {
                printf("%s\n", cmd);
                printf("%s\n", token);
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
