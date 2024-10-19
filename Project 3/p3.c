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

int MAX_UPDATES = 2000000;
/* Mutex lock */
pthread_mutex_t mutex;

/* Struct a shared variable to store result */
struct shared_data
{
    int value;
    int bonus;
    int thread1_updates;
    int thread2_updates;
};
/* Global shared variable */
struct shared_data *counter;

/* Thread1 function */
void *thread1_funct()
{
    while (counter->thread1_updates < MAX_UPDATES)
    {
        if (pthread_mutex_trylock(&mutex) == 0)
        {
            if ((counter->value % 100) == 0 && counter->thread1_updates + 100 <= MAX_UPDATES)
            {
                counter->value += 100;
                counter->bonus++;
                counter->thread1_updates += 100;
            }
            else
            {
                counter->value ++;
                counter->thread1_updates++;
            }
            pthread_mutex_unlock(&mutex);
        }
    }
    
    printf("Im thread 1, I did %d updates and got the bonus %d times, counter = %d\n", counter->thread1_updates, counter->bonus, counter->value);
    return NULL;
}

/* Thread2 function */
void *thread2_funct()
{
    while (counter->thread2_updates < MAX_UPDATES)
    {
        if (pthread_mutex_trylock(&mutex) == 0)
        {
            counter->value++;
            counter->thread2_updates++;
            pthread_mutex_unlock(&mutex);
        }
    }

    printf("Im thread 2, I did %d updates, counter = %d\n", counter->thread2_updates, counter->value);
    return NULL;
}

int main(int argc, char const *argv[])
{
    /* Allocate memory for shared data */
    counter = (struct shared_data *) malloc(sizeof(struct shared_data));
    counter->value = 0;
    counter->bonus = 0;
    counter->thread1_updates = 0;
    counter->thread2_updates = 0;

    pthread_t threads[2];
    int err;

    /* Initialize mutex lock */
    if ((pthread_mutex_init(&mutex, NULL))) {
        printf("Error occured when initialize mutex lock.");
        exit(0);
    }

    pthread_attr_t attr;
    if ((pthread_attr_init(&attr))) {
        printf("Error occured when initialize pthread_attr_t.");
        exit(0);
    }
    if (pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM)){
        fprintf(stderr, "Error occured when setting scope\n");
        exit(0);
    }

    /* Create thread1 */
    if ((err = pthread_create(&threads[0], &attr, &thread1_funct, NULL)))
    {
        fprintf(stderr, "ERROR: pthread_create, Code: %d\n", err);
        exit(0);
    }

    /* Create thread2 */
    if ((err = pthread_create(&threads[1], &attr, &thread2_funct, NULL)))
    {
        fprintf(stderr, "ERROR: pthread_create, Code: %d\n", err);
        exit(0);
    }

    /* Wait for threads to finish */
    for (int i = 0; i < 2; i++)
    {
        if (pthread_join(threads[i], NULL) != 0)
        {
            return 2;
        }
    }

    printf("From parent counter = %d\n", counter->value);

    /* Clean up */
    pthread_mutex_destroy(&mutex);
    pthread_attr_destroy(&attr);
    free(counter);
    pthread_exit(NULL);

    return 0;
}

// Task 2:
// proof that implementation is a solution to critical section section
// add comments to specify different sections of code
