# TaskScheduler-Cpp
C++ implementation of Task Scheduler with DAG representaion of tasks

The DAG_Generator.cpp file takes in the number of tasks as input and generates a Directed Acyclic Graph (DAG) file in dot format as a file DAG.dot. This can be viewed using dot.
It also generates a dagfile.txt which contains the node number, Execution times and the dependencies of each task and also gives the topological order of the graph

The Task-Scheduler.cpp has the implementation of scheduling using Critical Path Method and List Scheduling and gives the output in the Schedule.txt file. The first output is for Fixed Execution Times and the second is for Variable Execution Time of the tasks.
