// Name: Miguelangel Tamargo
// Panther ID: 5866999
//####################################################################################################################################
// Assignment 5: FIFO Algorithm For Pages
// This program simulates two page replacement algorithms: First-In-First-Out (FIFO) and Least Recently Used (LRU).
// It reads a sequence of page references from a file, processes them using both algorithms, and reports the number of
// page faults along with the final state of memory frames for each algorithm. The user specifies the number of frames
// via command-line arguments.
//####################################################################################################################################

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Converts a given string to lowercase to ensure uniform processing
void lowercase_conv(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

// Reads page references from a specified file and stores them in a dynamically allocated array
int *read_page_references(const char *filename, int *page_count) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file %s.\n", filename);
        return NULL;
    }

    int capacity = 10; // Initial capacity for page references
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
            break; // End of file reached
        } else if (result != 1) {
            fprintf(stderr, "Error: Invalid input in file.\n");
            free(page_references);
            fclose(file);
            return NULL;
        } else {
            // Expand the array if capacity is exceeded
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
            page_references[(*page_count)++] = page; // Store the page reference
        }
    }

    fclose(file);
    return page_references;
}

// Validates CLA for correct usage and frame limits
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

    // Validate the provided command-line arguments
    if (validate_args(argc, argv, &number_of_frames)) {
        return 1;
    }

    int page_count;
    // Retrieve the sequence of page references from the input file
    int *page_references = read_page_references(argv[1], &page_count);
    if (!page_references) {
        return 1;
    }

    // FIFO Algorithm Implementation
    int fifo_faults = 0; // Counter for FIFO page faults
    int *fifo_frames = malloc(number_of_frames * sizeof(int));
    if (!fifo_frames) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        free(page_references);
        return 1;
    }
    int fifo_index = 0; // Tracks the next frame to replace in FIFO

    // Initialize all FIFO frames to -1 indicating they are empty
    for (int i = 0; i < number_of_frames; i++) {
        fifo_frames[i] = -1;
    }

    // Process each page reference using FIFO strategy
    for (int i = 0; i < page_count; i++) {
        int page = page_references[i];
        int found = 0;

        // Check if the page is already present in one of the frames
        for (int j = 0; j < number_of_frames; j++) {
            if (fifo_frames[j] == page) {
                found = 1;
                break;
            }
        }

        if (!found) { // Page fault occurs
            fifo_faults++;
            fifo_frames[fifo_index] = page; // Replace the oldest page
            fifo_index = (fifo_index + 1) % number_of_frames; // Move to the next frame in FIFO order
        }
    }

    // LRU Algorithm Implementation
    int lru_faults = 0; // Counter for LRU page faults
    int *lru_frames = malloc(number_of_frames * sizeof(int));
    int *lru_counter = malloc(number_of_frames * sizeof(int)); // Tracks last usage time for each frame
    if (!lru_frames || !lru_counter) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        free(page_references);
        free(fifo_frames);
        return 1;
    }

    // Initialize LRU frames and counters
    for (int i = 0; i < number_of_frames; i++) {
        lru_frames[i] = -1;
        lru_counter[i] = 0;
    }

    // Process each page reference using LRU strategy
    for (int i = 0; i < page_count; i++) {
        int page = page_references[i];
        int found = 0;

        // Check if the page is already present and update its usage
        for (int j = 0; j < number_of_frames; j++) {
            if (lru_frames[j] == page) {
                found = 1;
                lru_counter[j] = i; // Update the last used time
                break;
            }
        }

        if (!found) { // Page fault occurs
            lru_faults++;
            int lru_index = 0;
            int min = lru_counter[0];

            // Find the frame with the least recently used page
            for (int j = 1; j < number_of_frames; j++) {
                if (lru_frames[j] == -1) { // Empty frame found
                    lru_index = j;
                    break;
                }
                if (lru_counter[j] < min) {
                    min = lru_counter[j];
                    lru_index = j;
                }
            }

            lru_frames[lru_index] = page; // Replace the LRU page
            lru_counter[lru_index] = i; // Update the usage time
        }
    }

    //  Output Results
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

    // Free Memory
    free(page_references);
    free(fifo_frames);
    free(lru_frames);
    free(lru_counter);

    return 0;
}
