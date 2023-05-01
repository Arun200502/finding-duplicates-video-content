#include <iostream>
#include <omp.h>
#include<ctime>
#include <fstream>
#include <bits/stdc++.h>
#include<stdio.h>

using namespace std;

void insertionSort(int arr1[], int arr2[], int low, int high) {
    for (int i = low + 1; i <= high; i++) {
        int key1 = arr1[i];
        int key2 = arr2[i];
        int j = i - 1;
        for (j; j >= low && (arr1[j] > key1 || arr2[j]>key2); j--) {
            arr1[j + 1] = arr1[j];
            arr2[j + 1] = arr2[j];
        }
        arr1[j + 1] = key1;
        arr2[j + 1] = key2;
    }
}

int partition(int arr1[], int arr2[], int low, int high,int k) {
    int pivot = arr1[high];
    int pivotTimeStamp = arr2[high];
    int i = low - 1;
    int t=2;
    if(k>20 && k<64)
    {
        t=4;
    }
    else
    {
        t=64;
    }
    #pragma omp parallel for shared(arr1, arr2) num_threads(t)
    for (int j = low; j <= high - 1; j++) {
        if (arr1[j] < pivot || (arr1[j] == pivot && arr2[j] < pivotTimeStamp )) {
            i++;
            swap(arr1[i], arr1[j]);
            swap(arr2[i], arr2[j]);
        }
    }
    swap(arr1[i + 1], arr1[high]);
    swap(arr2[i + 1], arr2[high]);
    return (i + 1);
}

void hybridQuickSort(int arr1[], int arr2[], int low, int high, int k) {
    if (high - low + 1 <= k) {
        insertionSort(arr1, arr2, low, high);
        return;
    }

    int pi = partition(arr1, arr2, low, high,k);

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            hybridQuickSort(arr1, arr2, low, pi - 1, k);
        }
        #pragma omp section
        {
            hybridQuickSort(arr1, arr2, pi + 1, high, k);
        }
    }
}

int tp(int arr[],int low,int high,int arr1[])
{
	int size = low;
    for (int i = low+1; i <=high ; i++) {
        if (arr1[i] != arr1[size]) {
        	swap(arr1[size+1],arr1[i]);
        	swap(arr[size+1],arr[i]);
            size++;
        }
    }
    return size;
}
int twopointer(int arr[],int low,int high,int arr1[])
{
    int mid=(high-low)/2;
    int t1;
    int t2;
    int t3;
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            t1=tp(arr,low,low+mid, arr1);
        }
        #pragma omp section
        {
            t2=tp(arr, low+mid+ 1, high,arr1);
        }
        
    }
    t3=t1+(t2-mid);
    t2=mid+1;
    if(arr1[t1]==arr1[mid+1])
    {
    	t2=mid+2;
    	t3=t3-1;
	}
    for(t1;t1<t3;t1++)
    {
    	swap(arr[t1+1],arr[t2]);
    	swap(arr1[t1+1],arr1[t2]);
    	t2++;
	}
	return t3;
	
}

int main() {
	
	double start=omp_get_wtime();
    std::ifstream input_file("hash.txt");
    int i=0;
    int n1=10000;
    int arr1[n1];
    int number;
    while (input_file >> number) {
        arr1[i]=number;
        i++;
    }
    input_file.close();
    std::ifstream input_file2("userid.txt");
    i=0;
    int n=10000;
    int arr2[n];
    while (input_file2 >> number) {
        arr2[i]=number;
        i++;
    }
    input_file2.close();
    n = i;
    
    int k = n/4;
    hybridQuickSort(arr1,arr2, 0, n - 1,k);
    int total=twopointer(arr2,0,n-1,arr1);
	std::ofstream outfile;
	std::ofstream outfile2;
	outfile.open("hashout.txt", std::ofstream::out | std::ofstream::trunc);
    outfile2.open("useridout.txt", std::ofstream::out | std::ofstream::trunc);
	#pragma omp parallel sections
    {
        #pragma omp section
        {
        	for (int i = 0; i<total; i++) 
			{
			    outfile << arr1[i] << std::endl;
            }
        }
        #pragma omp section
        {
        	for (int i = 0; i<total; i++) 
			{
			    outfile2 << arr2[i] << std::endl;
			}
        } 
    }
    outfile << arr1[total];
    outfile.close();
    outfile2 << arr2[total];
    outfile2.close();
    double end=omp_get_wtime();
    std::cout<<end-start;
    return 0;
}
