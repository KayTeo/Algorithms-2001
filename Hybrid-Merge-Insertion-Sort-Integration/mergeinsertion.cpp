#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <ctime>
#include<fstream>
#include <chrono>

using namespace std;
int s = 3;
int keyComparisons = 0;

void insertionSort(int arr[],int n)
{
    int i, key, j;
    for(i=1;i<n;i++)
    {
        //To set the position of key and set j to 1 position before key so that we can compare and see
        //which is the smaller value, hence swap when necessary
        key=arr[i];
        j=i-1;

        //Move elements of arr[], that are greater than key, to one position ahead of their current position
        //This will loop until values are being placed properly hence causing arr[j]>key to be false
        while(j >= 0 && arr[j]>key)
        {
            arr[j+1]=arr[j];  
            j=j-1; 
            keyComparisons++;
        }
        //if(j>=0 && arr[j]<key)
            //keyComparisons++;
        
        arr[j+1]=key;
    }
    //cout<<count<<"\n";
}

//Code for in place merging of two subarrays
//array is divided by pointers, start to mid and mid + 1 to end
void merge(int nums[], int start, int mid, int end){

    //Calculate size and declare var for copies of left and right subarrays
    int leftSize = mid - start + 1;
    int rightSize = end - mid;

    int *left = new int[leftSize];
    int *right = new int[rightSize];

    //Copy left subarray into left
    for(int leftC = 0; leftC < leftSize; leftC++){
        left[leftC] = nums[start+leftC];
    }

    //Copy right subarray into right
    for(int rightC = 0; rightC < rightSize; rightC++){
        right[rightC] = nums[mid +  1 + rightC];
    }

    //Start of merging, three counters for left, right and complete arrays
    int mainC = start;
    int leftC = 0;
    int rightC = 0;

    while(leftC < leftSize && rightC < rightSize){
        if(left[leftC] < right[rightC]){
            nums[mainC] = left[leftC];
            leftC++;
        }
        else{
            nums[mainC] = right[rightC];
            rightC++;
        }
        mainC++;
        keyComparisons++;
    }

    //If there is "leftover" elements not inserted in left or right subarrays
    while(leftC < leftSize){
        nums[mainC] = left[leftC];
        leftC++;
        mainC++;
    }
    while(rightC < rightSize){
        nums[mainC] = right[rightC];
        rightC++;
        mainC++;
    }
}

//mergeSort algorith, start and end are indexes e.g. start at 0 and end at 4 to sort the whole of a 5 element array
//Uses arrays ie pass by reference, so changes to nums is changed everywhere and no return value
void mergeSort(int nums[], int start, int end){
    int holder;

    //Base case termination
    if(start == end){
        return;
    }

    int size = end - start;
    int mid = start + (end - start) / 2;

    mergeSort(nums, start, mid);
    mergeSort(nums, mid + 1, end);

    merge(nums, start, mid, end);
}



void combineSort(int nums[], int start, int end){
    int holder;

    //Base case termination
    if(start == end){
        return;
    }

    int size = end - start;
    int mid = start + (end - start) / 2;

    //Calculate sizes of left and right array for insertionSort function
    int leftSize = mid - start + 1;
    int rightSize = end - mid;

    //Use insertion sort if array is equal to or smaller than s
    if( leftSize <= s){
        //printf("Insertion sort used\n");
        insertionSort(nums + start, leftSize);
    }
    else{
        combineSort(nums, start, mid);
    }

    if(rightSize <= s){
        insertionSort(nums + mid + 1, rightSize);
    }
    else{
        combineSort(nums, mid + 1, end);
    }

    merge(nums, start, mid, end);
}

void printArray(int arr[],int n)
{
    int i;
    for(i=0;i<n;i++)
    {
        cout<<arr[i]<<" ";
    }
    cout<<endl;
}

//Generates number in range
//Note: use srand only once in main as using srand withint 1 second of each other in different levels of code sets seed to the same value
int generateVal(int lower, int upper){

    //include upper
    upper++;
    int r = (rand() % upper) + lower; //Generate number between 1000-10million
    //printf("Generate val is %d\n", r);
    return r;
}

//Returns an array (pointer to int) of size lower to upper inclusive
int* randomArray(int arraySize, int lowerVal, int upperVal)
{
    //cout<<"The array size is ";
    cout<<arraySize<<"\n";

    int* randArr = new int[arraySize];

    for(int i=0;i<arraySize;i++)
    {
        randArr[i]=(rand() % upperVal) + lowerVal + 1; //Generate number between range lowerVal to upperVal
    }

    //printf("Random array is: ");
    //printArray(randArr,arraySize);
    return randArr;
}

void makeCopy(int* nums1, int* nums2, int size){
    for(int i = 0; i < size; i++){
        nums1[i] = nums2[i];
    }
}

//tries is the number of arrays it will generate to sort
//threshold is the s value below which insertion instead of mergesort will be used.
//Note that the value is stored as a global var, and this just changes it
void testSort(int tries, int threshold, int minVal, int maxVal, int minRange, int maxRange, string fileName){
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;
    ofstream myfile;
    myfile.open (fileName);

    s = threshold;
    int arrSize;
    myfile <<  "s value, combineSort time, mergeSort time\n";
    for(int count = 0; count < tries; count++){
        int arrSize = generateVal(minRange, maxRange);
        //printf("Generated length is: %d\n", arrSize);
        int* randomCombineArr = randomArray(arrSize, minVal, maxVal);
        int* randomMergeArr = new int[arrSize];
        makeCopy(randomMergeArr, randomCombineArr, arrSize);
        auto t1 = high_resolution_clock::now();
        combineSort(randomCombineArr, 0, arrSize - 1);
        auto t2 = high_resolution_clock::now();
        duration<double, milli> combineTime = t2 - t1;

        auto t3 = high_resolution_clock::now();
        mergeSort(randomMergeArr, 0, arrSize - 1);
        auto t4 = high_resolution_clock::now();
        duration<double, milli> mergeTime = t4 - t3;

        myfile << s << ", " << combineTime.count() << ", " << mergeTime.count() << "\n";
    }

    
}

void testSortRange(int tries, int threshold, int minVal, int maxVal, int minRange, int maxRange, string fileName){
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;
    ofstream myfile;
    myfile.open (fileName);
    s = threshold;
    int arrSize;
    myfile <<  "s value, combineSort time, mergeSort time\n";

    //Alter global var s to change subarray threshold
    for(; s <= threshold; s++){
        for(int count = 0; count < tries; count++){
            int arrSize = generateVal(minRange, maxRange);
            printf("Generated length is: %d\n", arrSize);
            int* randomCombineArr = randomArray(arrSize, minVal, maxVal);
            int* randomMergeArr = new int[arrSize];
            makeCopy(randomMergeArr, randomCombineArr, arrSize);
            auto t1 = high_resolution_clock::now();
            combineSort(randomCombineArr, 0, arrSize - 1);
            auto t2 = high_resolution_clock::now();
            duration<double, milli> combineTime = t2 - t1;

            auto t3 = high_resolution_clock::now();
            mergeSort(randomMergeArr, 0, arrSize - 1);
            auto t4 = high_resolution_clock::now();
            duration<double, milli> mergeTime = t4 - t3;

            myfile << s << ", " << combineTime.count() << ", " << mergeTime.count() << "\n";
        }
    }   

    
}

void testSortRangeAverage(int tries, int threshold, int minVal, int maxVal, int minRange, int maxRange, string fileName){
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;
    ofstream myfile;
    myfile.open (fileName);
    s = threshold;
    int arrSize;
    myfile <<  "s value, combineSort time, mergeSort time\n";
    double combineSum = 0;
    double mergeSum = 0;
    //Alter global var s to change subarray threshold
    for(; s <= threshold; s++){
        for(int count = 0; count < tries; count++){
            int arrSize = generateVal(minRange, maxRange);
            printf("Generated length is: %d\n", arrSize);
            int* randomCombineArr = randomArray(arrSize, minVal, maxVal);
            int* randomMergeArr = new int[arrSize];
            makeCopy(randomMergeArr, randomCombineArr, arrSize);
            auto t1 = high_resolution_clock::now();
            combineSort(randomCombineArr, 0, arrSize - 1);
            auto t2 = high_resolution_clock::now();
            duration<double, milli> combineTime = t2 - t1;

            auto t3 = high_resolution_clock::now();
            mergeSort(randomMergeArr, 0, arrSize - 1);
            auto t4 = high_resolution_clock::now();
            duration<double, milli> mergeTime = t4 - t3;

            combineSum += combineTime.count();
            mergeSum += mergeTime.count();
            printf("Arrsize: %d\n", arrSize);
        }
        printf("combineSum: %f, mergeSum: %f, arrSize: %d\n", combineSum, mergeSum, arrSize);

        //Get the average
        combineSum /= arrSize;
        mergeSum /= arrSize;
        printf("combineSum: %f, mergeSum: %f, arrSize: %d\n", combineSum, mergeSum, arrSize);
        myfile << s << ", " << combineSum << ", " << mergeSum << "\n";
    }      
}

void testSortComparisons(int tries, int threshold, int minVal, int maxVal, int minRange, int maxRange, string fileName){

    ofstream myfile;
    myfile.open (fileName);
    s = threshold;
    int arrSize;
    myfile <<  "s value, combineSort comparisons, mergeSort comparisons\n";
    int combineKeys = 0;
    int mergeKeys = 0;
    int combineKeysSum = 0;
    int mergeKeysSum = 0;
    //Alter global var s to change subarray threshold
    for(; s <= threshold; s++){
        for(int count = 0; count < tries; count++){
            int arrSize = generateVal(minRange, maxRange);
            printf("Generated length is: %d\n", arrSize);
            int* randomCombineArr = randomArray(arrSize, minVal, maxVal);
            int* randomMergeArr = new int[arrSize];
            makeCopy(randomMergeArr, randomCombineArr, arrSize);

            keyComparisons = 0;
            combineSort(randomCombineArr, 0, arrSize - 1);
            combineKeysSum += keyComparisons; 

            keyComparisons = 0;
            mergeSort(randomMergeArr, 0, arrSize - 1);
            mergeKeysSum += keyComparisons;

        }
        printf("combineKeySum: %d, mergeKeySum: %d, arrSize: %d\n", combineKeysSum, mergeKeysSum, arrSize);

        //Get the average
        printf("combineKeySum: %d, mergeKeySum: %d, arrSize: %d\n", combineKeysSum, mergeKeysSum, arrSize);
        myfile << s << ", " << combineKeysSum << ", " << mergeKeysSum << "\n";
    }      
}

void testSortComparisonSingle(int tries, int threshold, int minVal, int maxVal, int length, string fileName){

    ofstream myfile;
    myfile.open (fileName);
    s = threshold;
    int arrSize;
    myfile <<  "s value, combineSort comparisons, mergeSort comparisons\n";
    int combineKeys = 0;
    int mergeKeys = 0;
    int combineKeysSum = 0;
    int mergeKeysSum = 0;
    for(int count = 0; count < tries; count++){
        //printf("Length is: %d\n", length);
        int* randomCombineArr = randomArray(length, minVal, maxVal);
        int* randomMergeArr = new int[length];
        makeCopy(randomMergeArr, randomCombineArr, length);
        keyComparisons = 0;
        combineSort(randomCombineArr, 0, length - 1);
        combineKeysSum += keyComparisons; 

        keyComparisons = 0;
        mergeSort(randomMergeArr, 0, length - 1);
        mergeKeysSum += keyComparisons;
    }

        //Get the average
        //printf("combineKeySum: %d, mergeKeySum: %d, arrSize: %d\n", combineKeysSum, mergeKeysSum, arrSize);
        myfile << s << ", " << combineKeysSum << ", " << mergeKeysSum << "\n";
}

void testSortTimeSingle(int tries, int threshold, int minVal, int maxVal, string fileName){
    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;
    ofstream myfile;
    myfile.open (fileName);
    int arrSize;
    myfile <<  "s value, combineSort time, mergeSort time\n";

    s = threshold;
    //Alter global var s to change subarray threshold
    for(int count = 0; count < tries; count++){
        int arrSize = 10000000;
        int* randomCombineArr = randomArray(arrSize, minVal, maxVal);
        int* randomMergeArr = new int[arrSize];
        makeCopy(randomMergeArr, randomCombineArr, arrSize);
        auto t1 = high_resolution_clock::now();
        combineSort(randomCombineArr, 0, arrSize - 1);
        auto t2 = high_resolution_clock::now();
        duration<double, milli> combineTime = t2 - t1;

        auto t3 = high_resolution_clock::now();
        mergeSort(randomMergeArr, 0, arrSize - 1);
        auto t4 = high_resolution_clock::now();
        duration<double, milli> mergeTime = t4 - t3;

          myfile << s << ", " << combineTime.count() << ", " << mergeTime.count() << "\n";
    }


    
}

int main(){
    srand(time(NULL)); //Put this once in main to ensure that the number is different everything the code runs

    //testSortComparisonSingle(1, 8, 0, 10000000, 10000000, "test.csv");
    testSortTimeSingle(1, 6, 0, 10000000, "timeTest1.txt");

    /*
    for(int i = 0; i < 10; i++){
        int arrSize = generateVal(1, 20);
        printf("Random num is %d: \n", arrSize);
        int* randomArr = randomArray(arrSize, 1, 50);
        combineSort(randomArr, 0, arrSize - 1);
        printf("Sorted array is: ");
        printArray(randomArr, arrSize);
        printf("\n");
    }*/

    //get the random values of array size and variables in it
    //randomArray();
    return 0;
}
