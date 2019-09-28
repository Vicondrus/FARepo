/*
the time complexity of bfs is O(n + m) (liniar), where n is the number of nodes and m, the number of edges
in the worst case, every node and edge will be explored

it passes trough every node for initialization
then the adjency list of all the nodes is scanned once (when the node is at the head of the queue
*/

#include "pch.h"
#include <iostream>
#include "Profiler.h"

Profiler profiler("breadthTaking");

#define WHITE 0
#define GRAY 1
#define BLACK 2
#define INF 100000

int ops;

typedef struct n
{
	int key;
	n *next;
}NodeT;

typedef struct m
{
	int key;
	m **children;
	int nextChild;
	int childSize;
}MNodeT;

typedef struct l
{
	int key;
	NodeT *first;
	NodeT *last;
	int color;
	int dist;
	l *parent;
}ListT;

typedef struct g
{
	ListT **nodes;
	int size;
}GraphT;

ListT *createList(int key)
{
	ListT *l = (ListT *)malloc(sizeof(ListT));
	l->first = NULL;
	l->last = NULL;
	l->key = key;
	l->color = WHITE;
	return l;
}

MNodeT *initNode(int key)				//multiway tree node
{
	MNodeT *n = (MNodeT *)malloc(sizeof(MNodeT));
	n->key = key;
	n->nextChild = 0;
	n->children = (MNodeT **)calloc(3, sizeof(MNodeT*));
	n->childSize = 10;
	return n;
}


void append(ListT *list, int key)
{
	if (list->key == key)
		return;
	NodeT *node = (NodeT *)malloc(sizeof(NodeT));
	node->key = key;
	node->next = NULL;
	if (list->first == NULL)
	{
		list->first = list->last = node;
	}
	else
	{
		list->last->next = node;
		list->last = node;
	}
}

bool find(ListT *list, int key)
{
	NodeT* walker = list->first;
	while (walker != NULL)
	{
		if (walker->key == key)
			return true;
		walker = walker->next;
	}
	return false;
}

bool addEdge(ListT **graph, int a, int b)
{
	if (a == b)
		return false;
	if (find(graph[a-1], b))
		return false;
	append(graph[a-1], b);
	append(graph[b-1], a);
	return true;
}

void printList(ListT *list)
{
	NodeT* walker = list->first;
	printf("%d->", list->key);
	while (walker != NULL)
	{
		printf("%d ", walker->key);
		walker = walker->next;
	}
	printf("\n");
}

void addChild(MNodeT *parent, MNodeT *child)
{
	if (parent->nextChild >= parent->childSize)
	{
		parent->childSize *= 2;
		parent->children = (MNodeT **)realloc(parent->children, (parent->childSize + 1) * sizeof(MNodeT*));
	}
	parent->children[parent->nextChild++] = child;
}

MNodeT* bfs_component_demo(GraphT *graph, int s)				//bfs that stores nodes in bfs, for demo
{
	for (int i = 0; i < graph->size; i++)
	{
		//graph->nodes[i]->color = WHITE;
		graph->nodes[i]->dist = INF;
		graph->nodes[i]->parent = NULL;
	}
	graph->nodes[s - 1]->color = GRAY;
	graph->nodes[s - 1]->dist = 0;
	graph->nodes[s - 1]->parent = NULL;
	MNodeT **queue = (MNodeT **)calloc(graph->size + 1, sizeof(MNodeT *));
	int start = 0;
	int end = 0;
	queue[end++] = initNode(s);
	while (end > start)
	{
		NodeT *walker = graph->nodes[queue[start]->key - 1]->first;
		while (walker != NULL)
		{
			if (graph->nodes[walker->key - 1]->color == WHITE)
			{
				graph->nodes[walker->key - 1]->color = GRAY;
				graph->nodes[walker->key - 1]->color = graph->nodes[queue[start]->key - 1]->dist + 1;
				graph->nodes[walker->key - 1]->parent = graph->nodes[queue[start]->key - 1];
				queue[end++] = initNode(walker->key);
				addChild(queue[start], queue[end - 1]);
			}
			walker = walker->next;
		}
		//printf("%d ", graph.nodes[queue[start]->key - 1]->key);
		graph->nodes[queue[start]->key - 1]->color = BLACK;
		start++;
		//free(queue[start - 1]);
	}
	//free(queue);
	//for (int i = 0; i < end; i++)
	//{
		//printf("%d ", graph->nodes[queue[i]->key-1]->color);
	//}
	//printf("\n");
	return queue[0];
}

void bfs_component_analysis(GraphT *graph, int s)		//simplified bfs for analyiss, without multiwayTree
{
	for (int i = 0; i < graph->size; i++)
	{
		//graph->nodes[i]->color = WHITE;				//initialization
		graph->nodes[i]->dist = INF;
		graph->nodes[i]->parent = NULL;
		ops++;
	}
	graph->nodes[s - 1]->color = GRAY;
	graph->nodes[s - 1]->dist = 0;
	graph->nodes[s - 1]->parent = NULL;
	ops++;
	int *queue = (int *)calloc(graph->size + 1, sizeof(int));	//queue represented as an array
	int start = 0;
	int end = 0;
	queue[end++] = s;
	ops++;
	while (end > start)
	{
		NodeT *walker = graph->nodes[queue[start] - 1]->first;
		ops++;														//while the queue is not empty
		while (walker != NULL)										//take all the top's neighbours
		{
			ops++;
			if (graph->nodes[walker->key - 1]->color == WHITE)
			{
				graph->nodes[walker->key - 1]->color = GRAY;			//color them and enqueue
				graph->nodes[walker->key - 1]->dist = graph->nodes[queue[start] - 1]->dist + 1;
				graph->nodes[walker->key - 1]->parent = graph->nodes[queue[start] - 1];
				queue[end++] = walker->key;
				//printf("%d ", queue[end - 1]);
				ops++;
				ops += 3;
			}
			walker = walker->next;
		}
		//printf("%d ", graph.nodes[queue[start]->key - 1]->key);
		graph->nodes[queue[start] - 1]->color = BLACK;
		start++;										//dequeue
		ops++;
		//free(queue[start - 1]);
	}
	free(queue);
	//for (int i = 0; i < end; i++)
	//{
		//printf("%d ", graph->nodes[queue[i]->key-1]->color);
	//}
	//printf("\n");
}

void freeMNode(MNodeT *root)
{
	for (int i = 0; i < root->childSize; i++)
		free(root->children[i]);
	free(root);
}

void MPreOrder1(MNodeT *root, int level)
{
	if (root == NULL)
		return;
	printf("%*s%d\n", 3 * level, "", root->key);
	for (int i = 0; i < root->nextChild; i++)
		MPreOrder1(root->children[i], level + 1);
	//freeMNode(root);
}

void bfs_demo(GraphT g)
{

	for (int i = 0; i < g.size; i++)
	{
		//printf("%d\n", g.nodes[i]->color);
		if (g.nodes[i]->color == WHITE)
		{
			MNodeT *root = bfs_component_demo(&g, g.nodes[i]->key);
			MPreOrder1(root, 0);
			printf("\n\n");
		}
	}
}

void bfs_analysis(GraphT g)
{

	for (int i = 0; i < g.size; i++)
	{
		//printf("%d\n", g.nodes[i]->color);
		if (g.nodes[i]->color == WHITE)
		{
			bfs_component_analysis(&g, g.nodes[i]->key);
		}
	}
	//printf("\n\n\n");
}

GraphT generateRandomGraph(int n, int m)			//function that generates a random graph, not necessarely connected
{
	GraphT graph;
	graph.nodes = (ListT **)calloc(n + 1, sizeof(ListT *));
	for (int i = 0; i < n; i++)
	{
		graph.nodes[i] = createList(i + 1);
	}
	graph.size = n;
	int count = 0;
	int *random = (int *)calloc(3, sizeof(int));
	while (count < m)
	{
		FillRandomArray(random, 2, 1, n, true);
		if (addEdge(graph.nodes, random[0], random[1]) == true)
			count++;
	}
	free(random);
	return graph;
}

void freeGraph(GraphT g)
{
	for (int i = 0; i < g.size; i++)
	{
		NodeT *walker = g.nodes[i]->first;
		NodeT *prev = NULL;
		while (walker != NULL)
		{
			prev = walker;
			walker = walker->next;
			free(prev);
		}
		free(g.nodes[i]);
	}
	free(g.nodes);
}

void demo()
{
	int n = 9;
	ListT **graph = (ListT **)calloc(n + 1, sizeof(ListT*));
	for (int i = 0; i < n; i++)
	{
		graph[i] = createList(i + 1);
	}
	addEdge(graph, 1, 2);
	addEdge(graph, 2, 3);
	addEdge(graph, 1, 3);
	addEdge(graph, 1, 5);
	addEdge(graph, 3, 4);
	addEdge(graph, 4, 5);
	addEdge(graph, 3, 1);
	addEdge(graph, 2, 3);
	addEdge(graph, 6, 7);
	addEdge(graph, 7, 8);
	addEdge(graph, 7, 9);
	for (int i = 0; i < n; i++)
		printList(graph[i]);
	printf("\n\n\n");
	GraphT g1;
	g1.nodes = graph;
	g1.size = n;
	bfs_demo(g1);
	//MPreOrder1(root, 0);
}

void analysis()
{
	int n;
	int m;
	n = 100;
	for (m = 1000; m <= 4500; m += 100)
	{
		ops = 0;
		printf("\r%d", m / 100);
		GraphT g = generateRandomGraph(n, m);
		//for (int i = 0; i < 5; i++)
			//printList(g.nodes[i]);
		//printf("\n\n\n");
		bfs_analysis(g);
		freeGraph(g);
		profiler.countOperation("variableEdges", m, ops);
	}
	printf("\n");
	m = 4500;
	for (n = 100; n <= 200; n += 10)
	{
		ops = 0;
		printf("\r%d", n / 10);
		GraphT g = generateRandomGraph(n, m);
		//for (int i = 0; i < 5; i++)
			//printList(g.nodes[i]);
		//printf("\n\n\n");
		bfs_analysis(g);
		freeGraph(g);
		profiler.countOperation("variableVertices", n, ops);
	}
	profiler.showReport();
}

int main()
{
	demo();
	//analysis();
}


