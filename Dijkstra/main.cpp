#include <numeric>
#include <chrono>
#include <fstream>
#include "functions.h"

//Number of trials to run
int trials = 100;

//Probability of two vertices being connected, edge_prob / 100
int edge_prob = 10;

using namespace std;

// generates a graph with number of vertices Ve
int createGraph(int **graph, int Ve){
    int edges = 0;

    // allocating memory for graph
    for (int i = 0; i < Ve; i++)
        graph[i] = (int *)malloc(sizeof(int) * Ve);

    for (int i = 0; i < Ve; i++){
        for (int j = i; j < Ve; j++){
            if (i == j){
                graph[i][j] = 0;
            }

            else{
                // roll dice
                int chance = random(0, 100);

                // chance of edge existing
                // number of edges total is approximately
                // V(V+1)/2 * (edge_prob/100)
                if (chance <= edge_prob){
                    // randomises cost for edge
                    int dist = random(4, 20);
                    // sets cost for V1->V2 and V2->V1
                    graph[i][j] = dist;
                    graph[j][i] = dist;
                    //cout << "Test\n";
                    edges++;
                }
                else{
                    // no edge between V1 and V2
                    graph[i][j] = 0;
                    graph[j][i] = 0;
                }
            }
        }
    }
    return edges;
}

uint64_t dijkstra1(int **graph, int Ve, int source){
    uint64_t start = getTime();

    //shortest distance of vertex from source
    int* d = new int[Ve];

    //parent of vertex on shortest path
    int* pi = new int[Ve];

    //visited vertices
    bool* visited = new bool[Ve];

    //priority queue for getting next shortest distance
    PriorityQueueArray prioQueue;

    for (int i = 0; i < Ve; i++){
        // distance of source from source is 0
        if (i == source)
            d[i] = 0;
        else
            d[i] = INT32_MAX;

        pi[i] = -1;
        visited[i] = false;

        prioQueue.add(i, d[i]);
    }

    //while priority queue has vertices
    while (!prioQueue.isEmpty()){
        int *pair = prioQueue.pop();
        int u = pair[0], dist = pair[1];

        visited[u] = true;

        //loops through distance for all vertices from u by looking at the adj matrix
        //to keep track of edges
        for (int i = 0; i < Ve; i++){
            int vertex = i;
            int distanceFromU = graph[u][i];

            // if vertex is connected, hasn't been visited and the (distance of u from source) + (distance of vertex from u)
            // is less than current distance of vertex from source
            if (distanceFromU != 0 && !visited[i] && (d[u] + distanceFromU) < d[i])
            {
                // gets the shorter distance and updates the priority queue, distance array and parent array
                int shorterDistance = d[u] + distanceFromU;
                d[i] = shorterDistance;
                pi[i] = u;
                prioQueue.edit(i, shorterDistance);
            }
        }
    }

    uint64_t timeTaken = getTime() - start;
    return timeTaken;
}

uint64_t dijkstra2(AdjacencyList Adj, int Ve, int source){
    uint64_t start = getTime();

    // d keeps track of shortest distance of vertex from source
    // pi keeps track of parent of vertex
    int* d = new int[Ve];
    int* pi = new int[Ve];

    // visited keeps track of whether vertex has been completely visited
    bool* visited = new bool[Ve];

    // our priority queue for getting next shortest distance
    PriorityQueueMinHeap prioQueue;

    for (int i = 0; i < Ve; i++){
        // distance of source from source is 0
        if (i == source)
            d[i] = 0;
        else
            d[i] = INT32_MAX;

        pi[i] = -1;
        visited[i] = false;

        prioQueue.add(i, d[i]);
    }

    long debugB = 0;

    // continue until priority queue is empty, i.e. no more vertices to explore
    while (!prioQueue.isEmpty()){
        int *pair = prioQueue.pop();
        int u = pair[0], dist = pair[1];

        visited[u] = true;

        //loops through distance for all vertices from u by looking at the adj list
        //should be more efficient than our adj matrix
        for (int i = 0; i < Adj.list[u].size(); i++)
        {
            int *edge = Adj.list[u][i];
            int vertex = edge[0], distanceFromU = edge[1];

            if (!visited[vertex] && (d[u] + distanceFromU) < d[vertex])
            {
                debugB++;
                int shorterDistance = d[u] + distanceFromU;
                d[vertex] = shorterDistance;
                pi[vertex] = u;
                prioQueue.edit(vertex, shorterDistance);
            }
        }
    }

    uint64_t timeTaken = getTime() - start;
    return timeTaken;
}

int main(){

    ofstream outfile;
    outfile.open("result.txt", ios_base::app);
    outfile << "avgVertices" << "\t" << "avgEdges" << "\t" << "adjTime" << "\t" << "listTime" << endl;
    outfile.close();

    for (int Ve = 50; Ve <= 1000; Ve += 50){
        vector<uint64_t> ATimes, BTimes;
        vector<int> edgeCount;

        for (int i = 0; i < trials; i++){
            cout << i << " ";
            // our graph in adj matrix form
            int **graph = new int *[Ve];

            // generates and stores the graph
            int edges = createGraph(graph, Ve);

            // our graph in adj list form
            // using double pointer because 2d arrays can't be passed as argument
            int **tmp = new int *[Ve];
            for (int i = 0; i < Ve; i++)
                tmp[i] = graph[i];
            AdjacencyList Adj(tmp, Ve);

            int source = 0;

            // dijkstra functions return the time taken in microseconds
            uint64_t timeTakenA = dijkstra1(graph, Ve, source);
            uint64_t timeTakenB = dijkstra2(Adj, Ve, source);

            // append to vectors
            ATimes.push_back(timeTakenA);
            BTimes.push_back(timeTakenB);
            edgeCount.push_back(edges);

            // for some reason this isn't deallocating variables
            // properly so the program takes up a metric fuck ton of memory
            // no joke
            for (int i = 0; i < Ve; i++)
            {
                delete graph[i];
            }

            delete graph;
            delete tmp;
        }

        uint64_t avgA = (std::accumulate(ATimes.begin(), ATimes.end(), 0) / trials);
        uint64_t avgB = (std::accumulate(BTimes.begin(), BTimes.end(), 0) / trials);
        int avgEdges = (std::accumulate(edgeCount.begin(), edgeCount.end(), 0) / trials);

        cout << "\nFor " << trials << " trials:\nAdjacency matrix + priority queue average time:\t" << avgA << " microseconds"
             << "\nAdjacency list + minimizing heap average time:\t" << avgB << " microseconds" << endl;
        cout << "Average number of Vertices: " << Ve << "\nAverage number of Edges: " << avgEdges << endl;

        ofstream outfile;
        outfile.open("result.txt", ios_base::app);
        outfile << Ve << "\t" << avgEdges << "\t" << avgA << "\t" << avgB << endl;
        outfile.close();
    }
}