# Task:
### Write a C program that simulates a parallel task execution scenario using process forking. The program should create a specified number of child processes, each performing a unique task. The parent process should wait for all child processes to complete before displaying the final result.
## Requirements:
	1	Your program should take an integer input n from the user, where n represents the number of child processes to be created. n should be less than 5.
	2	Each child process should perform a different task, such as computing the factorial of a number, finding prime numbers in a range, or any computationally intensive operation.
	3	The parent process should display a message before creating (forking) the child processes.
	4	Each child process should print its own identifier (PID) and the task it is performing.
	5	After completing their tasks, each child process should print a completion message.
	6	The parent process should wait for all child processes to finish before displaying a final message.
## Additional Considerations:
	•	Use the fork() system call to create child processes.
	•	You may use other relevant system calls or functions as needed (e.g., wait() , exit() ).
	•	Ensure proper error handling for system calls.
	•	Design the code to guarantee the creation of exactly n children, ensuring that each child executes its intended task. Properly manage the process creation to avoid any unintended duplication or omission of child processes.

## Sample output:
```
$ ./parallel_execution Enter the number of child processes to create: 3 Parent process (PID: 1234) is creating 3 child processes.
Child 1 (PID: 1235) is computing the factorial of 5. Child 2 (PID: 1236) is finding prime numbers up to 20. Child 3 (PID: 1237) is performing a custom task.
Child 1 (PID: 1235) completed its task. Result: 120 Child 2 (PID: 1236) completed its task. Result: 2 3 5 7 11 13 17 19 Child 3 (PID: 1237) completed its task. Result: Custom task completed.
All child processes have completed. Parent (PID: 1234) is displaying the final message.
```
