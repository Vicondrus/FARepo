/*
the time complexity for union by rank and path compression are roughly O(m log(n)) where m is the number of operations
and n is the number of nodes

for kruskal's algorithm, the other expensive operation is the sort, which takes O(m log(n)) (?) time
so, the algorithm will still take O(m log(n)) time
as m is 4*n => the function is liniarithmic O(n log(n))

[mlogn complexity (n nodes, m operations) for union by rank
mlogn for path compression
O(m alpha(n)) where alpha is the inverse of Ackermann function
mlogn complexity for kruskal]
*/


#include "pch.h"
#include <iostream>
#include <algorithm>
#include "Profiler.h"

#define NULL 0

int ops;

Profiler profiler("Disjoints");

typedef struct n
{
	int key;
	n* parent;
	int rank;
}NodeT;

typedef struct e
{
	NodeT* start;
	NodeT* end;
	int weight;
}EdgeT;


NodeT *createNode(int key)
{
	NodeT *node = (NodeT *)malloc(sizeof(NodeT));
	node->key = key;
	node->parent = NULL;
	node->rank = -1;
	return node;
}

void makeSet(NodeT *x)			//function which makes a set from a nod (setting its rank and the parent to himself)
{
	x->parent = x;
	x->rank = 0;
	ops++;
}

NodeT *findSet(NodeT *x)		//finds the representative of the set
{
	ops++;
	if (x != x->parent)
	{
		x->parent = findSet(x->parent);
		ops++;
	}
	return x->parent;
}

void link(NodeT *x, NodeT *y)		//link a node to another
{
	ops+=2;
	if (x->rank > y->rank)
		y->parent = x;
	else
	{
		x->parent = y;
		ops++;
		if (x->rank == y->rank)
			y->rank++;
	}
}

void unionS(NodeT *x, NodeT *y)			//makes the union between two sets
{
	link(findSet(x), findSet(y));
}

bool comp(EdgeT a, EdgeT b)
{
	return (a.weight > b.weight);
}


int partition(EdgeT *a, int p, int r)		
{										
	EdgeT x = a[r];						
	int i = p - 1;
	for (int j = p; j < r; j++)
	{
		if (a[j].weight <= x.weight)
		{
			i++;
			std::swap(a[i], a[j]);
		}
	}
	std::swap(a[i + 1], a[r]);
	return i + 1;
}


void quickSort(EdgeT *a, int p, int r)		
{
	if (p < r)
	{
		int q = partition(a, p, r);
		quickSort(a, p, q - 1);
		quickSort(a, q + 1, r);
	}
}


EdgeT *kruskal(EdgeT *edges, int n, int m)
{
	EdgeT *A = (EdgeT *)calloc(n, sizeof(EdgeT));		//array of edges in the tree
	quickSort(edges, 0, m - 1);						//sort the array ascendingly
	int  count = 0;
	int i = 0;
	while (count < n - 1)
	{
		if (findSet(edges[i].start) != findSet(edges[i].end))		//if the edge has the ends from different sets
		{
			unionS(edges[i].start, edges[i].end);					//take it
			A[count] = edges[i];
			count++;
		}
		i++;
	}
	return A;
}

bool findIfEdgePresent(EdgeT *edges, int n, int a, int b)
{
	for (int i = 0; i < n; i++)
		if ((edges[i].start->key == a && edges[i].end->key == b) || (edges[i].start->key == b && edges[i].end->key == a))
			return 1;
	return 0;
}

EdgeT *randomConnectedGraph(int n, int m)					//function which returns a vector
{													//of edges of a random connceted graph
	EdgeT *edges = (EdgeT *)calloc(m + 1, sizeof(EdgeT));
	NodeT **nodes = (NodeT **)calloc(n + 1, sizeof(NodeT*));
	for (int i = 0; i < n; i++)
	{
		nodes[i] = createNode(i + 1);
		makeSet(nodes[i]);
	}
	for (int i = 0; i < n - 1; i++)
	{	
		edges[i].start = nodes[i];
		edges[i].end = nodes[i+1];
		edges[i].weight = 10;
	}
	int egg[2], weight[2];
	for (int i = n - 1; i < m; i++)
	{
		FillRandomArray(egg, 2, 1, n, true);
		if (!findIfEdgePresent(edges, i, egg[0], egg[1]))
		{
			edges[i].start = nodes[egg[0] - 1];
			edges[i].end = nodes[egg[1] - 1];
			FillRandomArray(weight, 1, 1, 10);
			edges[i].weight = weight[0];
		}
		else
			i--;
	}
	return edges;
}

void demo()
{
	NodeT **nodes = (NodeT **)calloc(11, sizeof(NodeT*));
	for (int i = 0; i < 10; i++)
	{
		nodes[i] = createNode(i);
		makeSet(nodes[i]);
	}
	for (int i = 0; i < 10; i++)
	{
		printf("%d->%d ", nodes[i]->key, findSet(nodes[i])->key);
	}
	printf("\n");
	unionS(nodes[0], nodes[1]);
	printf("Union between %d and %d\n", nodes[0]->key, nodes[1]->key);
	for (int i = 0; i < 10; i++)
	{
		printf("%d->%d ", nodes[i]->key, findSet(nodes[i])->key);
	}
	printf("\n");
	unionS(nodes[3], nodes[5]);
	printf("Union between %d and %d\n", nodes[3]->key, nodes[5]->key);
	for (int i = 0; i < 10; i++)
	{
		printf("%d->%d ", nodes[i]->key, findSet(nodes[i])->key);
	}
	printf("\n");
	unionS(nodes[8], nodes[6]);
	printf("Union between %d and %d\n", nodes[8]->key, nodes[6]->key);
	for (int i = 0; i < 10; i++)
	{
		printf("%d->%d ", nodes[i]->key, findSet(nodes[i])->key, findSet(nodes[i])->rank);
	}
	printf("\n");
	unionS(nodes[4], nodes[9]);
	printf("Union between %d and %d\n", nodes[4]->key, nodes[9]->key);
	for (int i = 0; i < 10; i++)
	{
		printf("%d->%d ", nodes[i]->key, findSet(nodes[i])->key, findSet(nodes[i])->rank);
	}
	printf("\n");
	unionS(nodes[4], nodes[3]);
	printf("Union between %d and %d\n", nodes[4]->key, nodes[3]->key);
	for (int i = 0; i < 10; i++)
	{
		printf("%d->%d ", nodes[i]->key, findSet(nodes[i])->key, findSet(nodes[i])->rank);
	}
	printf("\n");
	unionS(nodes[4], nodes[1]);
	printf("Union between %d and %d\n", nodes[4]->key, nodes[1]->key);
	for (int i = 0; i < 10; i++)
	{
		printf("%d->%d ", nodes[i]->key, findSet(nodes[i])->key);
	}
	printf("\n");
}

void demoK()
{
	int n = 5,m=7;
	EdgeT *edges = randomConnectedGraph(n,m);
	for (int i = 0; i < m; i++)
	{
		printf("%d %d %d\n", edges[i].start->key, edges[i].end->key, edges[i].weight);
	}
	printf("\n\n");
	EdgeT *kr = kruskal(edges, n, m);
	for (int i = 0; i < n - 1; i++)
		printf("%d %d %d\n", kr[i].start->key, kr[i].end->key,kr[i].weight);
}

void analysis()
{
	for (int n = 100; n <= 10000; n += 100)
	{
		printf("\r%d", n / 100);
		ops = 0;
		for (int j = 0; j < 5; j++)
		{
			EdgeT *edges = randomConnectedGraph(n,4*n);
			//for (int i = 0; i < 4 * n; i++)
			//{
				//printf("%d %d %d\n", edges[i].start->key, edges[i].end->key, edges[i].weight);
			//}
			//printf("\n\n");
			EdgeT *kr = kruskal(edges, n, 4 * n);
			//for (int i = 0; i < n - 1; i++)
				//printf("%d %d\n", kr[i].start->key, kr[i].end->key);
			//for (int i = 0; i < n; i++)
				//free(edges[i].start);
			//free(edges[n - 1].end);
			free(edges);
			free(kr);
		}
		profiler.countOperation("operationsOnSets", n, ops/5);
	}
	profiler.showReport();
}

int main()
{
	demoK();
	//analysis();
}

