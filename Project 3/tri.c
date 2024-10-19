#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int MAX = 2000000;
pthread_mutex_t mutex;

struct thread_data
{
    int count;
    int bonus;
    int thread1_updates;
    int thread2_updates;
};

void *thread1_funct(void *arg)
{
    struct thread_data *data = (struct thread_data *)arg;
    while (data->thread1_updates < MAX)
    {
        if (pthread_mutex_trylock(&mutex) == 0)
        {
            if ((data->count % 100) == 0 && data->thread1_updates + 100 <= MAX)
            {
                data->count += 100;
                data->bonus++;
                data->thread1_updates += 100;
            }
            else
            {
                data->count ++;
                data->thread1_updates++;
            }
            pthread_mutex_unlock(&mutex);
        }
    }
    
    printf("Im thread 1, I did %d updates and got the bonus %d times, counter = %d\n", data->thread1_updates, data->bonus, data->count);
    return NULL;
}

void *thread2_funct(void *arg)
{
    struct thread_data *data = (struct thread_data *)arg;
    while (data->thread2_updates < MAX)
    {
        if (pthread_mutex_trylock(&mutex) == 0)
        {
            data->count++;
            data->thread2_updates++;
            pthread_mutex_unlock(&mutex);
        }
    }

    printf("Im thread 2, I did %d updates, counter = %d\n", data->thread2_updates, data->count);
    return NULL;
}

int main(int argc, char const *argv[])
{
    struct thread_data data;
    data.count = 0;
    data.bonus = 0;
    data.thread1_updates = 0;
    data.thread2_updates = 0;

    pthread_t thread[2];
    int err;

    if (pthread_mutex_init(&mutex, NULL) != 0)
    {
        printf("Mutex Initialization Faliled!\n");
        return 1;
    }

    if (pthread_create(&thread[0], NULL, &thread1_funct, (void *)&data) != 0)
    {
        printf("Thread Creation Failed!\n");
        return 1;
    }
    printf("Thread 1 has started\n");

    if (pthread_create(&thread[1], NULL, &thread2_funct, (void *)&data) != 0)
    {
        printf("Thread Creation Failed!\n");
        return 1;
    }
    printf("Thread 2 has started\n");

    for (int i = 0; i < 2; i++)
    {
        if (pthread_join(thread[i], NULL) != 0)
        {
            return 2;
        }
    }
    pthread_mutex_destroy(&mutex);
    printf("From parent: counter = %d\n", data.count);
    return 0;
}
