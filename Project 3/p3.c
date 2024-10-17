#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


int MAX = 4000000;
int main(int argc, char const *argv[])
{
    int counter = 0;
    int bonus_counter = 0;

    for (int i = 0; i < MAX; i++)
    {
        if (counter % 100 == 0)
        {
            counter += 100;
            bonus_counter += 1;
        } else{
            counter +=1;
        }
        
    }
    printf("Reached 4Mil: %d\nThe bonus counter was %d\n", counter, bonus_counter);

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
