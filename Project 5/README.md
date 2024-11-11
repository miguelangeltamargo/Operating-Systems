description :

Write a program in C to determine how many page faults will occur with a FIFO and LRU replacement policy. You will read from the command line 2 parameters as shown below:

myprogram pagereffile numberofframes

The file will have an integer set of page references separated by spaces without any newlines of space at the end of the line. The number of page references is not limited. The numberofframes must be between 1 and 10. You will output the number of page faults and the final state of memory.

you can use the sample reference string used in class to verify the correctness of your program : 
7 0 1 2 0 3 0 4 2 3 0 3 0 3 2 1 2 0 1 7 0 1
Sample output

./a.out myfile.txt 3
FIFO: 15 page faults
Final state of memory: 7 0 1 
LRU: 12 page faults
Final state of memory: 1 0 7 

### Feature Checklist

| Feature                                | Completed | Started | Not Completed |
|----------------------------------------|:---------:|:-------:|:-------------:|
| **Correct FIFO Logic**                 |     _     |    _    |      🚩       |
| **Correct FIFO Number of Page Faults** |     _     |    _    |      🚩       |
| **Correct FIFO Final Memory State**    |     _     |    _    |      🚩       |
| **Correct LRU Logic**                  |     _     |    _    |      🚩       |
| **Correct LRU Number of Page Faults**  |     _     |    _    |      🚩       |
| **Correct LRU Final Memory State**     |     _     |    _    |      🚩       |