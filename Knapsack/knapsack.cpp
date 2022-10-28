#include<iostream>
#include<vector>

using namespace std;

//Capacity
vector<int> weight;
vector<int> profit;
int c = 100; //capacity
vector<vector<int>> dpMatrix;

//wIndex and pIndex tracks last element of knapsack vectors
int knapsackTopDown(int item, int weightAccumulated, int profitAccumulated){
    //TODO: Write base case
    cout << "Considering item " << item << " with accumulated weight " << weightAccumulated << "\n";

    //If weight exceeds without adding current element
    if(weightAccumulated > c){
        cout << "Weight accumulated exceeded with : " << weightAccumulated << "\n";
        return -1;
    }

    //At last element, check if adding it will exceed capacity
    else if(item == 0){

        //Do not add last item
        if(weightAccumulated + weight[0] > c){
            return profitAccumulated;
        }

        //Add last item
        else{
            return profitAccumulated + profit[0];
        }
    }



    cout << "Splitting...\n";

    //TODO: Finish recursion
    //Returns which is bigger - the knapsack without the current item or the knapsack with
    return max(

                knapsackTopDown(item - 1, weightAccumulated, profitAccumulated),
                knapsackTopDown(item - 1, weightAccumulated + weight[item], profitAccumulated + profit[item])
    );
}

void knapsackBottomUp(int maxWeight, int elements){
    int include;
    int pass;

    for(int w = 0; w < maxWeight; w++){
        //First row of graph
        //Considering only the first item
        //If it does not exceed weight capacity, include it in knapsack by setting profit 
        dpMatrix[0][w] = weight[0] <= w ? profit[0] : 0;
    }

    for(int ith = 1; ith < elements; ith++){
        for(int w = 0; w < maxWeight; w++){
            

            if(weight[ith] <= w){
                include = profit[ith] + dpMatrix[ith - 1][w - weight[ith]];
            }
            else{
                include = -1;
            }

            //Don't include ith element, take profit from previous 
            pass = dpMatrix[ith - 1][w];
            dpMatrix[ith][w] = max(include, pass);

        }
    }
}

int main(){

    int elements = 3;
    int maxWeight = 10;
    c = maxWeight; 

    dpMatrix.resize(elements);
    for(int i = 0; i < elements; i++){
        dpMatrix[i].resize(maxWeight);
    }

    weight = {1, 2, 3};
    profit = {10, 15, 40};

    knapsackBottomUp(maxWeight, elements);
    /*
    int test = knapsackTopDown(weight.size() - 1, 0, 0);*/

    //cout << "\n\nFinal answer: " << test;

    for(int i = 0; i < dpMatrix.size(); i++){
        for(int j = 0; j < dpMatrix[i].size(); j++){
            cout << dpMatrix[i][j] << " ";
        }
        cout << "\n";
    }
    return 0;
}