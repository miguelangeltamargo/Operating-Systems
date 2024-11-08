#include <stdlib.h>
#include <stdio.h>

#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 4
#define FILE_NAME = 'max.txt'

/* The available amount of each resource */
int available[NUMBER_OF_RESOURCES];

/* The maximum demand of each customer */
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

/* The amount currently allocated to each customer */
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

/* The remaining need of each customer */
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

int main(int argc, char const *argv[])
{
    
    //Read arguement lines
    // Read max.txt
    //run a loop for user interactins with commands 'RQ', 'RL', and '*'
    // RQ - request resurces; RL - releases resources; * - outputs values of the data structures
    // Ex; RQ 0 3 1 2 1 

    return 0;
}
