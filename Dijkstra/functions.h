#include<cstdio>
#include <vector>
#include<iostream>
#include <numeric>

using namespace std;

// returns random number in range [min, max]
int random(int min, int max){
    static bool first = true;
    if (first){
        srand(time(NULL));
        first = false;
    }
    return min + rand() % ((max + 1) - min);
}

// returns current time in microseconds
uint64_t getTime(){
    return chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now().time_since_epoch()).count();
}

class AdjacencyList{
public:
    //adjaency list class
    AdjacencyList(int **matrix, int V){
        for (int i = 0; i < V; i++){
            vector<int *> subList;
            list.push_back(subList);

            for (int j = 0; j < V; j++){

                //Add the edges
                if (matrix[i][j] != 0){
                    int *tmp = (int *)malloc(sizeof(int) * 2);
                    tmp[0] = j;
                    tmp[1] = matrix[i][j];
                    list[i].push_back(tmp);
                }
            }
        }
    }
    //list data
    vector<vector<int *>> list;
};

class PriorityQueueArray{
public:
    PriorityQueueArray(){};

    //Add edge and vertices
    void add(int vertex, int distance){
        int *tmp = new int[2]{vertex, distance};
        queue.push_back(tmp);
    }

    //Edge weight change
    void edit(int vertex, int newDistance){
        // loop through the queue to find the vertex and update the currently known distance
        for (int i = 0; i < queue.size(); i++){
            if (queue[i][0] == vertex){
                queue[i][1] = newDistance;
                break;
            }
        }
    }

    // returns [vertex, distance] of the shortest distance pair
    int *pop(){
        // default shortest vertex is 0
        // loop through queue, set smallest index and vertex if needed
        int smallestIndex = 0, *smallestPair = queue[0];
        for (int i = 0; i < queue.size(); i++){
            if (queue[i][1] < queue[smallestIndex][1]){
                smallestIndex = i;
                smallestPair = queue[i];
            }
        }
        // removes vertex from queue
        queue.erase(queue.begin() + smallestIndex);
        return smallestPair;
    }
    bool isEmpty(){
        return queue.size() == 0;
    }

protected:
    //vector queue
    vector<int *> queue;
};

class PriorityQueueMinHeap{
public:
    //empty constructor
    PriorityQueueMinHeap(){};

    //get parent
    int parent(int i) { return (i - 1) / 2; }

    // get left child of ith node
    int left(int i) { return (2 * i + 1); }

    // get right child of ith node
    int right(int i) { return (2 * i + 2); }

    // checks if ith node is a leaf
    bool isLeaf(int i) { return 2 * i >= (_heap.size() - 1) && i < _heap.size(); }

    // checks if ith node has left child
    bool hasLeft(int i) { return left(i) < _heap.size(); }

    // checks if ith node has right child
    bool hasRight(int i) { return right(i) < _heap.size(); }

	// swap nodes at i and j
    void swap(int i, int j){
        int tmp0 = _heap[i][0],
            tmp1 = _heap[i][1];

        _heap[i][0] = _heap[j][0];
        _heap[i][1] = _heap[j][1];

        _heap[j][0] = tmp0;
        _heap[j][1] = tmp1;
    }

    // sorts itself upwards from i
    void heapifyUp(int i){
        // checks up parent distance is greater than current index
        while (i != 0 && _heap[parent(i)][1] > _heap[i][1])
        {
            // if so, swap and change current index to its parent since its shifted
            swap(i, parent(i));
            i = parent(i);
        }
    }

    // sorts itself downwards from i
    void heapifyDown(int i){
        // keep swapping if node has children
        while (hasLeft(i)){
            int smallestChildIndex = -1;
            // if has children, it WILL have a left child
            if (hasLeft(i)){
                int leftChild = left(i);
                // it may not have a right child
                if (hasRight(i)){
                    int rightChild = right(i);
                    // get the smaller of the left and right children
                    smallestChildIndex = (_heap[leftChild][1] <= _heap[rightChild][1]) ? leftChild : rightChild;
                }
                // no right child
                else
                    smallestChildIndex = leftChild;
            }
            // no children at all
            else
                break;
            if (_heap[i][1] > _heap[smallestChildIndex][1]){
                swap(i, smallestChildIndex);
                i = smallestChildIndex;
            }
            // when the smallest child less or equals to i
            else
                break;
        }
    }

    // adds vertex and its distance from source into the priority queue
    void add(int vertex, int distance){
        // inserts vertex, distance pair into heap
        int *tmp = new int[2]{vertex, distance};
        _heap.push_back(tmp);

        // sorts itself upwards
        heapifyUp(_heap.size() - 1);
    }

    // changes the distance of vertex from the source
    void edit(int vertex, int newDistance){
        // loop through the queue to find the vertex and update the distance
        for (int i = 0; i < _heap.size(); i++){
            if (_heap[i][0] == vertex){
                int oldDistance = _heap[i][1];
                _heap[i][1] = newDistance;

                // if new distance if somehow larger than old distance,
                // no choice but to sort itself downwards
                if (newDistance > oldDistance)
                    heapifyDown(i);
                else
                    heapifyUp(i);
                break;
            }
        }
    }

    // returns [vertex, distance] of the shortest distance pair
    int *pop(){
        if (_heap.size() == 0)
            return NULL;

        int *smallest = new int[2]{_heap[0][0], _heap[0][1]};
        _heap.erase(_heap.begin());

        // edge case if size = 0 after erasing
        if (_heap.size() > 1){
            swap(0, _heap.size() - 1);
            heapifyDown(0);
        }
        return smallest;
    }

    bool isEmpty(){
        return _heap.size() == 0;
    }

    void printHeap(){
        for (int i = 0; i < _heap.size(); i++){
            int *tmp = _heap[i];
            cout << "(" << tmp[0] << "," << tmp[1] << ") ";
        }
        cout << endl;
    }

protected:
	//heap implemented as a vector (of pairs) 
    //tree structure stored in vector
    vector<int *> _heap;
};
