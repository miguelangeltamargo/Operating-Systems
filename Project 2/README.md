## Objective

### Implement FCFS and SJF scheduling algorithms in C and calculate the average waiting time and average turnaround time of concurrent processes.
## Description:

    Input:  the number of processes along with the burst time and arrival time for each process. In addition to the desired scheduling algorithm (FCFS or SJF). The program should expect a file name and a scheduling algorithm as arguments.

      Ex: myprogram myfile.txt FCFS

    The file should contain the number of processes in the first line. Then, each following line should contain the burst time and arrival time of one process as follows:

```

<number of processes>
<process 1 arrival time> <process 1 burst time>
<process 2 arrival time> <process 2 burst time>
…

``` 

Sample Output:
```
Order of Execution: P1 -> P2 -> P3
Average Waiting Time: 2.33
Average Turnaround Time: 8.33
```
