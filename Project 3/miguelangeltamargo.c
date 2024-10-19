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
/* Mutex lock data type initialized*/
pthread_mutex_t mutex;

/* Struct to store shared variable between threads */
struct shared_data
{
    int value;              //Counnter variable that boith threads will update incrementally
    int bonus;              //Bonus counter for thread 1 
    int thread1_updates;    //Counter for number of updates made by thread 1
    int thread2_updates;    //Counter for number of updates made by thread 2
};
/* Global shared variable */
struct shared_data *counter;

/* Thread1 function */
void *thread1_funct()
{
    while (counter->thread1_updates < MAX_UPDATES)
    {
        // Thread 1 Entry Section: Checks if the mutex is locked
        if (pthread_mutex_trylock(&mutex) == 0)
        {
            // Thread 1 Critical Section: Modifing the shared data
            if ((counter->value % 100) == 0 && counter->thread1_updates + 100 <= MAX_UPDATES)
            {
                //Bonus section of thread 1, safety check to increment by 100 without overflow
                counter->value += 100;
                counter->bonus++;
                counter->thread1_updates += 100;
            }
            else
            {
                //Regular increment
                counter->value ++;
                counter->thread1_updates++;
            }
            // Thread 1 Exit Section: Unlocks the mutex 
            pthread_mutex_unlock(&mutex);
        }
    }
    // Thread 1 Remainder Section: Executes after critical section
    printf("Im thread 1, I did %d updates and got the bonus %d times, counter = %d\n", counter->thread1_updates, counter->bonus, counter->value);
    return NULL;
}

/* Thread2 function */
void *thread2_funct()
{
    while (counter->thread2_updates < MAX_UPDATES)
    {
        // Thread 2 Entry Section: Checks if the mutex is locked
        if (pthread_mutex_trylock(&mutex) == 0)
        {
            // Thread 2 Critical Section: Modifing the shared data
            counter->value++;
            counter->thread2_updates++;

            // Thread 2 Exit Section: Unlocks the mutex
            pthread_mutex_unlock(&mutex);
        }
    }
    // Thread 2 Remainder Section: Executes after critical section
    printf("Im thread 2, I did %d updates, counter = %d\n", counter->thread2_updates, counter->value);
    return NULL;
}

int main(int argc, char const *argv[])
{
    /* Allocate memory for shared data */
    counter = (struct shared_data *) malloc(sizeof(struct shared_data));

    counter->value = 0;             //Initialze counter to 0
    counter->bonus = 0;             //Initialize counter to 0
    counter->thread1_updates = 0;   //Initialize thread 1 update count to 0
    counter->thread2_updates = 0;   //Initialize thread 2 update count to 0

    pthread_t threads[2];           //Array to hold thread IDs
    int err;

    /* Initialize mutex lock */
    if ((pthread_mutex_init(&mutex, NULL))) {
        printf("Error occured when initialize mutex lock.");
        exit(0);
    }

    //Initialize thread attribute and set scope to system-wide
    //Default on linux but, left for use on other OS
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
    // Print final count from parent thread
    printf("From parent counter = %d\n", counter->value);

    /* Clean up */
    pthread_mutex_destroy(&mutex);
    pthread_attr_destroy(&attr);
    free(counter);
    pthread_exit(NULL);             //Exit main thread

    return 0;
}
