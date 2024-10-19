// Name: Miguelangel Tamargo
// Panther ID: 5866999
// ################################################################################################
// Assignment 3: Deadlock
// 
// 
// 
// ################################################################################################
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int MAX = 2000000;

struct counts
{
    int counter;
    int bonus_counter;
};


void *count_up(void* arg){
    struct counts *data = (struct counts *)arg;
    int i = 0;

        while (i < MAX)
    {
        if (data->counter % 100 == 0)
        {
            data->counter += 100;
            data->bonus_counter += 1;
            i+=100;
        } else{
            data->counter +=1;
            i+=1;
        }
    }

    return NULL;
}

int main(int argc, char const *argv[])
{
    pthread_t tid[2];
    int rc;
    
    struct counts counters;
    counters.counter = 0;
    counters.bonus_counter = 0;

    pthread_t thread;
    int err;

    err = pthread_create(&thread, NULL, count_up, (void *)&counters);
    if (err) {
        printf("Error creating thread\n");
        return 1;
    }

    printf("Starting Thread: %lu\n\n", (unsigned long)thread);
    printf("Waiting for the thread to end...\n\n");

    pthread_join(thread, NULL);

    printf("Ending Thread: %lu\n", (unsigned long)thread);
    printf("Counter Finished at: %d\nThe bonus counter was %d\n", counters.counter, counters.bonus_counter);

    return 0;
}
//Task 1:
// create 2 threads
// share data structure called counter; each will increment toi a total of 2 mil
// each thread increments to a total of 2 mil but the counter itselff will be a total of 4 mill
// count must be 2mill for both threads
// t1 has advantage. when tw reaches a value of multiplier of 100 it ccan make a hundred of 100 updates
// must keep track of how many times this bonus jump occurs
// Thread2 gets no bonus
// synchronize the counter, which is the Critical section
// semaophore usage

// Task 2:
// proof that implementation is a solution to critical section section
// add comments to specify different sections of code
