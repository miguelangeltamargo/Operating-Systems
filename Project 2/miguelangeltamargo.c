// Name: Miguelangel Tamargo
// Panther ID: 5866999
// ################################################################################################
// Assignment 2: Scheduling Algorithms
// This program calculates the average waiting time and the average turn around time of the user
// specified scheduling algorithm of First Come First Server (FCFS) and Shortest Job First (SJF).
// The program will then print the averages along with a diagram of processes execution order.
// ################################################################################################
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

// A structure to represent a process in the scheduling algorithm
typedef struct
{
	char name[10];
	int arrival;
	int burst;
	int start;
	int completion;
	int waiting_time;
	int turnaround_time;
} PROCESS;

// This function converts a string to lowercase
void lowercase_conv(char *str)
{
	for (int i = 0; str[i] != '\0'; i++)
	{
		str[i] = tolower((unsigned char)str[i]);
	}
}

// A comparison function for sorting processes by burst time (for SJF scheduling) used for qsort() function.
int burst_comparison(const void *a, const void *b)
{
	PROCESS *process_A = (PROCESS *)a;
	PROCESS *process_B = (PROCESS *)b;

	return process_A->burst - process_B->burst;
}
// A comparison function for sorting processes by arrival time (for FCFS scheduling) used for qsort() function
int arrival_comparison(const void *a, const void *b)
{
	PROCESS *process_A = (PROCESS *)a;
	PROCESS *process_B = (PROCESS *)b;

	return process_A->arrival - process_B->arrival;
}

// This function calculates the average waiting time of processes
float waiting_avg(PROCESS *p, int arr_len)
{
	float avg;
	float total = 0;
	for (int i = 0; i < arr_len; i++)
	{
		total += p[i].waiting_time;
	}
	return (avg = total / arr_len);
}

// This function calculates the average turn around time of processes
float tat_avg(PROCESS *p, int arr_len)
{
	float avg;
	float total = 0;
	for (int i = 0; i < arr_len; i++)
	{
		total += p[i].turnaround_time;
	}
	return (avg = total / arr_len);
}
// This function prints the SJF execution diagram
void sjf_execution_order(char **done_process, int done_count) {
    // Loop through completed processes and print their names
    for (int i = 0; i < done_count; i++)
    {
        if (i == done_count - 1) // Check if it's the last completed process
        {
            printf("%s\n", done_process[i]); // Print without the arrow
        }
        else
        {
            printf("%s -> ", done_process[i]); // Print with the arrow
        }
    }
}
// This function prints the FCFS execution diagram
void fcfs_execution_order(PROCESS *process_list, int num_process) {
    for (int i = 0; i < num_process; i++)
    {
        if (i == num_process - 1)
        {
            printf("%s\n", process_list[i].name);
        }
        else
        {
            printf("%s -> ", process_list[i].name);
        }
    }
}

int main(int argc, char **argv)
{

	// Check if both command line arguments are provided
    if (argc != 3) {
        fprintf(stderr, "Please Run The Program Again With Correct Arguments.\n");
        fprintf(stderr, "Example: %s myfile.txt FCFS\n", argv[0]);
        return 1;
    }



	FILE *file;
	int num_process;
	int *arrival;
	int *burst;
	int i;
	int process_clock = 0; // clock to track scheduling time

	// Open the file
	file = fopen(argv[1], "r");
	// Read the first line - number of processes, note the '&' operator before num_process
	fscanf(file, "%d", &num_process);

	// Allocate memory for structs of the number of processes
	PROCESS *process_list = malloc(sizeof(PROCESS) * num_process);
	// Handling Malloc Error
	if (process_list == NULL)
	{
		printf("Malloc Error\n");
		return 1;
	}

 	// Read process data from file
	for (i = 0; i < num_process; i++)
	{
		strcpy(process_list[i].name, "P"); // initialize process name
        char num_str[5]; // temporary string for numbering processes
        sprintf(num_str, "%d", i + 1); // create process number string from int i in num_str
        strcat(process_list[i].name, num_str); // concat number to process name

        fscanf(file, "%d", &process_list[i].arrival);
        fscanf(file, "%d", &process_list[i].burst);
    }

	// close file
	fclose(file);

	char sched[20];	// variable to hold scheduling algorithm name
	strcpy(sched, argv[2]); // copy scheduling type from command line
	lowercase_conv(sched); // convert to lowercase for comparison

	// First Come First Served scheduling
	if (strcmp(sched, "fcfs") == 0)
	{
		// sort the processes by arrval time
		qsort(process_list, num_process, sizeof(PROCESS), arrival_comparison);

		// simulate processes based on arrival time and fill attributes
		for (i = 0; i < num_process; i++)
		{
			process_list[i].start = process_clock;
			process_list[i].completion = process_clock + process_list[i].burst;
			process_clock += process_list[i].burst;

			// calculate turnaround and waiting times
			process_list[i].turnaround_time = process_list[i].completion - process_list[i].arrival;
			process_list[i].waiting_time = process_list[i].start - process_list[i].arrival;
		}
		// prints the FCFS execution order diagram 
		fcfs_execution_order(process_list, num_process);
	}





	// Shortest Job First scheduling
	else if (strcmp(sched, "sjf") == 0)
{
	// Sort processes by arrival time initially
    qsort(process_list, num_process, sizeof(PROCESS), arrival_comparison);
    int i = 0;
	// Handle the first process manually as its doesn't depend on others
    process_list[i].start = process_clock;
    process_list[i].completion = process_clock + process_list[i].burst;
    process_clock += process_list[i].burst;

    process_list[i].turnaround_time = process_list[i].completion - process_list[i].arrival;
    process_list[i].waiting_time = process_list[i].start - process_list[i].arrival;

    // create an array to store processes that have arrived
    PROCESS *arrived_processes = malloc(sizeof(PROCESS) * num_process);
    // Handling Malloc Error
    if (arrived_processes == NULL)
    {
        printf("Malloc Error-arrived_processes\n");
        return 1;
    }

	// Dynamic array to keep track of completed processes names
    char **done_process = malloc(num_process * sizeof(char *));
	// Handling Malloc Error
    if (done_process == NULL)
    {
        printf("Malloc Error - done_process!\n");
        return 1;
    }

    // Allocate memory for the first name of the completed process and copy it to array
    done_process[i] = malloc(strlen(process_list[i].name) + 1);
    strcpy(done_process[i], process_list[i].name);

    int arrived_count = 1; // account for first process already scheduled
    int done_count = 1; // account with first process completed
    char *next_process = malloc(10 * sizeof(char)); // buffer for next process name
    if (next_process == NULL)
    {
        printf("Malloc Error - next_process!\n");
        free(arrived_processes);
        free(done_process);
        return 1;
    }

	// scheduling loop to handle the SJF algorithm
    while (done_count < num_process)
    {
        // Update the arrived process list
        arrived_count = 0; // Reset for this iteration
        for (int x = 0; x < num_process; x++)
        {
			// check if the process has arrived by comparing arrival time versus the current process clock
            if (process_list[x].arrival <= process_clock)
            {
                arrived_processes[arrived_count] = process_list[x]; // store the arrived process
				arrived_count++;
            }
        }

		// Sort only the array of arrived processes by burst time
        qsort(arrived_processes, arrived_count, sizeof(PROCESS), burst_comparison);

        // Find the next process to schedule with the shortest burst
        int found_next = 0; // flag to check if we found a valid next process
        for (int y = 0; y < arrived_count; y++)
        {
            // check if the process has not already been completed
            int is_done = 0;
            for (int x = 0; x < done_count; x++)
            {
                if (strcmp(done_process[x], arrived_processes[y].name) == 0)
                {
                    is_done = 1; // flag as done
                    break;
                }
            }
			// If the process is not done, select it as the next process to schedule
            if (!is_done)
            {
                // copy the name of the next process to next_process for scheduling
                strcpy(next_process, arrived_processes[y].name);
                found_next = 1; // set flag as found
                break;
            }
        }

		// Proceed if we found a valid next process
        if (found_next)
        {
            // Find the next process in the process_list to schedule
            for (int r = 0; r < num_process; r++)
            {
                if (strcmp(process_list[r].name, next_process) == 0)
                {
                    // setting process struct attributes
                    process_list[r].start = process_clock;
                    process_list[r].completion = process_clock + process_list[r].burst;
                    process_clock += process_list[r].burst;

                    process_list[r].turnaround_time = process_list[r].completion - process_list[r].arrival;
                    process_list[r].waiting_time = process_list[r].start - process_list[r].arrival;

                    // allocate memory for completed process scheduling name for more comparisons
                    done_process[done_count] = malloc(strlen(process_list[r].name) + 1);
                    strcpy(done_process[done_count], process_list[r].name);
                    done_count++;
                    break;
                }
            }
        }
    }


	// Print SJF execution order diagram
	sjf_execution_order(done_process, done_count);


    // free memory allocations
    free(arrived_processes);
    free(next_process);
    free(done_process);
}
	// calculating waiting average and turn around average
	float Final_Waiting_Avg = waiting_avg(process_list, num_process);
	float Final_TAT_Avg = tat_avg(process_list, num_process);

	// printf("Here's your processes :\n");
	// for (int i = 0; i < num_process; i++)
	// {
	// 	printf("Process: %s\tArrival: %d\tBurst: %d\tStart: %d\tCompletion: %d\tWaitingTime: %d\tTurnAroundTime: %d\n", process_list[i].name,
	// 		   process_list[i].arrival, process_list[i].burst, process_list[i].start, process_list[i].completion,
	// 		   process_list[i].waiting_time, process_list[i].turnaround_time);
	// }

	//priniting final turn around time and waiting time averages
	printf("Average Waiting Time: %.2f\n", Final_Waiting_Avg);
	printf("Average Turnaround Time: %.2f\n", Final_TAT_Avg);


	// freeing last allocated memory
	free(process_list);

	return 0;
}
