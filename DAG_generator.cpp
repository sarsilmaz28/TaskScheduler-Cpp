#include <bits/stdc++.h>

using namespace std;

void DFS(int v, vector<bool> &visited, stack<int> &s, unordered_map<int, list<int>> &graph) {
    visited[v] = true;

    for (int neighbor : graph[v]) {
        if (!visited[neighbor]) {
            DFS(neighbor, visited, s, graph);
        }
    }

    s.push(v);
}


vector<int> topologicalSort(int no_of_vert, unordered_map<int, list<int>> &graph) {
    vector<bool> visited(no_of_vert + 1, false);
    stack<int> s;

    for (int i = 1; i <= no_of_vert; i++) {
        if (!visited[i]) {
            DFS(i, visited, s, graph);
        }
    }

    vector<int> result;
    while (!s.empty()) {
        result.push_back(s.top());
        s.pop();
    }

    return result;
}




// Function to check for a cycle when a new edge is added to the graph
bool CheckAcyclic(const vector<pair<int, int>> &edge, int ed, vector<bool> &check, int v)
{
    int i;
    bool value;

    // If the current vertex is visited already, then the graph contains cycle.
    if (check[v] == true)
    {
        return false;
    }
    else
    {
        check[v] = true;

        // for every vertex, check for all vertices connected to it and check if there is a cycle
        for (i = ed; i >= 0; i--)
        {
            if (edge[i].first == v)
            {
                return CheckAcyclic(edge, ed, check, edge[i].second);
            }
        }
    }
    // if the path ends then, remove the current edge from visited path i.e., reassign the vertexes visited in that path to false again.
    check[v] = false;

    // if its the only edge, return true
    if (i == 0)
        return true;
}

// to check if there is any isolated edge.
bool checkIsolated(vector<pair<int, int>> &edge, int i)
{
    // cout << "HELLO " << i << endl;
    for (int j = 0; j < edge.size(); j++)
    {
        if (edge[j].first == i)
            return false;
        if (edge[j].second == i)
            return false;
    }
    return true;
}


void writeToFile(vector<pair<int, int>> &edge,int no_of_vert,vector<int> &sortedVertices)
{
    ofstream MyFile("dagFile.txt");
    for (int i = 0; i < no_of_vert; i++) {

       
        MyFile<< i + 1<<" "<<rand() % 30 + 2<<" ";
         
        vector<int> dependencies;

        for (int j = 0; j < edge.size(); j++) {
            if (edge[j].second == i+1) {
                dependencies.push_back(edge[j].first);
            }
        }

        for(auto i:dependencies)
        {
            MyFile<<i<<" ";
        }
        MyFile<<"\n";
    }
     // Printing the sorted vertices
    MyFile<< "Topological Sort: \n";
    for (int v : sortedVertices) {
        MyFile << v << " ";
    }
    MyFile.close();
}


void GenerateRandGraphs(int no_of_vert )
{
    int i, j, count;
    vector<pair<int, int>> edge(no_of_vert/2);
    unordered_map<int, list<int>> mp;
    unordered_map<int, list<int>> graph;
    vector<bool> check(no_of_vert + 1);

    i = 0;
    srand((unsigned)time(0));
    while (i < no_of_vert/2)
    {
        // Build an edge between two random vertices
        edge[i].first = rand() % no_of_vert + 1;
        edge[i].second = rand() % no_of_vert + 1;

        for (j = 1; j <= no_of_vert; j++)
            check[j] = false;

        // check if adding the edge created a cycle . If created, discard the edge and recreate the edge. Else move forward
        if (CheckAcyclic(edge, i, check, edge[i].first) == true)
            i++;
    }

    // After graph is created, we check if there is an isolated edge.
    for (int i = 1; i <= no_of_vert; i++)
    {
        if (checkIsolated(edge, i))
        {
            int connected_vertex = rand() % no_of_vert + 1; // connect the isolated edge to a random vertex from the graph

            while (connected_vertex == i)
            {
                connected_vertex = rand() % no_of_vert + 1;
            }
            
            edge.push_back({i, connected_vertex});

            // no_of_vert++;
        }
    }

    // Printing the edge matrix
    cout << "Printing edges" << endl;
    for (int i = 0; i < edge.size(); i++)
    {
        cout << edge[i].first << " " << edge[i].second << endl;
    }

       for (int i = 0; i < edge.size(); i++) {
        int u = edge[i].first;
        int v = edge[i].second;
        graph[u].push_back(v);
    }

    // Perform topological sort
    vector<int> sortedVertices = topologicalSort(no_of_vert, graph);


    // Writing the output in dot file
    ofstream MyFile("DAG.dot");

    MyFile << "digraph DAG {";

    for (i = 0; i < no_of_vert; i++)
    {
        count = 0;
        MyFile << "\n\t" << i + 1 << "-> { ";
        // cout << "\n\t" << i + 1 << "-> { ";

        for (j = 0; j < edge.size(); j++)
        {
            if (edge[j].first == i + 1)
            {
                // to avoid self loops
                if (find(mp[i + 1].begin(), mp[i + 1].end(), edge[j].second) == mp[i + 1].end())
                {
                    // cout << edge[j].second << "  ";
                    MyFile << edge[j].second << " ";
                    mp[i + 1].push_back(edge[j].second);
                    count++;
                }
            }
            else if (edge[j].second == i + 1)
            {
                count++;
            }
        }
        // cout << " }" << endl;
        MyFile << " }" << endl;
    }
    MyFile << "}";
    MyFile.close();

    writeToFile(edge,no_of_vert,sortedVertices);
}



int main()
{
    int no_of_vert;

    cout << "Enter the number of TASKS: ";
    cin >> no_of_vert;
    // cout << "Enter the number of edges for the random graph: ";
    // cin >> e;
    if (no_of_vert == 1)
    {
        ofstream MyFile("DAG.dot");

        MyFile << "digraph DAG {\n";
        MyFile << "1;" << endl;
        MyFile << "}";
        MyFile.close();
        return 0;
    }

    GenerateRandGraphs(no_of_vert);

    return 0;
}
