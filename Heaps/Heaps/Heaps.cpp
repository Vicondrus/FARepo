/*
on average, the bottomUp approach has less assignments than the topDown approach (approx. 22000 for n=10000, whereas
the other has 4800)

it's the same case regarding comparisons (with approx. 12000 comparisons for bottomUp, but 22000 for topDown)

it seems that they both have a liniar growth

in the best case (where the array is sorted descendingly), the bottomUp approach is, again,
better (with 2200 assignments, whereas the other has 48000)

regarding comparisons, the bottomUp approach has more than the topDown approach (22000 for tD and 24000 for bU)

their growth, here, is liniar

in the worst case, we can clearly see how badly the topDown approach manages to transform the array into a heap

we can clearly see the liniarithmic growth for the topDown aproach
and the liniar grwoth for the bottomUp (even though it's not clear at first sight that it has a liniar growth)

*/

#include "pch.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "Profiler.h"

#define INF 1000000

Profiler profiler("heaps");

int TDComps, TDAssigns;
int BUComps, BUAssigns;

int parent(int i)
{
	return (i - 1) / 2;
}

int left(int i)
{
	return 2 * (i + 1) - 1;
}

int right(int i)
{
	return 2 * (i + 1);
}


int indexOfMax3(int *a, int i, int size)	//function that returns the index of the highest
{														//of three elements of an array
	int largest;										//for which the indexes are given
	if (left(i) < size && a[left(i)] > a[i])
		largest = left(i);
	else
		largest = i;
	if (right(i) < size && a[right(i)] > a[largest])
		largest = right(i);
	BUComps+=2;
	return largest;
}

void maxHeapify(int *a, int i, int size)		//heapifies an array (restores the heap property by sinking
{												//the smaller elements
	int largest = indexOfMax3(a, i, size);
	if (largest != i)
	{
		std::swap(a[i], a[largest]);
		BUAssigns += 3;
		maxHeapify(a, largest, size);			//done recursively by heapifying each sub-heap after sinking the smaller
	}											//element by one position
}

void buildMaxHeapBA(int *a, int n, int size)	//as the last half of the array is already formed of
{												//heaps, we apply the heapify function to the first half
	for (int i = n / 2; i >= 0; i--)			//so the whole array is heapified from the each individual heap
		maxHeapify(a, i, size);					//of the last half (the bottom) up to the very first element (the top)
}

void maxBubble(int *a, int i, int key)		//the function which, given an element, 
{											//places that element on position i of the heap (which should be the size
	a[i] = key;								//of the heap) and then brings (bubbles) it up to its place
	TDAssigns++;							//so that the heap property is regained
	while (i >= 0 && a[parent(i)] < a[i])
	{
		TDComps++;
		std::swap(a[i], a[parent(i)]);
		TDAssigns += 3;
		i = parent(i);
	}
	TDComps++;
}

void maxHeapInsert(int *a, int key, int &size)		//function which inserts an element to a heap, maintaining
{													//the heap property
	//a[++size] = -INF;
	//TDAssigns++;
	size++;
	maxBubble(a, size, key);
}

void buildMaxHeapTD(int *a, int n)		//to build a heap TopDown, we just insert each element from the
{										//heap
	int size = 0;
	for (int i = 1; i < n; i++)
	{
		maxHeapInsert(a, a[i], size);
	}
}

void heapSort(int *a, int n)
{
	int size = n;
	buildMaxHeapBA(a, n, size);
	for (int i = n-1; i > 0; i--)
	{
		std::swap(a[0], a[i]);
		size--;
		maxHeapify(a, 0, size);
	}
}

void resetCounters()
{
	TDAssigns = TDComps = BUAssigns = BUComps = 0;
}

bool isHeap(int *a, int i, int n)		//function that verifies is an array has the heap property
{
	if (i > (n - 2) / 2)
		return true;

	if (a[i] >= a[2 * i + 1] && a[i] >= a[2 * i + 2] &&
		isHeap(a, 2 * i + 1, n) && isHeap(a, 2 * i + 2, n))
		return true;

	return false;
}

void countOperatonsBst(int n)
{
	profiler.countOperation("topDownAssignsBst", n, TDAssigns);
	profiler.countOperation("topDownCompsBst", n, TDComps);
	profiler.countOperation("topDownTotalBst", n, TDComps + TDAssigns);
	profiler.countOperation("bottomUpAssignsBst", n, BUAssigns);
	profiler.countOperation("bottomUpCompsBst", n, BUComps);
	profiler.countOperation("bottomUpTotalBst", n, BUAssigns + BUComps);
}

void countOperationsAvg(int n)
{
	TDAssigns /= 5;
	TDComps /= 5;
	BUAssigns /= 5;
	BUComps /= 5;
	profiler.countOperation("topDownAssignsAvg", n, TDAssigns);
	profiler.countOperation("topDownCompsAvg", n, TDComps);
	profiler.countOperation("topDownTotalAvg", n, TDComps + TDAssigns);
	profiler.countOperation("bottomUpAssignsAvg", n, BUAssigns);
	profiler.countOperation("bottomUpCompsAvg", n, BUComps);
	profiler.countOperation("bottomUpTotalAvg", n, BUAssigns + BUComps);
}

void countOperationsWst(int n)
{
	profiler.countOperation("topDownAssignsWst", n, TDAssigns);
	profiler.countOperation("topDownCompsWst", n, TDComps);
	profiler.countOperation("topDownTotalWst", n, TDComps + TDAssigns);
	profiler.countOperation("bottomUpAssignsWst", n, BUAssigns);
	profiler.countOperation("bottomUpCompsWst", n, BUComps);
	profiler.countOperation("bottomUpTotalWst", n, BUComps + BUAssigns);
}

void createGroups()
{
	profiler.createGroup("AssignsGroupAvg", "topDownAssignsAvg", "bottomUpAssignsAvg");
	profiler.createGroup("CompsGroupAvg", "topDownCompsAvg", "bottomUpCompsAvg");
	profiler.createGroup("AssignsGroupBst", "topDownAssignsBst", "bottomUpAssignsBst");
	profiler.createGroup("CompsGroupBst", "topDownCompsBst", "bottomUpCompsBst");
	profiler.createGroup("AssignsGroupWst", "topDownAssignsWst", "bottomUpAssignsWst");
	profiler.createGroup("CompsGroupWst", "topDownCompsWst", "bottomUpCompsWst");
	profiler.createGroup("TotalGroupBst", "topDownTotalBst", "bottomUpTotalBst");
	profiler.createGroup("TotalGroupAvg", "topDownTotalAvg", "bottomUpTotalAvg");
	profiler.createGroup("TotalGroupWst", "topDownTotalWst", "bottomUpTotalWst");
}

int main()
{
	int a[10] = { 1, 4, 2, 3, 9, 7, 8, 10, 14, 16 };
	heapSort(a, 10);
	int b[10] = { 1, 4, 2, 3, 9, 7, 8, 10, 14, 16 };
	buildMaxHeapBA(b, 10, 10);
	for (int i = 0; i < 10; i++)
		printf("%d ", b[i]);
	printf("\n");
	int c[10] = { 1, 4, 2, 3, 9, 7, 8, 10, 14, 16 };
	buildMaxHeapTD(c, 10);
	for (int i = 0; i < 10; i++)
		printf("%d ", c[i]);
	printf("\n");
	//for (int i = 0; i < 10; i++)
	//	printf("%d ", a[i]);
	if (!isHeap(b, 0, 10))
	{
		printf("\nFAILED BA\n");
	}

	if (!isHeap(c, 0, 10))
	{
		printf("\nFAILED TD\n");
	}
	
	/*
	int *a, *b;
	for (int n = 100; n <= 10000; n += 100)
	{
		resetCounters();

		a = (int *)calloc(n + 1, sizeof(int));
		b = (int *)calloc(n + 1, sizeof(int));
		FillRandomArray(a, n, 0, 50000, false, -1);
		for (int i = 0; i < n; i++)
		{
			b[i] = a[i];
		}

		buildMaxHeapBA(a, n, n);
		buildMaxHeapTD(b, n);

		countOperatonsBst(n);

		free(a);
		free(b);

		resetCounters();

		for (int j = 0; j < 5; j++)
		{
			a = (int *)calloc(n + 1, sizeof(int));
			b = (int *)calloc(n + 1, sizeof(int));
			FillRandomArray(a, n, 0, 50000, false, 0);
			for (int i = 0; i < n; i++)
			{
				b[i] = a[i];
			}
			buildMaxHeapBA(a, n, n);
			buildMaxHeapTD(b, n);

			free(a);
			free(b);
		}

		countOperationsAvg(n);

		resetCounters();

		a = (int *)calloc(n + 1, sizeof(int));
		b = (int *)calloc(n + 1, sizeof(int));
		FillRandomArray(a, n, 0, 50000, false, 1);
		for (int i = 0; i < n; i++)
		{
			b[i] = a[i];
		}
		buildMaxHeapBA(a, n, n);
		buildMaxHeapTD(b, n);

		countOperationsWst(n);

		free(a);
		free(b);

		printf("\r%d", n / 100);
	}
	createGroups();
	profiler.showReport();
	*/
}


