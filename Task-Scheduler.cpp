#include <bits/stdc++.h>
using namespace std;

struct Task
{
    int id;
    int executionTime;
    // int adjustedExecutionTime;
    vector<int> dependencies; // Preceding nodes on which the execution of this node depends
    int criticalPathTime;     // Sum of execution time of the node and its successors
};

// Read the tasks from a file with id, execution time and dependencies of each task
vector<Task> readTasksFromFile(const string &filename)
{
    vector<Task> tasks;
    ifstream file(filename);

    if (file.is_open())
    {
        string line;
        while (getline(file, line))
        {
            if (line == "Topological Sort: ") // To skip reading the topological sort as a task
                return tasks;
            istringstream iss(line);
            int id, time, dependency;
            vector<int> dependencies;

            if (iss >> id >> time)
            {
                while (iss >> dependency)
                {
                    dependencies.push_back(dependency);
                }

                Task task;
                task.id = id;
                task.executionTime = time;
                task.dependencies = dependencies;
                task.criticalPathTime = 0;

                tasks.push_back(task);
            }
        }

        file.close();
    }
    else
    {
        cout << "Unable to open the file: " << filename << endl;
    }

    return tasks;
}

// Read the topological order and store in a vector
vector<int> readLastLineValues(const string &filename)
{
    vector<int> values;

    ifstream file(filename);
    if (file.is_open())
    {
        string line;
        string lastLine;

        // Read the file line by line until reaching the end
        while (getline(file, line))
        {
            lastLine = line; // Store each line, but only the last line will be kept
        }

        file.close();

        // Process the last line to extract the values
        istringstream iss(lastLine);
        int value;

        while (iss >> value)
        {
            values.push_back(value);
        }
    }
    else
    {
        cout << "Unable to open the file: " << filename << endl;
    }

    return values;
}

// Imorovising the order of execution by making all the independent nodes appear first in the raedy list
vector<int> putIndependentNodesAtFront(vector<int> &topologicalSortedValues, vector<Task> &tasks)
{
    vector<int> result;
    for (auto taskID : topologicalSortedValues)
    {
        for (auto task : tasks)
        {
            if (taskID == task.id)
            {
                if (task.dependencies.empty())
                {
                    result.push_back(taskID);
                    break;
                }
            }
        }
    }
    for (auto taskID : topologicalSortedValues)
    {
        if (find(result.begin(), result.end(), taskID) == result.end())
        {
            result.push_back(taskID);
        }
    }
    return result;
}

// Function to get predecessors and successors of each node in the DAG
pair<vector<unordered_set<int>>, vector<unordered_set<int>>> getPredecessorsAndSuccessors(const vector<Task> &tasks)
{
    int n = tasks.size();
    vector<unordered_set<int>> predecessors(n);
    vector<unordered_set<int>> successors(n);

    // Iterate over each task
    for (const Task &task : tasks)
    {
        int taskId = task.id;

        // Iterate over each dependency of the task
        for (int dependency : task.dependencies)
        {
            // Add the dependency as a predecessor of the task
            predecessors[taskId - 1].insert(dependency);

            // Add the task as a successor of the dependency
            successors[dependency - 1].insert(taskId);
        }
    }

    return make_pair(predecessors, successors);
}

// Function to print predecessors and successors for each node in the DAG
// void printPredecessorsAndSuccessors(const vector<unordered_set<int>> &predecessors, const vector<unordered_set<int>> &successors)
// {
//     int n = predecessors.size();

//     cout << "Node\tPredecessors\tSuccessors" << endl;
//     for (int i = 0; i < n; i++)
//     {
//         cout << i + 1 << "\t";

//         // Print predecessors
//         cout << "[";
//         for (int predecessor : predecessors[i])
//         {
//             cout << predecessor << ", ";
//         }
//         cout << "]\t\t";

//         // Print successors
//         cout << "[";
//         for (int successor : successors[i])
//         {
//             cout << successor << ", ";
//         }
//         cout << "]" << endl;
//     }
// }

// Calculating the criticalPath Time for each task in Bottom Up manner
void calculateCriticalPathTime(vector<int> &betterOrder, vector<Task> &tasks, vector<unordered_set<int>> &successors)
{
    for (int i = betterOrder.size() - 1; i >= 0; i--)
    {
        Task &currTask = tasks[betterOrder[i] - 1];

        // If task has no successor
        if (successors[betterOrder[i] - 1].empty())
        {
            currTask.criticalPathTime = currTask.executionTime;
        }
        else
        {
            int maxSuccessorCriticalPathTime = 0;
            for (const auto &successor : successors[betterOrder[i] - 1])
            {
                Task &successorTask = tasks[successor - 1];
                maxSuccessorCriticalPathTime = max(maxSuccessorCriticalPathTime, successorTask.criticalPathTime);
            }
            currTask.criticalPathTime = currTask.executionTime + maxSuccessorCriticalPathTime;
        }
    }
}

// Printing CPT for every task
void printCriticalPathTime(vector<Task> &tasks)
{
    for (auto task : tasks)
    {
        cout << task.id << " " << task.criticalPathTime << endl;
    }
}

// Finding Critical Path
vector<Task> findCriticalPath(vector<Task> &tasks, vector<unordered_set<int>> &successors)
{
    vector<Task> criticalPath;
    Task currTask = tasks[0];
    for (auto task : tasks)
    {
        if (task.criticalPathTime > currTask.criticalPathTime)
        {
            currTask = task;
        }
    }
    criticalPath.push_back(currTask);

    while (!successors[currTask.id - 1].empty())
    {
        currTask.criticalPathTime = 0;
        for (auto successor : successors[currTask.id - 1])
        {
            if (currTask.criticalPathTime < tasks[successor - 1].criticalPathTime)
            {
                currTask = tasks[successor - 1];
            }
        }
        criticalPath.push_back(currTask);
    }
    return criticalPath;
}

int calculateCmax(vector<Task> &tasks)
{
    int ans = 0;
    for (auto taskID : tasks)
    {
        ans = max(ans, taskID.criticalPathTime);
    }
    return ans;
}

void createPairs(vector<Task> &tasks, priority_queue<pair<int, int>> &readyList)
{
    for (auto task : tasks)
    {
        readyList.push({task.criticalPathTime, task.id});
    }
}

// void printReadyList(priority_queue<pair<int, int>> readyList)
// {
//     while (!readyList.empty())
//     {
//         pair<int, int> readyListElement = readyList.top();
//         readyList.pop();
//         cout << readyListElement.second << " " << readyListElement.first << endl;
//     }
// }
// void printBetterOrder(vector<int> &betterOrder)
// {
//     for (auto i : betterOrder)
//     {
//         cout << i << " ";
//     }
//     cout << endl;
// }

int main()
{
    string filename = "dagFile.txt";

    int M; // Number of processors
    cout << "Enter the number of Processors" << endl;
    cin >> M;

    vector<Task> tasks = readTasksFromFile(filename);
    vector<int> topologicalSortedValues = readLastLineValues(filename);

    vector<int> betterOrder = putIndependentNodesAtFront(topologicalSortedValues, tasks);

    // Find the predecessors and successors of every Node
    pair<vector<unordered_set<int>>, vector<unordered_set<int>>> dagInfo = getPredecessorsAndSuccessors(tasks);
    vector<unordered_set<int>> predecessors = dagInfo.first;
    vector<unordered_set<int>> successors = dagInfo.second;

    // Print predecessors and successors
    // printPredecessorsAndSuccessors(predecessors, successors);

    // Calculate the CPT for each task
    calculateCriticalPathTime(betterOrder, tasks, successors);

    // print Critical Path Time for corresponding to every task
    printCriticalPathTime(tasks);

    // Find the critical Path
    vector<Task> criticalPath = findCriticalPath(tasks, successors);

    // Create  the readyList(PRIORIRY QUEUE)
    priority_queue<pair<int, int>> readyList;
    createPairs(tasks, readyList);
    // printReadyList(readyList);

    vector<tuple<int, int, int, int>> schedule; // (taskID, start time, completionTime, processorNumber)
    vector<int> processorCompletionTimes(M, 0);

    // Selects the tasks from the order
    while (!readyList.empty())
    {
        pair<int, int> process;
        process = readyList.top();
        readyList.pop();

        int taskID = process.second;
        Task currentTask;
        for (const Task &task : tasks)
        {
            if (task.id == taskID)
            {
                currentTask = task;
                break;
            }
        }

        // Just to initialise. All the values get changed as the code moves forward
        int earliestStartTime = 0;
        int selectedProcessor = 0;
        int minCompletionTime = processorCompletionTimes[0];

        // Find the processor with the earliest completion time JO PEHLE KHALI HOGA
        for (int i = 0; i < M; ++i)
        {
            if (processorCompletionTimes[i] < minCompletionTime)
            {
                minCompletionTime = processorCompletionTimes[i];
                selectedProcessor = i;
            }
        }

        // Every process starts only when the processor is allocated
        earliestStartTime = processorCompletionTimes[selectedProcessor];

        // Calculate earliest start time on the selected processor
        for (int dependency : currentTask.dependencies)
        {
            for (const auto &scheduledTask : schedule)
            {
                // If the current tasks depends on some task scheduled, then the start time of the current
                // task is delayed till all dependency tasks are completed
                if (get<0>(scheduledTask) == dependency && get<2>(scheduledTask) > earliestStartTime)
                {
                    earliestStartTime = get<2>(scheduledTask);
                }
            }
        }

        // // Calculate completion time on the selected processor
        int completionTime = earliestStartTime + currentTask.executionTime;

        // Update the processor's completion time
        processorCompletionTimes[selectedProcessor] = completionTime;

        // Add the task ID, completion time, and processor number to the schedule
        schedule.push_back(make_tuple(currentTask.id, earliestStartTime, completionTime, selectedProcessor));
    }

    vector<int> Load(M, 0);

    // Print the schedule in file
    ofstream MyFile("Scheduling.txt");

    for (const auto &scheduledTask : schedule)
    {
        Load[get<3>(scheduledTask)] = Load[get<3>(scheduledTask)] + (get<2>(scheduledTask) - get<1>(scheduledTask));

        MyFile << "Task ID: " << setw(4) << get<0>(scheduledTask)
               << ",\t\t Start Time: " << setw(4) << get<1>(scheduledTask)
               << ",\t\t Completion Time: " << setw(4) << get<2>(scheduledTask)
               << ",\t\t Processor Number: " << setw(4) << get<3>(scheduledTask) + 1
               << endl;
    }

    // Print the critical Path
    MyFile << "\nCritical Path is: Start -> ";
    for (auto taskID : criticalPath)
    {
        MyFile << taskID.id << " -> ";
    }
    MyFile << "Exit" << endl;

    int Cmax = calculateCmax(tasks);
    MyFile << "\nCmax: " << Cmax << endl;

    float averageLoad = 0;
    for (int i = 0; i < Load.size(); i++)
    {
        averageLoad += Load[i];
        MyFile << "\nLoad on Processor " << i + 1 << ": " << Load[i] << endl;
    }

    averageLoad = averageLoad / Load.size();
    for (int i = 0; i < Load.size(); i++)
    {
        MyFile << "\nDifference between Average Load and Load on Processor " << i + 1 << ": " << abs(averageLoad - Load[i]) << endl;
    }

    /////////////////////////////////////// LAST ///////////////////////////////////////////////

    vector<tuple<int, int, int, int,int,int,int,int>> schedule2; // (taskID, start time, completionTime, processorNumber,executionTime, AdjustedExecutionTime)
    vector<pair<int,int>> processorCompletionTimes2(M, {0,0});

    srand((unsigned)time(0));

    // Selects the tasks from the order
    for (auto taskID : betterOrder)
    {

        Task currentTask;
        for (const Task &task : tasks)
        {
            if (task.id == taskID)
            {
                currentTask = task;
                break;
            }
        }

        // Just to initialise. All the values get changed as the code moves forward
        int earliestStartTime = 0;
        int selectedProcessor = 0;
        int minCompletionTime = processorCompletionTimes2[0].first;

        // Find the processor with the earliest completion time JO PEHLE KHALI HOGA
        for (int i = 0; i < M; ++i)
        {
            if (processorCompletionTimes2[i].first < minCompletionTime)
            {
                minCompletionTime = processorCompletionTimes2[i].first;
                selectedProcessor = i;
            }
        }

        // Every process starts only when the processor is allocated
        earliestStartTime = processorCompletionTimes2[selectedProcessor].first;

        // Calculate earliest start time on the selected processor
        for (int dependency : currentTask.dependencies)
        {
            for (const auto &scheduledTask : schedule2)
            {
                // If the current tasks depends on some task scheduled, then the start time of the current
                // task is delayed till all dependency tasks are completed
                if (get<0>(scheduledTask) == dependency && get<2>(scheduledTask) > earliestStartTime)
                {
                    earliestStartTime = get<2>(scheduledTask);
                }
            }
        }
        // Generate random adjustment factor between -25% and 25%
        float adjustmentFactor = (rand() % 51 - 25) / 100.0; // Random number between -0.25 and 0.25
        // cout<<adjustmentFactor<<endl;

        // Calculate adjusted execution time
        int adjustedExecutionTime = static_cast<int>(currentTask.executionTime * (1 + adjustmentFactor));

        // Calculate completion time on the selected processor
        int completionTime = earliestStartTime + adjustedExecutionTime;

        int tentativeEarliestStartTime=0;
        int tentativeCompletionTime=0;
        if(earliestStartTime==0)
            tentativeCompletionTime = earliestStartTime + currentTask.executionTime;
        else
        {
            tentativeEarliestStartTime=processorCompletionTimes2[selectedProcessor].second;
            tentativeCompletionTime=tentativeEarliestStartTime+ currentTask.executionTime;
        }
        // Update the processor's completion time
        processorCompletionTimes2[selectedProcessor].first = completionTime;
        processorCompletionTimes2[selectedProcessor].second = tentativeCompletionTime;

        // Add the task ID, completion time, and processor number to the schedule
        schedule2.push_back(make_tuple(currentTask.id, earliestStartTime, completionTime, selectedProcessor, currentTask.executionTime,adjustedExecutionTime,tentativeEarliestStartTime,tentativeCompletionTime));
    }

    vector<int> Load2(M, 0);

    // Print the schedule in file
    // ofstream MyFile("Scheduling.txt");

    MyFile << "\n";

    int change=0;

    for (int i = 0; i < schedule2.size(); i++)
    {
        Load2[get<3>(schedule2[i])] = Load2[get<3>(schedule2[i])] + (get<2>(schedule2[i]) - get<1>(schedule2[i]));

        change = get<2>(schedule2[i]) - get<7>(schedule2[i]);

        MyFile << "Task ID: " << setw(4) << get<0>(schedule2[i])
               << ",\t\t Tentative Start Time: " << setw(4) << get<6>(schedule2[i])
               << ",\t\t Start Time: " << setw(4) << get<1>(schedule2[i])
               << ",\t\t Tentative Completion Time: " << setw(4) << get<7>(schedule2[i])
               << ",\t\t Completion Time: " << setw(4) << get<2>(schedule2[i])
               << ",\t\t Processor Number: " << setw(4) << get<3>(schedule2[i]) + 1;

        if (change > 0)
        {
            MyFile << ",\t\t Delayed Finish: +" << change;
        }

        else if (change < 0)
        {
            MyFile << ",\t\t Early Finish: " << change;
        }

        else
        {
            MyFile << ",\t\t Timely Finish (NO CHANGE): ";
        }

        MyFile << endl;
    }

    // Print the critical Path
    // MyFile << "\nCritical Path is: Start -> ";
    // for (auto taskID : criticalPath)
    // {
    //     MyFile << taskID.id << " -> ";
    // }
    // MyFile << "Exit" << endl;

    // int Cmax2 = calculateCmax(tasks);
    // MyFile << "\nCmax: " << Cmax << endl;

    if(change>0)
    {
        MyFile << "\nOverall Execution Time Increased by :" <<change<<endl;
    }
    else if(change<0)
    {
         MyFile << "\nOverall Execution Time Decreased by :" <<-change<<endl;
    }
    else
    {
         MyFile << "\nOverall Execution Time Remained Same"<<endl;
    }

    float averageLoad2 = 0;
    for (int i = 0; i < Load2.size(); i++)
    {
        averageLoad2 += Load2[i];
        MyFile << "\nLoad on Processor " << i + 1 << ": " << Load2[i] << endl;
    }

    averageLoad2 = averageLoad2 / Load2.size();
    for (int i = 0; i < Load2.size(); i++)
    {
        MyFile << "\nDifference between Average Load and Load on Processor " << i + 1 << ": " << abs(averageLoad2 - Load2[i]) << endl;
    }

    MyFile.close();

    return 0;
}
