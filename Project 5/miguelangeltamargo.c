
// Name: Miguelangel Tamargo
// Panther ID: 5866999
// ########################################################################################################################################
// Assignment 5: Banker's Algorithm Simulation
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
#include <ctype.h>

// Reused from Assignment 2: Function to convert a string to lowercase
void lowercase_conv(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

// Reused from Assignment 2: Function to read integers from a file
int *read_page_references(const char *filename, int *page_count) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file %s.\n", filename);
        return NULL;
    }

    int capacity = 10;
    int *page_references = malloc(capacity * sizeof(int));
    if (!page_references) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        fclose(file);
        return NULL;
    }

    *page_count = 0;
    while (1) {
        int page;
        int result = fscanf(file, "%d", &page);
        if (result == EOF) {
            break;
        } else if (result != 1) {
            fprintf(stderr, "Error: Invalid input in file.\n");
            free(page_references);
            fclose(file);
            return NULL;
        } else {
            if (*page_count >= capacity) {
                capacity *= 2;
                int *temp = realloc(page_references, capacity * sizeof(int));
                if (!temp) {
                    fprintf(stderr, "Error: Memory reallocation failed.\n");
                    free(page_references);
                    fclose(file);
                    return NULL;
                }
                page_references = temp;
            }
            page_references[(*page_count)++] = page;
        }
    }

    fclose(file);
    return page_references;
}

// Reused from Assignment 2: Function to check command-line arguments
int validate_args(int argc, char *argv[], int *number_of_frames) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s pagereffile numberofframes\n", argv[0]);
        return 1;
    }

    *number_of_frames = atoi(argv[2]);
    if (*number_of_frames < 1 || *number_of_frames > 10) {
        fprintf(stderr, "Error: numberofframes must be between 1 and 10.\n");
        return 1;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    int number_of_frames;
    if (validate_args(argc, argv, &number_of_frames)) {
        return 1;
    }

    int page_count;
    int *page_references = read_page_references(argv[1], &page_count);
    if (!page_references) {
        return 1;
    }

    // FIFO Implementation
    int fifo_faults = 0;
    int *fifo_frames = malloc(number_of_frames * sizeof(int));
    if (!fifo_frames) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        free(page_references);
        return 1;
    }
    int fifo_index = 0;

    for (int i = 0; i < number_of_frames; i++) {
        fifo_frames[i] = -1;
    }

    for (int i = 0; i < page_count; i++) {
        int page = page_references[i];
        int found = 0;

        for (int j = 0; j < number_of_frames; j++) {
            if (fifo_frames[j] == page) {
                found = 1;
                break;
            }
        }

        if (!found) {
            fifo_faults++;
            fifo_frames[fifo_index] = page;
            fifo_index = (fifo_index + 1) % number_of_frames;
        }
    }

    // LRU Implementation
    int lru_faults = 0;
    int *lru_frames = malloc(number_of_frames * sizeof(int));
    int *lru_counter = malloc(number_of_frames * sizeof(int));
    if (!lru_frames || !lru_counter) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        free(page_references);
        free(fifo_frames);
        return 1;
    }

    for (int i = 0; i < number_of_frames; i++) {
        lru_frames[i] = -1;
        lru_counter[i] = 0;
    }

    for (int i = 0; i < page_count; i++) {
        int page = page_references[i];
        int found = 0;

        for (int j = 0; j < number_of_frames; j++) {
            if (lru_frames[j] == page) {
                found = 1;
                lru_counter[j] = i; // Update last used time
                break;
            }
        }

        if (!found) {
            lru_faults++;
            int lru_index = 0;
            int min = lru_counter[0];

            for (int j = 1; j < number_of_frames; j++) {
                if (lru_frames[j] == -1) {
                    lru_index = j;
                    break;
                }
                if (lru_counter[j] < min) {
                    min = lru_counter[j];
                    lru_index = j;
                }
            }

            lru_frames[lru_index] = page;
            lru_counter[lru_index] = i;
        }
    }

    // Output the results
    printf("FIFO: %d page faults\n", fifo_faults);
    printf("Final state of memory:");
    for (int i = 0; i < number_of_frames; i++) {
        if (fifo_frames[i] != -1) {
            printf(" %d", fifo_frames[i]);
        }
    }
    printf("\n");

    printf("LRU: %d page faults\n", lru_faults);
    printf("Final state of memory:");
    for (int i = 0; i < number_of_frames; i++) {
        if (lru_frames[i] != -1) {
            printf(" %d", lru_frames[i]);
        }
    }
    printf("\n");

    // Free allocated memory
    free(page_references);
    free(fifo_frames);
    free(lru_frames);
    free(lru_counter);

    return 0;
}