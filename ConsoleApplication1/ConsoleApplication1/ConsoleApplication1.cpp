//in average, Bubble Sort makes the most assigns (for an array of length 10000, it will make approximatively 75000000 assigns)
//it has a quadratic growth
//it is then followed by Insertion Sort, which has a more gentle growth, but it also looks like a quadratic one (for n=10000, it takes 25000000 assigns)
//Selection Sort behaves the best, having a linear growth (for n=10000, we get 30000 assigns)

//in the best case, regarding assignments, Insertion and Selection Sort behave the same, having a linear growth, whereas Bubble Sort makes no assigns

//in the worst case, regarding assignments, Bubble Sort has the worst performance, then followed by Insertion, both having a quadratic growth 
//(for n=10000, Bubble makes a bit more than 75000000 assigns, so average is close to worst, and the we can say the same about Insertion which takes a bit over 25000000 assigns)
//Selection Sort has a linear growth

//in average, Bubble Sort makes the most comparisons (close to 99000000 for n=10000), it is then followed by Selection (with 25000000) and then by Selection (with 50000000)
//all of them have a growth which seems quadratic

//in the best case, regarding comparisons, Selection Sort behaves the same as in average (with 5000000 and quadratic), whereas Bubble and Insertion Sort have a linear growth

//int the worst case, regarding comparisons, Bubble Sort has the most comparisons (over 99000000, for n=10000), then followed by Selection Sort(over 25000000) and then Insertion Sort (almost 50000000)
//all of them have a quadratic growth

//so, Bubble Sort is the worst sorting algorithm, regarding time efficinecy (but it's simple)
//Insertion Sort and Selection Sort are, more or less, the same thing (Selection has more comparisons, but Insertion has more assignments)

#include "pch.h"
#include <iostream>
#include <stdio.h>
#include "Profiler.h"

Profiler profiler("Sorts");

int bubbleComps, insertionComps, selectionComps;
int bubbleAssigns, insertionAssigns, selectionAssigns;
int bubbleCompsAvg, insertionCompsAvg, selectionCompsAvg;
int bubbleAssignsAvg, insertionAssignsAvg, selectionAssignsAvg;
int bubbleCompsBst, insertionCompsBst, selectionCompsBst;
int bubbleAssignsBst, insertionAssignsBst, selectionAssignsBst;
int bubbleCompsWst, insertionCompsWst, selectionCompsWst;
int bubbleAssignsWst, insertionAssignsWst, selectionAssignsWst;
int bubbleTotalAvg, insertionTotalAvg, selectionTotalAvg;
int bubbleTotalBst, insertionTotalBst, selectionTotalBst;
int bubbleTotalWst, insertionTotalWst, selectionTotalWst;

void bubbleSort(int n, int *a)	//stable
{
	int ok = 1;
	while (ok)
	{
		ok = 0;
		for (int j = 1; j < n; j++)
		{
			bubbleComps++;
			if (a[j] < a[j - 1])
			{
				ok = 1;
				int aux = a[j];
				a[j] = a[j - 1];
				a[j - 1] = aux;
				bubbleAssigns += 3;
			}
		}
	}
}

void selectionSort(int n, int *a)	//not stable
{
	for (int i = 0; i < n - 1 ; i++)
	{
		int min = i;
		for (int j = i + 1; j < n; j++)
		{
			selectionComps++;
			if (a[j] < a[min])
			{
				min = j;
			}
		}
		int aux = a[i];
		a[i] = a[min];
		a[min] = aux;
		selectionAssigns += 3;
	}
}

void insertionSort(int n, int *a)	//stable
{
	for (int i = 1; i < n; i++)
	{
		int v = a[i];
		insertionAssigns++;
		int j = i-1;
		while (j >= 0 && v < a[j])
		{
			a[j + 1] = a[j];
			insertionAssigns++;
			insertionComps++;
			j--;
		}
		a[j+1] = v;
		insertionComps++;
		insertionAssigns++;
	}
}

int main()
{
	
	int ex1[5] = { 10, 4, 2, 7, 11};
	bubbleSort(5, ex1);
	for (int i = 0; i < 5; i++)
		printf("%d ", ex1[i]);
	printf("\n");
	int ex2[5] = { 10, 4, 2, 7, 11 };
	insertionSort(5, ex2);
	for (int i = 0; i < 5; i++)
		printf("%d ", ex2[i]);					//hardcoded examples that show that the sorts work
	printf("\n");
	int ex3[5] = { 10, 4, 2, 7, 11 };
	selectionSort(5, ex3);
	for (int i = 0; i < 5; i++)
		printf("%d ", ex3[i]);
	printf("\n");
	getchar();


	/*
		for (int n = 100; n <= 10000; n += 100)
		{
			insertionAssigns = 0;		//variables that count assigns and comparisons are initialized
			selectionAssigns = 0;		//being ready to count the operations in the best case
			bubbleAssigns = 0;
			insertionComps = 0;
			selectionComps = 0;
			bubbleComps = 0;

			int *a = (int *)calloc(n + 1, sizeof(int));
			FillRandomArray(a, n, 0, 50000, false, 1);			//best performance for each sort
			int *b = (int *)calloc(n + 1, sizeof(int));			//appears when the arrays are already sorted ascendingly
			for (int j = 0; j < n; j++)
				b[j] = a[j];
			int *c = (int *)calloc(n + 1, sizeof(int));
			for (int j = 0; j < n; j++)
				c[j] = a[j];
			bubbleSort(n, a);			//each sort is applied on each array (the arrays are equal)
			insertionSort(n, b);
			selectionSort(n, c);
			free(a);
			free(b);
			free(c);
			insertionAssignsBst = insertionAssigns;		//the operation counts are passed to other variables, in case we need them later
			bubbleAssignsBst = bubbleAssigns;
			selectionAssignsBst = selectionAssigns;
			insertionCompsBst = insertionComps;
			bubbleCompsBst = bubbleComps;
			selectionCompsBst = selectionComps;

			profiler.countOperation("insertionAssignsBst", n, insertionAssignsBst);		//tell the profiler to add
			profiler.countOperation("insertionCompsBst", n, insertionCompsBst);			//the obatined values
			profiler.countOperation("bubbleAssignsBst", n, bubbleAssignsBst);			//for each sort
			profiler.countOperation("bubbleCompsBst", n, bubbleCompsBst);
			profiler.countOperation("selectionAssignsBst", n, selectionAssignsBst);
			profiler.countOperation("selectionCompsBst", n, selectionCompsBst);
			profiler.countOperation("selectionTotalBst", n, selectionCompsBst + selectionAssignsBst);
			profiler.countOperation("insertionTotalBst", n, insertionCompsBst + insertionAssignsBst);
			profiler.countOperation("bubbleTotalBst", n, bubbleCompsBst + bubbleAssignsBst);

			insertionAssigns = 0;			//reinitialize the counters for the average cases
			selectionAssigns = 0;
			bubbleAssigns = 0;
			insertionComps = 0;
			selectionComps = 0;
			bubbleComps = 0;

			for (int i = 0; i < 5; i++)			//to find an average, run the sorts 5 times for each value of n
			{
				a = (int *)calloc(n + 1, sizeof(int));
				FillRandomArray(a, n, 0, 50000, false, 0);
				b = (int *)calloc(n + 1, sizeof(int));
				for (int j = 0; j < n; j++)
					b[j] = a[j];
				c = (int *)calloc(n + 1, sizeof(int));
				for (int j = 0; j < n; j++)
					c[j] = a[j];
				bubbleSort(n, a);
				insertionSort(n, b);
				selectionSort(n, c);
				free(a);
				free(b);
				free(c);
				printf("\r%d%", n / 100);		//progress
			}

			insertionAssigns/=5;		//find the average in each case
			selectionAssigns/=5;
			bubbleAssigns/=5;
			insertionComps/=5;
			selectionComps/=5;
			bubbleComps/=5;

			insertionAssignsAvg = insertionAssigns;			//move to secondary variables in case we need 
			bubbleAssignsAvg = bubbleAssigns;				//the values later
			selectionAssignsAvg = selectionAssigns;
			insertionCompsAvg = insertionComps;
			bubbleCompsAvg = bubbleComps;
			selectionCompsAvg = selectionComps;

			profiler.countOperation("insertionAssignsAvg", n, insertionAssignsAvg);		//add the values to the profiler
			profiler.countOperation("insertionCompsAvg", n, insertionCompsAvg);			//for each case
			profiler.countOperation("bubbleAssignsAvg", n, bubbleAssignsAvg);
			profiler.countOperation("bubbleCompsAvg", n, bubbleCompsAvg);
			profiler.countOperation("selectionAssignsAvg", n, selectionAssignsAvg);
			profiler.countOperation("selectionCompsAvg", n, selectionCompsAvg);
			profiler.countOperation("selectionTotalAvg", n, selectionCompsAvg + selectionAssignsAvg);
			profiler.countOperation("insertionTotalAvg", n, insertionCompsAvg + insertionAssignsAvg);
			profiler.countOperation("bubbleTotalAvg", n, bubbleCompsAvg + bubbleAssignsAvg);


			insertionAssigns = 0;		//reinitialize counters for worst performance
			selectionAssigns = 0;
			bubbleAssigns = 0;
			insertionComps = 0;
			selectionComps = 0;
			bubbleComps = 0;

			a = (int *)calloc(n + 1, sizeof(int));
			FillRandomArray(a, n, 0, 50000, false, -1);		//worst performance for each sort appears when
			b = (int *)calloc(n + 1, sizeof(int));			//the arrays are sorted descendingly
			for (int j = 0; j < n; j++)
				b[j] = a[j];
			c = (int *)calloc(n + 1, sizeof(int));
			for (int j = 0; j < n; j++)
				c[j] = a[j];
			bubbleSort(n, a);		//apply sorts
			insertionSort(n, b);
			selectionSort(n, c);
			free(a);
			free(b);
			free(c);
			insertionAssignsWst = insertionAssigns;		//move values to secondary variables
			bubbleAssignsWst = bubbleAssigns;
			selectionAssignsWst = selectionAssigns;
			insertionCompsWst = insertionComps;
			bubbleCompsWst = bubbleComps;
			selectionCompsWst = selectionComps;

			profiler.countOperation("insertionAssignsWst", n, insertionAssignsWst);		//add information to profiler
			profiler.countOperation("insertionCompsWst", n, insertionCompsWst);
			profiler.countOperation("bubbleAssignsWst", n, bubbleAssignsWst);
			profiler.countOperation("bubbleCompsWst", n, bubbleCompsWst);
			profiler.countOperation("selectionAssignsWst", n, selectionAssignsWst);
			profiler.countOperation("selectionCompsWst", n, selectionCompsWst);
			profiler.countOperation("selectionTotalWst", n, selectionCompsWst + selectionAssignsWst);
			profiler.countOperation("insertionTotalWst", n, insertionCompsWst + insertionAssignsWst);
			profiler.countOperation("bubbleTotalWst", n, bubbleCompsWst + bubbleAssignsWst);

		}
	profiler.createGroup("CompsGroupAvg","bubbleCompsAvg", "insertionCompsAvg", "selectionCompsAvg");	//group the average comparisons
	profiler.createGroup("AssignsGroupAvg", "bubbleAssignsAvg", "insertionAssignsAvg");	//group the average assigns (selection will be taken separately as it grows slowly)
	profiler.createGroup("CompsGroupBst", "bubbleCompsBst", "insertionCompsBst");		//group the best case comparisons (again, selection separately)
	profiler.createGroup("AssignsGroupBst", "bubbleAssignsBst", "insertionAssignsBst", "selectionAssignsBst"); //group the best case assigns
	profiler.createGroup("CompsGroupWst", "bubbleCompsWst", "insertionCompsWst", "selectionCompsWst");	//group the worst case comparisons
	profiler.createGroup("AssignsGroupWst", "bubbleAssignsWst", "insertionAssignsWst");	//group the  worst case assigns (selection taken separately)
	profiler.createGroup("TotalGroupAvg", "bubbleTotalAvg", "insertionTotalAvg", "selectionTotalAvg");
	profiler.createGroup("TotalGroupBst", "bubbleTotalBst", "insertionTotalBst", "selectionTotalBst");
	profiler.createGroup("TotalGroupWst", "bubbleTotalWst", "insertionTotalWst", "selectionTotalWst");

	profiler.showReport();
	*/
}