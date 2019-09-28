/*
quickSort is slightly better than heapSort in most cases, regarding assignments and comparisons separately,
so as a whole too (it makes 270000 assignments in average, as opposed to 370000, for an array of 10000 elemetns; 160000 comparisons as opposed to 270000)

in the best case, heapSort behaves slightly better than in the average case (with a total of 590000 computations), but, again,
worse than quickSort (which behaves better even in its average case that heapSort in best case; it's hard to
make the best case for quickSort, as the algorithm that I used doesn't work, but I assume that it behaves slightly better than
in the average case)
update: I used a different pivot for best case, so that an already ordered array would be its best case
it really does significatly less computations (a total of 330000, 70000 less than in average)
and it behaves better than heapSort
update2: yet, there is a case in which heapSort behaves better, having a liniar growth, when all the elements
of the array are equal; if this happens, it behaves better quickSort, even in its best case

they both have linearithmic growth in both cases (O(nlogn)) (and if the elements in the array that is heapSorted are distinct)

the downside of quickSort, however, is in its worst case (which, if implemented well enough, can be avoided)
it has a quadratic growth and it makes a tremendous ammount of comparisons and assignments

whereas heapSort has a linearithmic growth even in the worst case



*/


#include "pch.h"
#include <iostream>
#include <time.h>
#include "Profiler.h"

Profiler profiler("HeapNQuick");

int countCompsHeap, countAssignsHeap, countAssignsQuick, countCompsQuick;

int parent(int i)
{
	return (i - 1) / 2;
}

int left(int i)
{
	return (i + 1) * 2 - 1;
}

int right(int i)
{
	return (i + 1) * 2;
}

int maxOfThree(int *a, int i, int size)
{
	int larger;
	if (left(i) < size && a[i] < a[left(i)])
		larger = left(i);
	else
		larger = i;
	if (right(i) < size && a[larger] < a[right(i)])
		larger = right(i);
	countCompsHeap += 2;
	return larger;
}

void maxHeapify(int *a, int n, int i, int size)
{
	int largest = maxOfThree(a, i, size);
	if (largest != i)
	{
		std::swap(a[largest], a[i]);
		countAssignsHeap += 3;
		maxHeapify(a, n, largest, size);
	}
}

void buildHeap(int *a, int n, int size)
{
	for (int i = n / 2; i >= 0; i--)
		maxHeapify(a, n, i, size);
}

void heapSort(int *a, int n)		//heapSort algorithm		//not stable
{									//that builds a heap and than puts the greatest parent on the corresponding position
	buildHeap(a, n, n);				//than it repeatedly applies heapify to mantain the heap property of the array
	int size = n;
	for (int i = n - 1; i > 0; i--)
	{
		std::swap(a[0], a[i]);
		countAssignsHeap += 3;
		size--;
		maxHeapify(a, n, 0, size);
	}
}

int crippledPartition(int *a, int p, int r)
{
	int x = a[(p + r) / 2];
	int i = p - 1;
	int j = r + 1;
	while (i <= j)
	{
		do
			j--;
		while (a[j] > x);
		do
			i++;
		while (a[i] < x);
		if (i < j)
		{
			std::swap(a[i], a[j]);
			i++;
			j--;
		}
		else
			return j;
	}
}

int partition(int *a, int p, int r)		//partition algortithm that
{										//moves all the element greater that the pivot to its right
	int x = a[r];						//and all the lesser elements, to its left
	countAssignsQuick++;				//returning its position
	int i = p - 1;
	for (int j = p; j < r; j++)
	{
		if (a[j] <= x)
		{
			i++;
			std::swap(a[i], a[j]);
			countAssignsQuick += 3;
		}
		countCompsQuick++;
	}
	std::swap(a[i + 1], a[r]);
	countAssignsQuick += 3;
	return i + 1;
}

int partitionForBest(int *a, int p, int r)		//partition algortithm that
{												//is used for the best case
	int x = a[(r+p)/2];						
	countAssignsQuick++;				
	int i = p - 1;
	for (int j = p; j < r; j++)
	{
		if (a[j] <= x)
		{
			i++;
			std::swap(a[i], a[j]);
			countAssignsQuick += 3;
		}
		countCompsQuick++;
	}
	std::swap(a[i + 1], a[(p+r)/2]);
	countAssignsQuick += 3;
	return i + 1;
}

void quickSort(int *a, int p, int r)		//quickSort algorithm that is applied recursively		//not stable
{
	if (p < r)
	{
		int q = partition(a, p, r);
		quickSort(a, p, q - 1);
		quickSort(a, q + 1, r);
	}
}

void quickSortForBest(int *a, int p, int r)		
{
	if (p < r)
	{
		int q = partitionForBest(a, p, r);
		quickSortForBest(a, p, q - 1);
		quickSortForBest(a, q + 1, r);
	}
}

int randomPartition(int *a, int p, int r)
{
	srand(time(NULL));
	int i = rand();
	i = i % (r - p) + p;
	std::swap(a[p], a[i]);
	return partition(a, p, r);
}

int quickSelect(int *a, int p, int r, int i)		//algorithm that selects the ith greatest element from array
{
	if (p == r)
		return a[p];
	int q = randomPartition(a, p, r);
	int relPos = q - p + 1;
	if (i == relPos)
		return a[q];
	else
		if (i < relPos)
			return quickSelect(a, p, q - 1, i);
		else
			return quickSelect(a, q + 1, r, i - relPos);
}

void selectionSort(int *a, int p, int q)
{
	for (int i = p; i < q; i++)
	{
		int min = i;
		for (int j = i + 1; j <= q; j++)
		{
			if (a[j] < a[min])
			{
				min = j;
			}
		}
		int aux = a[i];
		a[i] = a[min];
		a[min] = aux;
	}
}

void quickSortFaster(int *a, int p, int r, int n)
{
	if (r - p < 5)
		selectionSort(a, p, r);
	else
	{
		quickSelect(a, p, r, n / 2);
		quickSortFaster(a, p, n / 2, n);
		quickSortFaster(a, n / 2 + 1, r, n);
	}
}

void countAvg(int n)
{
	profiler.countOperation("QuickAssignsAvg", n, countAssignsQuick);
	profiler.countOperation("QuickCompsAvg", n, countCompsQuick);

	profiler.countOperation("HeapAssignsAvg", n, countAssignsHeap);
	profiler.countOperation("HeapCompsAvg", n, countCompsHeap);

	profiler.countOperation("TotalQuickAvg", n, countAssignsQuick + countCompsQuick);
	profiler.countOperation("TotalHeapAvg", n, countAssignsHeap + countCompsHeap);
}

void countWst(int n)
{
	profiler.countOperation("QuickAssignsWst", n, countAssignsQuick);
	profiler.countOperation("QuickCompsWst", n, countCompsQuick);

	profiler.countOperation("HeapAssignsWst", n, countAssignsHeap);
	profiler.countOperation("HeapCompsWst", n, countCompsHeap);

	profiler.countOperation("TotalQuickWst", n, countAssignsQuick + countCompsQuick);
	profiler.countOperation("TotalHeapWst", n, countAssignsHeap + countCompsHeap);
}

void countBst(int n)
{
	profiler.countOperation("QuickAssignsBst", n, countAssignsQuick);
	profiler.countOperation("QuickCompsBst", n, countCompsQuick);

	profiler.countOperation("HeapAssignsBst", n, countAssignsHeap);
	profiler.countOperation("HeapCompsBst", n, countCompsHeap);

	profiler.countOperation("TotalQuickBst", n, countAssignsQuick + countCompsQuick);
	profiler.countOperation("TotalHeapBst", n, countAssignsHeap + countCompsHeap);
}

void resetCounters()
{
	countAssignsHeap = countAssignsQuick = countCompsHeap = countCompsQuick = 0;
}

void generateBestQuickSort(int *b, int p, int r)			//algorithm that generates
{															//the best case for quickSort
	int count = r - p;								//it swaps recursively the middle element with
	if (count < 3)											//the to-be pivot
		return;												//guaranteeing that the best element to be chosen
	int q = p + (count - 1) / 2;					//will be at the pivot's position
	//if(q-p >=3 )										//doesn't really work
		generateBestQuickSort(b, p, q);
	std::swap(b[q], b[r]);
	//if(p-q-1 >=3)
		generateBestQuickSort(b, ++q, r);
}

void createGroups()
{
	profiler.createGroup("AssignsAvg", "QuickAssignsAvg", "HeapAssignsAvg");
	profiler.createGroup("CompsAvg", "QuickCompsAvg", "HeapCompsAvg");
	profiler.createGroup("TotalAvg", "TotalQuickAvg", "TotalHeapAvg");

	profiler.createGroup("AssignsBst", "QuickAssignsBst", "HeapAssignsBst");
	profiler.createGroup("CompsBst", "QuickCompsBst", "HeapCompsBst");
	profiler.createGroup("TotalBst", "TotalQuickBst", "TotalHeapBst");
	//profiler.createGroup("TotalBst/Avg", "TotalQuickAvg", "TotalHeapBst");

	//profiler.createGroup("AssignsWst", "QuickAssignsWst", "HeapAssignsWst");
	//profiler.createGroup("CompsWst", "QuickCompsWst", "HeapCompsWst");
	//profiler.createGroup("TotalWst", "TotalQuickWst", "TotalHeapWst");
}

int main()
{	
	int a[5] = { 5,2,1,3,4 };
	int b[5] = { 5,2,1,3,4 };
	printf("%d\n",quickSelect(a, 0, 4, 3));
	quickSort(a,0, 4);
	heapSort(b,5);
	for (int i = 0; i < 5; i++)
		printf("%d ", a[i]);
	printf("\n");
	for (int i = 0; i < 5; i++)
		printf("%d ", b[i]);

	/*
	int *a, *b;
	for (int n = 100; n <= 10000; n += 100)
	{

		printf("\r%d", n / 100);
		for (int i = 0; i < 5; i++)
		{
			a = (int *)calloc(n + 1, sizeof(int));
			b = (int *)calloc(n + 1, sizeof(int));
			FillRandomArray(a, n, 0, 50000, false, 0);
			for (int j = 0; j < n; j++)
				b[j] = a[j];
			quickSort(a, 0, n - 1);
			heapSort(b, n);
			free(a);
			free(b);
		}

		countAssignsHeap /= 5;
		countAssignsQuick /= 5;
		countCompsHeap /= 5;
		countCompsQuick /= 5;

		countAvg(n);

		resetCounters();

		
		a = (int *)calloc(n + 1, sizeof(int));
		b = (int *)calloc(n + 1, sizeof(int));
		FillRandomArray(a, n, 0, 50000, false, 1);
		for (int i = 0; i < n; i++)
			b[i] = a[i];

		quickSort(a, 0, n - 1);
		heapSort(b, n);

		free(a);
		free(b);

		countWst(n);

		resetCounters();

		a = (int *)calloc(n + 1, sizeof(int));
		b = (int *)calloc(n + 1, sizeof(int));
		FillRandomArray(a, n, 0, 50000, false, 1);
		for (int i = 0; i < n; i++)
			b[i] = 15;
		//generateBestQuickSort(b, 0, n - 1);			//does not work properly as the sort has more operations that it should

		quickSortForBest(a, 0, n - 1);
		heapSort(b, n);

		free(a);
		free(b);

		countBst(n);
		
		resetCounters();
	}

	createGroups();

	profiler.showReport();
	*/
}
