/*
hashTables are quite efficient,
as the insertion and search procedures take in average O(1) time

even when the fill factor is high, the hashSearch function behaves well
up until 90%, the average number of searched cells is 2, even the maximum number remaining much smaller than N

at 99% fill factor, the average remains small for hits
and the maximum too

whereas for misses,
up until 90%, the average remains relatively low (below 10), but then it starts to grow pretty fast
reaching 96 for 99%, but that remains much smaller than N

and the maximum grows dratstically, compared to hits, reaching 917 searches (which remains relatively samll compared to N
<10%)

all of the above repsect the given equations:
average successful effort: log(1/(1-a))/a
average unsuccessful effort: 1/(1-a)

*/


#include "pch.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include "Profiler.h"
#pragma warning(disable:4996)

#define N 10007

Profiler profiler("hashes");

int c1 = 5;
int c2 = 7;
int ops, totalOpsF, totalOpsN;

typedef struct
{
	int key;
	char name[30];
}EntryT;

bool isNull(EntryT x)
{
	if (x.key == 0)
		return 1;
	else
		return 0;
}

bool isDeleted(EntryT x)
{
	if (x.key == -1)
		return 1;
	else
		return 0;
}

int hPrime(int key, int n)					//h' -> auxiliary hash function
{
	int sum = 0, aux = key;
	while (aux != 0)
	{
		sum += aux % 10;
		aux /= 10;
	}
	return (sum*key) % n;
}

int h(int key, int i, int n)				//quadratic hash function
{
	return (hPrime(key,n) + c1 * i + c2 * i*i) % n;
}

int hashInsert(EntryT *T, int key, char name[30], int n)		//insert function
{														//searches each entry using the hash function's value
	int i = 0;											//given by the key and i (how many times it tried to insert)
	//int isInserted = 0;								//until it finds a free entry
	do
	{													//or until it finds the given key
		int j = h(key, i,n);								//if it doesn't find any free space, than the hashTable is full
		if (isNull(T[j]) || isDeleted(T[j]))
		{
			printf("Inserted key %d with name %s at index %d\n", key, name, j);
			T[j].key = key;
			strcpy_s(T[j].name, name);
			return j;
		}
		else 
		{
			if (T[j].key == key)
			{
				strcpy_s(T[j].name, name);
				printf("Updating key %d. New name: %s\n", key,T[j].name);
				//printf("Key %d already present",key);
				return -1;
			}
			printf("Collision for key %d with key %d\n", key, T[j].key);
			i++;
		}
	} while (i < n);
	printf("No more space in hashTable\n");
	return -1;
}

int *fillSearchIndexes(EntryT *T)				//finds the values we want to search for
{												//so that we are sure it will be a hit
	int* searchIndexes = (int *)calloc(1501, sizeof(int));
	FillRandomArray(searchIndexes, 1500, 0, N - 1, true, 0);
	for (int i = 0; i < 1500; i++)
	{
		while (isNull(T[searchIndexes[i]]))
		{
			searchIndexes[i]++;
		}
	}
	return searchIndexes;
}

int hashSearch(EntryT *T, int key, int n)				//searches the table for a key
{												//at the values given by the hash function
	int i = 0;									//until it finds the requested key
	int j;										//or reaches a null or	arrives at where it started
	do
	{
		ops++;
		j = h(key, i,n);
		if (T[j].key==key)
			return j;
		else
		{
			i++;
		}
	} while (!isNull(T[j]) && i < n);
	printf("Key %d not found\n", key);
	return -1;
}

bool hashDelete(EntryT *T, int key, int n)			//extra function that deletes a given key from the table
{											// marking it as "DELETED" (-1) and not as "NULL" (0) 
	int pos = hashSearch(T, key,n);			//not to interfere with the hashSearch
	if (pos != -1)
	{
		T[pos].key = -1;
		strcpy_s(T[pos].name, "\0");
		printf("Deleting entry at index %d\n", pos);
		return true;
	}
	else
		return false;
}

EntryT *createTable(int n)
{
	EntryT *T = (EntryT *)calloc(n, sizeof(EntryT));
	for (int i = 0; i < n; i++)
	{
		T[i].key = NULL;
	}
	return T;
}

void emptyTable(EntryT *T)				//function that sets all the keys from the table to NULL
{
	for (int i = 0; i < N; i++)
	{
		T[i].key = NULL;
		strcpy_s(T[i].name, "\0");
	}
}

void printTable(EntryT *T, int n)
{
	int occ = 0;
	printf("\n");
	for (int i = 0; i < n; i++)
	{
		printf("%d %d %s\n", i, T[i].key, T[i].name);
		if (!isNull(T[i]))
			occ++;
	}
	//printf("%d\n", occ);

}

void analysis()
{
	FILE *pf = fopen("data.txt", "w");
	EntryT *T = createTable(N);

	int *random = (int *)calloc(3, sizeof(int));

	float alpha[5] = { 0.8,0.85,0.9,0.95,0.99 };
	char c[30];
	strcpy_s(c, "DOOM\0");
	float fillGrade = 0;
	fprintf(pf,"fillFact   avgFound   maxFound   avgNFound   maxNFoud\n");
	int beta = 0;
	int maxOpsF = -1;
	int maxOpsN = -1;
	for (int w = 0; w < 5; w++)
	{
		while (fillGrade < alpha[w])
		{
			FillRandomArray(random, 2, 0, 500000, true, 0);
			int key = random[0];					//insert ranom keys
			//while (key <= 0)
				//key = rand();
			if (hashInsert(T, key, c,N) > 0)
				beta++;
			fillGrade = (float)beta / (float)N;				//until the fillGrade reaches the required values
			printf("\r%f", fillGrade);
		}
		//printTable(T);
		//printf("%lf\n", fillGrade);
		for (int j = 0; j < 5; j++)
		{
			int* searchIndexes = fillSearchIndexes(T);
			for (int i = 0; i < 1500; i++)					//search for values that are in the table
			{
				hashSearch(T, T[searchIndexes[i]].key,N);
				if (ops > maxOpsF)
					maxOpsF = ops;
				totalOpsF += ops;
				ops = 0;
			}
			FillRandomArray(searchIndexes, 1500, 50001, 100000, true, 0);
			for (int i = 0; i < 1500; i++)					//search for values that are not in the table
			{
				hashSearch(T, searchIndexes[i],N);
				if (ops > maxOpsN)
					maxOpsN = ops;
				totalOpsN += ops;
				ops = 0;
			}
			//emptyTable(T);
		}
		fprintf(pf,"\r%f     %f    %d       ", alpha[w], (float)totalOpsF / (1500 * 5), maxOpsF);
		fprintf(pf,"%f     %d\n", (float)totalOpsN / (1500 * 5), maxOpsN);
		maxOpsF = -1;
		maxOpsN = -1;
		totalOpsF = 0;
		totalOpsN = 0;
	}
}

void demo()
{
	
	int n = 10;
	EntryT *T = createTable(n);

	char c[30];
	strcpy_s(c, "Einstein\0");
	hashInsert(T, 1, c,n);
	strcpy_s(c, "Bertie\0");
	hashInsert(T, 123, c,n);
	strcpy_s(c, "Max\0");
	hashInsert(T, 53, c,n);
	strcpy_s(c, "Otto\0");
	hashInsert(T, 643, c,n);
	strcpy_s(c, "Ozzy\0");
	hashInsert(T, 3, c,n);
	strcpy_s(c, "James\0");
	hashInsert(T, 9, c,n);
	strcpy_s(c, "Tom\0");
	hashInsert(T, 4, c,n);
	strcpy_s(c, "Jerr\0");
	hashInsert(T, 4, c, n);
	hashDelete(T, 123, n);
	printTable(T,n);
	printf("Searching for key 643...\n");
	printf("Found key 643 at position %d with name %s\n", hashSearch(T, 643, n),T[hashSearch(T,643,n)].name);
	printf("Searching for key 3...\n");
	printf("Found key 3 at position %d with name %s\n", hashSearch(T, 3, n), T[hashSearch(T, 3, n)].name);
	printf("Searching for key 123...\n");
	hashSearch(T, 123, n);
}

int main()
{
	demo();
	//analysis();
}

