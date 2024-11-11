#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define NUMBER_OF_CUSTOMERS 5
// #define NUMBER_OF_RESOURCES 4

// /* The available amount of each resource */
// int available[NUMBER_OF_RESOURCES];

// /* The maximum demand of each customer */
// int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

// /* The amount currently allocated to each customer */
// int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

// /* The remaining need of each customer */
// int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

int NUMBER_OF_CUSTOMERS = 5;
int NUMBER_OF_RESOURCES = 4;
#define FILE_NAME "max.txt"
int* available;
int** maximum;
int** need;
int** allocation;

void tokenize_request(char *string, int *req_res){
    char *token;
    int i = 0;
    token = strtok(string, ", ");
    while (token != NULL) {
        req_res[i] = atoi(token);
        i++;
        token = strtok(NULL, ", ");

    }
    
}

int mem_alloc_func() {
    // Memory allocation for starting aavailabe resource
    available = malloc(NUMBER_OF_RESOURCES * sizeof(int));
    if (available == NULL) {
        perror("Memory Allocation For Available Failed\n");
        return 1;
    }
    // Memory allocation for rows(customers)
    maximum = malloc(NUMBER_OF_CUSTOMERS * sizeof(int*));
    need = malloc(NUMBER_OF_CUSTOMERS * sizeof(int*));
    allocation = malloc(NUMBER_OF_CUSTOMERS * sizeof(int*));
    if (maximum == NULL || need == NULL || allocation == NULL) {
        perror("Memory Allocation For Customers Failed\n");
        return 1;
    }
    //Memory allocation for columns of each row
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        maximum[i] = malloc(NUMBER_OF_RESOURCES * sizeof(int));
        need[i] = malloc(NUMBER_OF_RESOURCES * sizeof(int));
        allocation[i] = malloc(NUMBER_OF_RESOURCES * sizeof(int));
        if (maximum[i] == NULL || need[i] == NULL || allocation[i] == NULL) {
            perror("Memory Allocation For Resources Failed\n");
            for (int j = 0; j < i; j++) {
                free(maximum[j]);
                free(need[j]);
                free(allocation[j]);
            }
            return 1;
        }
    }
    return 0;
}

int mem_realloc_func() {
    int new_customer_size = NUMBER_OF_CUSTOMERS * 2;
    int** new_maximum = realloc(maximum, new_customer_size * sizeof(int*));
    int** new_need = realloc(need, new_customer_size * sizeof(int*));
    int** new_allocation = realloc(allocation, new_customer_size * sizeof(int*));

    // handle realloc error with free
    if (new_maximum == NULL || new_need == NULL || new_allocation == NULL) {
        perror("Realloc For Customers Failed\n");
        for (int i = 0; i < NUMBER_OF_CUSTOMERS / 2; i++) {
            free(maximum[i]);
            free(need[i]);
            free(allocation[i]);
        }
        free(maximum);
        free(need);
        free(allocation);
        return 1;
    }
    maximum = new_maximum;
    need = new_need;
    allocation = new_allocation;

    // Allocate new rows for the expanded section
    for (int i = NUMBER_OF_CUSTOMERS; i < new_customer_size; i++) {
        maximum[i] = malloc(NUMBER_OF_RESOURCES * sizeof(int));
        need[i] = malloc(NUMBER_OF_RESOURCES * sizeof(int));
        allocation[i] = malloc(NUMBER_OF_RESOURCES * sizeof(int));
    }

    NUMBER_OF_CUSTOMERS = new_customer_size;
    return 0;
}

int file_read_function() {
    FILE* file;
    file = fopen(FILE_NAME, "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
    
    char data_get[256];
    int customers_counted = 0, resources_counted = 0;
    char* token;
    
    while (fgets(data_get, 100, file) != NULL) {
        if (customers_counted >= NUMBER_OF_CUSTOMERS) mem_realloc_func();
        token = strtok(data_get, ", ");
        while (token != NULL && resources_counted < NUMBER_OF_RESOURCES) {
            maximum[customers_counted][resources_counted] = atoi(token);
            need[customers_counted][resources_counted] = atoi(token);
            resources_counted++;
            token = strtok(NULL, ", ");
        }
        // CHECK IF RESOURCES COUNTED MATCHED STARTING RESOURCES NUMBER AND IF IT MATCHED THE LAST
        // NUMBER OF RESOURCES COUNTED IF IT HAD ANY
        // if (NUMBER_OF_RESOURCES < resources_counted) NUMBER_OF_RESOURCES = resources_counted;
        resources_counted = 0;
        customers_counted++;
    }

    NUMBER_OF_CUSTOMERS = customers_counted;
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        printf("Customer %d: [", i);
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
            printf(j != NUMBER_OF_RESOURCES - 1 ? "%d, " : "%d]\n", maximum[i][j]);
        }
    }

    fclose(file);
    return 0;
}

int request_resources(int customer_num, int request[]) { }

int main(int argc, char const* argv[]) {
    NUMBER_OF_RESOURCES = argc - 1; // Dynamically setting the number of resources
    if (mem_alloc_func() != 0) {
        perror("Memory Allocation Failed in mem_alloc_func\n");
        return 1;
    }

    printf("\nWelcome to the Banker's Algorithm Simulation\n\n");
    printf("Initializing system with resources:\n");

    // Check if both command line arguments are provided
    for (int i = 0; i < argc-1; i++) {
    // Populating array with int type of availabe resource values from CLA
        if ((available[i] = atoi(argv[i+1])) == 0) {                                                //FIX THiS CHECK TO ALLOLW 0s
            perror("Please Run The Program Again With Correct Format For Available Resources.\n");
            fprintf(stderr, "Example: %s 10 5 7 8...\n", argv[0]);
            return 1;
        }
    }
    //Prints the available resources to stdout
    printf("Available: [");
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        printf(i != NUMBER_OF_RESOURCES - 1 ? "%d, " : "%d]\n\n", available[i]);
    }

    printf("Reading maximum request file...\n");
    printf("Maximum requests initialized:\n");
        if (file_read_function() != 0) {
        perror("Error in file_read_function\n");
        return 1;
    }    

    char cmd[50]; // buffer overloads unaccounted
    char* token;
    int requested_resources[NUMBER_OF_RESOURCES];
    while (1) {
        printf("\nEnter a command: \n");
        printf("- 'RQ customer_num resource_1 resource_2 resource_3 resource_4' to request "
               "resources\n");
        printf("- 'RL customer_num resource_1 resource_2 resource_3 resource_4' to release "
               "resources\n");
        printf("- '*' to display the current values of the data structures\n");
        printf("- 'exit' to quit\n");

        if (fgets(cmd, sizeof(cmd), stdin) != NULL) {
            cmd[strcspn(cmd, "\n")] = '\0';
            token = strtok(cmd, " ");

            // 'RQ' Command Function
            if (strcmp(token, "RQ") == 0) {
                token = strtok(NULL, " ");

                int req_customer = atoi(token);
                char req_resources[32];
                strcpy(req_resources, token+strlen(token)+1);
                
                tokenize_request(req_resources, requested_resources);
                // printf("This is the customer ID: %d\n", req_customer);
                // for (int i = 0; i < NUMBER_OF_RESOURCES; i++)  {
                //     printf("Checking: Printing Digit: %d\n", requested_resources[i]);
                // }
                // return 1;
            }

            // 'RL' Command Function
            if (strcmp(token, "RL") == 0) {
                token = strtok(cmd, " ");
                int req_customer = atoi(token);
                char req_resources[32];
                strcpy(req_resources, token + strlen(token) + 3);
                
                tokenize_request(req_resources, requested_resources);
                // printf("This is the customer ID: %d\n", req_customer);
                // for (int i = 0; i < NUMBER_OF_RESOURCES; i++)  {
                //     printf("Checking: Printing Digit: %d\n", requested_resources[i]);
                // }
                // return 1;
            }

            // '* Command Function
            if (strcmp(token, "*") == 0) {
                token = strtok(cmd, " ");
                int req_customer = atoi(token);
                
            }

            // 'exit' Command Function
            if (strcmp(token, "exit") == 0) {
                return 0;
            }

        }
    }

    // Read arguement lines
    // Read max.txt
    // run a loop for user interactins with commands 'RQ', 'RL', and '*'
    // RQ - request resurces; RL - releases resources; * - outputs values of the data structures
    // Ex; RQ 0 3 1 2 1

    return 0;
}
