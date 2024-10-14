# Assignment description : PA3 Explanation Video
[Project Explanation Video.](https://www.youtube.com/watch?v=eY9MmRYtNSk)

## Task 1:

Implement a solution to the critical section problem for 2 threads using mutex locks OR semaphore. It's up to you to choose one tool.

Both two threads need to concurrently increment a shared variable called counter by one 2,000,000 times starting from zero.  all global memory is shared among threads of a process. Both threads need to count to 2,000,000 which means the only correct overall count is 4,000,000.

Thread1 needs to be designed in a way that every time it sees 

```(counter−> value%100) == 0```

it increments counter−> value by 100 (bonus). That counts for 100 individual updates.  
You also need to keep track of how many times the thread got the bonus and report it in its remainder section.
Thread2 does not get any bonus, it only increments counter->value by 1.
You should prevent each thread from counting to more than 2,000,000. 
Remember the parent process needs to wait for its 2 threads to join.
Your code will be tested on a Linux machine with gcc compiler using the command:

```gcc -ofname thread-solution.c -lpthread```

## Task 2:

1- Make comments indicating the following sections in your code:

    entry section
    critical section
    exit section
    remainder section

2- In your PDF report, prove that your implementation is a solution to the critical section problem. In other words, prove that you code satisfies the 3 conditions of the critical section solution ( Mutual exclusion, progress, and bounded wait). 

Expected output:
Both threads need to report the number of updates done at the end of their remainder sections.
Both threads need to report the current value of the shared variable counter at the end of their remainder sections. 
thread1 needs to report the number of times it got the bonus at the end of its remainder sections. 

```
Im thread1, I did  2000000 updates and I got the bonus for 16089 times, counter  =  2554849
Im thread2, I did  2000000 updates, counter  =  4000000
from parent counter  =  4000000
```
 

# IMPORTANT!

## No loop should be inside the critical section! When a thread acquires the mutex, it is allowed to do ONLY ONE manipulation of counter->value in the critical section to give the other thread a chance to make progress.
