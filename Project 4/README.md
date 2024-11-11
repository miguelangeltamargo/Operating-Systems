# Description:

The objective of this project is to develop a program that implements the banker’s algorithm. The program simulates resource allocation and management in a multi-process environment where customers make requests and release resources from a central bank. The banker, acting as the resource manager, will grant requests only if they maintain system safety, ensuring deadlock avoidance.

The Banker:

The banker evaluates requests from n customers for m types of resources. To manage resources effectively, the banker maintains the following data structures:

#define NUMBER_OF_CUSTOMERS 5
#define NUMBER_OF_RESOURCES 4

/* The available amount of each resource */
int available[NUMBER_OF_RESOURCES];

/* The maximum demand of each customer */
int maximum[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

/* The amount currently allocated to each customer */
int allocation[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

/* The remaining need of each customer */
int need[NUMBER_OF_CUSTOMERS][NUMBER_OF_RESOURCES];

The banker grants a request if it adheres to the safety algorithm. Requests that jeopardize system safety are denied. Function prototypes for requesting and releasing resources are defined as follows:

int request_resources(int customer_num, int request[]);
void release_resources(int customer_num, int release[]);

The request_resources() function returns 0 upon successful allocation and -1 upon failure.

Testing Your Implementation:

Design a program that enables user interaction for requesting and releasing resources, as well as displaying the current values of the different data structures (available, maximum, allocation, and need) associated with the banker’s algorithm.

Invoke your program by passing the number of resources of each type through the command line. For instance, if there are four resource types with ten instances of the first type, five of the second type, seven of the third type, and eight of the fourth type, execute your program as follows:

```./a.out 10 5 7 8```

The available array will be initialized with these values. Initially, the program reads a file containing the maximum number of requests for each customer. For example, if there are five customers and four resources, the input file format is as follows:

6,4,7,3
4,2,3,2
2,5,3,3
6,3,3,2
5,6,7,5

Each line in the input file represents the maximum request of each resource type for each customer. The program initializes the maximum array accordingly.

The user then enters commands to request resources, release resources, or display the current values of the data structures. Use 'RQ' for requesting resources, 'RL' for releasing resources, and ' * ' to output the values of the different data structures. For instance, to request resources (3, 1, 2, 1) for customer 0, enter the following command:

```RQ 0 3 1 2 1```

The program outputs whether the request would be satisfied or denied based on the safety algorithm. Similarly, to release resources (1, 2, 3, 1) for customer 4, enter the command:

```RL 4 1 2 3 1```

Finally, entering the command ' * ' outputs the values of the available, maximum, allocation, and need arrays.

Sample execution output (THIS IS JUST AN EXAMPLE):

```Welcome to the Banker's Algorithm Simulation

Initializing system with resources:
Available: [10, 5, 7, 8]

Reading maximum request file...
Maximum requests initialized:
Customer 0: [6, 4, 7, 3]
Customer 1: [4, 2, 3, 2]
Customer 2: [2, 5, 3, 3]
Customer 3: [6, 3, 3, 2]
Customer 4: [5, 6, 7, 5]

Please enter commands:
- 'RQ customer_num resource_1 resource_2 resource_3 resource_4' to request resources
- 'RL customer_num resource_1 resource_2 resource_3 resource_4' to release resources
- '*' to display the current values of the data structures
- 'exit' to quit

Command: RQ 0 3 1 2 1
Requesting resources for Customer 0...
Request granted. System is in a safe state.

Command: RQ 1 2 2 2 2
Requesting resources for Customer 1...
Request granted. System is in a safe state.

Command: RQ 2 1 1 1 1
Requesting resources for Customer 2...
Request denied. System would be left in an unsafe state.

Command: RL 1 2 2 2 2
Releasing resources for Customer 1...
Resources released successfully.

Command: *
Current system state:
Available: [5, 1, 3, 6]
Maximum:
Customer 0: [6, 4, 7, 3]
Customer 1: [4, 2, 3, 2]
Customer 2: [2, 5, 3, 3]
Customer 3: [6, 3, 3, 2]
Customer 4: [5, 6, 7, 5]
Allocation:
Customer 0: [3, 1, 2, 1]
Customer 1: [0, 0, 0, 0]
Customer 2: [0, 0, 0, 0]
Customer 3: [0, 0, 0, 0]
Customer 4: [0, 0, 0, 0]
Need:
Customer 0: [3, 3, 5, 2]
Customer 1: [4, 2, 3, 2]
Customer 2: [2, 5, 3, 3]
Customer 3: [6, 3, 3, 2]
Customer 4: [5, 6, 7, 5]

Command: exit
Exiting program. Goodbye!```
