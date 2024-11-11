// Name: Miguelangel Tamargo
// Panther ID: 5866999
// ########################################################################################################################################
// Assignment 4: Banker's Algorithm Simulation
// In this program, we simulate the Banker's Algorithm for deadlock avoidance in a multi-process
// system. The program allows customers to request and release resources from the bank while
// ensuring that the system remains in a safe state. This code allows dynamic size of resources and
// customers based on users max.txt file and starting CLA of available resources The code follows
// the necessary safety checks and implements the resource-request and release functions according
// to the algorithm.
// ########################################################################################################################################

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int NUMBER_OF_CUSTOMERS = 5; // Number of customers in the system
int NUMBER_OF_RESOURCES = 4; // Number of resource types

#define FILE_NAME "max.txt"

// Global array and 2D array to populate with resource and customer data.
int* available; // The available amount of each resource
int** maximum; // The maximum demand of each customer
int** need; // The remaining need of each customer
int** allocation; // The amount currently allocated to each customer

// Function to tokenize resource values
void tokenize_resources(char* string, int* req_res) {
    char* token;
    int i = 0;
    token = strtok(string, ", ");
    while (token != NULL) {
        req_res[i] = atoi(token);
        i++;
        token = strtok(NULL, ", ");
    }
}

// Function to allocate memory of all neccesary data structures
int mem_alloc_func() {
    // Memory allocation for available resources
    available = malloc(NUMBER_OF_RESOURCES * sizeof(int));
    if (available == NULL) {
        perror("Memory Allocation For Available Failed\n");
        return 1;
    }
    // Memory allocation for customer arrays
    maximum = malloc(NUMBER_OF_CUSTOMERS * sizeof(int*));
    need = malloc(NUMBER_OF_CUSTOMERS * sizeof(int*));
    allocation = malloc(NUMBER_OF_CUSTOMERS * sizeof(int*));
    if (maximum == NULL || need == NULL || allocation == NULL) {
        perror("Memory Allocation For Customers Failed\n");
        return 1;
    }
    // Memory allocation for resources within each customer
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        maximum[i] = malloc(NUMBER_OF_RESOURCES * sizeof(int));
        need[i] = malloc(NUMBER_OF_RESOURCES * sizeof(int));
        allocation[i] = malloc(NUMBER_OF_RESOURCES * sizeof(int));
        if (maximum[i] == NULL || need[i] == NULL || allocation[i] == NULL) {
            perror("Memory Allocation For Resources Failed\n");
            // Free previously allocated memory in case of failure
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

// Function to reallocate memory if needed
int mem_realloc_func() {
    int new_customer_size = NUMBER_OF_CUSTOMERS * 2;
    int** new_maximum = realloc(maximum, new_customer_size * sizeof(int*));
    int** new_need = realloc(need, new_customer_size * sizeof(int*));
    int** new_allocation = realloc(allocation, new_customer_size * sizeof(int*));

    // Handle realloc error by freeing allocated memory
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

    // Allocate new rows for the expanded customer arrays
    for (int i = NUMBER_OF_CUSTOMERS; i < new_customer_size; i++) {
        maximum[i] = malloc(NUMBER_OF_RESOURCES * sizeof(int));
        need[i] = malloc(NUMBER_OF_RESOURCES * sizeof(int));
        allocation[i] = malloc(NUMBER_OF_RESOURCES * sizeof(int));
    }

    NUMBER_OF_CUSTOMERS = new_customer_size;
    return 0;
}

// Function to read the maximum demands from the file
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

    while (fgets(data_get, 128, file) != NULL) {
        if (customers_counted >= NUMBER_OF_CUSTOMERS) mem_realloc_func();
        token = strtok(data_get, ", ");
        while (token != NULL && resources_counted < NUMBER_OF_RESOURCES) {
            maximum[customers_counted][resources_counted] = atoi(token);
            need[customers_counted][resources_counted] = atoi(token);
            allocation[customers_counted][resources_counted] = 0; // Initialize allocation to 0
            resources_counted++;
            token = strtok(NULL, ", ");
        }
        resources_counted = 0;
        customers_counted++;
    }

    NUMBER_OF_CUSTOMERS = customers_counted;
    // Display the initialized maximum demands
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        printf("Customer %d: [", i);
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
            printf(j != NUMBER_OF_RESOURCES - 1 ? "%d, " : "%d]\n", maximum[i][j]);
        }
    }

    fclose(file);
    return 0;
}

// Function to check if the system is in a safe state
int is_safe() {
    int work[NUMBER_OF_RESOURCES];
    int finish[NUMBER_OF_CUSTOMERS];
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        work[i] = available[i]; // Initialize work to available
    }
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        finish[i] = 0; // Initialize finish flags
    }

    while (1) {
        int found = 0;
        for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
            if (!finish[i]) {
                int j;
                for (j = 0; j < NUMBER_OF_RESOURCES; j++) {
                    if (need[i][j] > work[j]) {
                        break; // Not enough resources for customer i
                    }
                }
                if (j == NUMBER_OF_RESOURCES) {
                    // Customer i can finish
                    for (int k = 0; k < NUMBER_OF_RESOURCES; k++) {
                        work[k] += allocation[i][k];
                    }
                    finish[i] = 1;
                    found = 1;
                }
            }
        }
        if (!found) {
            break; // No progress can be made
        }
    }

    // Check if all customers could finish
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        if (!finish[i]) {
            return 0; // System is not in a safe state
        }
    }
    return 1; // System is in a safe state
}

// Function to handle resource requests from customers
int request_resources(int customer_num, int request[]) {
    // Check if Request <= Need
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        if (request[i] > need[customer_num][i]) {
            printf("Request denied: exceeds its maximum claim for resource \n");
            return -1; // Return failure
        }
    }

    // Check if Request <= Available
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        if (request[i] > available[i]) {
            printf("Request denied: resources are not available\n");
            return -1; // Return failure
        }
    }

    // Pretend to allocate resources
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        available[i] -= request[i];
        allocation[customer_num][i] += request[i];
        need[customer_num][i] -= request[i];
    }

    // Check if the new state is safe
    if (is_safe()) {
        printf("Request has been granted. System is in a safe state.\n");
        return 0; // Return success
    } else {
        // Revert allocation if not safe
        for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
            available[i] += request[i];
            allocation[customer_num][i] -= request[i];
            need[customer_num][i] += request[i];
        }
        printf("Request denied. System would be left in an unsafe state.\n");
        return -1; // Return failure
    }
}

// Function to handle resource release from customers
void release_resources(int customer_num, int release[]) {
    // Check the customer has enough resources to release
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        if (release[i] > allocation[customer_num][i]) {
            printf("Error: Cannot release more resources than allocated to customer %d for "
                   "resource %d\n",
                customer_num, i);
            return;
        }
    }
    // Release the resources
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        allocation[customer_num][i] -= release[i];
        available[i] += release[i];
        need[customer_num][i] += release[i];
    }
    printf("Resources released successfully.\n");
    return;
}

// Function to display the current state of the system
void display_state() {
    printf("\nCurrent system state:\n");

    // Print available resources
    printf("Available: [");
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        printf("%d%s", available[i], (i < NUMBER_OF_RESOURCES - 1) ? ", " : "]\n");
    }

    // Print maximum resource demand of each customer
    printf("Maximum:\n");
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        printf("Customer %d: [", i);
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
            printf("%d%s", maximum[i][j], (j < NUMBER_OF_RESOURCES - 1) ? ", " : "]\n");
        }
    }

    // Print current allocation to each customer
    printf("Allocation:\n");
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        printf("Customer %d: [", i);
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
            printf("%d%s", allocation[i][j], (j < NUMBER_OF_RESOURCES - 1) ? ", " : "]\n");
        }
    }

    // Print remaining need of each customer
    printf("Need:\n");
    for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
        printf("Customer %d: [", i);
        for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
            printf("%d%s", need[i][j], (j < NUMBER_OF_RESOURCES - 1) ? ", " : "]\n");
        }
    }
}

int main(int argc, char const* argv[]) {
    NUMBER_OF_RESOURCES = argc - 1; // Dynamically setting the number of resources
    if (mem_alloc_func() != 0) {
        perror("Memory Allocation Failed in mem_alloc_func\n");
        return 1;
    }

    printf("\nWelcome to the Banker's Algorithm Simulation\n\n");
    printf("Initializing system with resources:\n");

    // Initialize available resources from command-line arguments
    for (int i = 0; i < argc - 1; i++) {
        // Convert the argument to an integer
        available[i] = atoi(argv[i + 1]);
        // If atoi returns 0, check if the input was not "0"
        if (available[i] == 0 && strcmp(argv[i + 1], "0") != 0) {
            perror("Please Run The Program Again With Correct Format For Available Resources.\n");
            fprintf(stderr, "Example: %s 10 5 7 8...\n", argv[0]);
            return 1;
        }
    }

    // Print the available resources to stdout
    printf("Available: [");
    for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
        printf(i != NUMBER_OF_RESOURCES - 1 ? "%d, " : "%d]\n\n", available[i]);
    }

    printf("Reading maximum request file...\n");
    printf("Maximum requests initialized:\n");
    // reading file function called and accounted for error handling
    if (file_read_function() != 0) {
        perror("Error in file_read_function\n");
        return 1;
    }

    char cmd[128]; // Command buffer (buffer overflow unaccounted for)
    char* token;
    int requested_resources[NUMBER_OF_RESOURCES];
    char req_resources[32];
    int customer_id;
    while (1) {
        printf("\nEnter a command: \n");
        printf("- 'RQ customer_num resource_1 resource_2 resource_3 resource_4' to request "
               "resources\n");
        printf("- 'RL customer_num resource_1 resource_2 resource_3 resource_4' to release "
               "resources\n");
        printf("- '*' to display the current values of the data structures\n");
        printf("- 'exit' to quit\n");

        if (fgets(cmd, sizeof(cmd), stdin) != NULL) {
            cmd[strcspn(cmd, "\n")] = '\0'; // Remove newline character
            token = strtok(cmd, " ");

            if (token == NULL) {
                continue;
            }

            // 'RQ' Command Function
            if (strcmp(token, "RQ") == 0) {
                memset(req_resources, 0,
                    sizeof(req_resources)); // Resets the req_resources array to 0's
                token = strtok(NULL, " ");

                customer_id = atoi(token);
                // Validate customer number
                if (customer_id < 0 || customer_id >= NUMBER_OF_CUSTOMERS) {
                    printf("Error: Invalid customer number %d.\n", customer_id);
                    continue;
                }
                printf("Requesting resources for Customer %d\n", customer_id);
                // Tokenizing requested resources
                strcpy(req_resources, token + strlen(token) + 1);
                tokenize_resources(req_resources, requested_resources);
                // Calling request_resource to check for system safety and grant resources
                request_resources(customer_id, requested_resources);
            }

            // 'RL' Command Function
            else if (strcmp(token, "RL") == 0) {
                memset(req_resources, 0,
                    sizeof(req_resources)); // Resets the req_resource array to 0's
                token = strtok(NULL, " ");

                customer_id = atoi(token);
                // Validate customer number
                if (customer_id < 0 || customer_id >= NUMBER_OF_CUSTOMERS) {
                    printf("Error: Invalid customer number %d.\n", customer_id);
                    continue;
                }
                printf("Releasing resources for Customer %d\n", customer_id);
                // Tokenizing released resources
                strcpy(req_resources, token + strlen(token) + 1);
                tokenize_resources(req_resources, requested_resources);
                // Calling release_resources function to update allocation, available and need array
                release_resources(customer_id, requested_resources);
            }

            // '*' Command Function
            else if (strcmp(token, "*") == 0) {
                display_state();
            }

            // 'exit' Command Function
            else if (strcmp(token, "exit") == 0) {
                printf("Exiting program. Goodbye!\n");

                // Free allocated memory before exiting
                free(available);
                for (int i = 0; i < NUMBER_OF_CUSTOMERS; i++) {
                    free(maximum[i]);
                    free(need[i]);
                    free(allocation[i]);
                }
                free(maximum);
                free(need);
                free(allocation);

                return 0;
            }

            // Handle invalid commands
            else {
                printf("Invalid command. Please try again.\n");
            }
        }

        // Clear input buffer before prompting for a new command
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }

    return 0;
}