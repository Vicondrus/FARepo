/*
depth first search has a time complexity of O(m+n), where m is the number of edges and n, the number of vertices
if one remains constant and the other varies, we will get a liniar growth

tarjan's algorithm has a time complexity of O(m+n) too, passing trough each node and trough each edge once, in the worst case

as the topological sort is just a dfs with an additional stack, it will also have a time complexity of O(m+n)
tarjan's algorithm can be used to check if the graph is a dag or not (as a topological sort can be applied only to a dag)
only nodes with no ingoing edges can be used as start points
*/

#include "pch.h"
#include <iostream>
#include "Profiler.h"

#define WHITE 0
#define GRAY 1
#define BLACK 2
#define INF 100000
#define UNDEFINED -351

Profiler profiler("inDepth");

typedef struct q
{
	int key;
	q **children;
	int nextChild;
}MNodeT;

typedef struct n
{
	int key;
	int fin;
	n *next;
}NodeT;

typedef struct l
{
	int key;
	NodeT *first;
	NodeT *last;
	int color;
	int disc;
	int fin;
	int index;
	int lowlink;
	bool onStack;
	l *parent;
}ListT;

typedef struct g
{
	ListT **nodes;
	int size;
}GraphT;

MNodeT *initNode(int key)				//multiway tree node
{
	MNodeT *n = (MNodeT *)malloc(sizeof(MNodeT));
	n->key = key;
	n->nextChild = 0;
	n->children = (MNodeT **)calloc(10, sizeof(MNodeT*));
	return n;
}

int ops = 0;

void createNode(int *parent, int i, MNodeT **taken, MNodeT **root, int *ind)		//recursive function
{																		//which fills the auxiliary array
	if (taken[i] != NULL)												//it goes up to the root (and it sets it)
		return;															//than it links the nodes to their parents
	taken[i] = initNode(i);												//when exiting recursion
	if (parent[i] == -1)
	{
		root[(*ind)++] = taken[i];
		return;
	}
	if (taken[parent[i]] == NULL)
	{
		createNode(parent, parent[i], taken, root, ind);
	}
	MNodeT *n = taken[parent[i]];
	n->children[n->nextChild++] = taken[i];
}

MNodeT **createMTree(int *parent, int n, int *ind)									//function which initializes the array of nodes
{																		//and than goes through the parent vector 
	MNodeT **taken = (MNodeT **)calloc(n + 2, sizeof(MNodeT*));			//creating each corresponding node
	for (int i = 1; i <= n; i++)
	{
		taken[i] = NULL;
	}

	MNodeT **roots = (MNodeT **)calloc(n,sizeof(MNodeT*));
	for (int i = 1; i <= n; i++)
	{
		createNode(parent, i, taken, roots, ind);
	}
	return roots;
}

void MPreOrder1(MNodeT *root, int level)
{
	if (root == NULL)
		return;
	printf("%*s%d\n", 3 * level, "", root->key);
	for (int i = 0; i < root->nextChild; i++)
		MPreOrder1(root->children[i], level + 1);
}

ListT *createList(int key)
{
	ListT *l = (ListT *)malloc(sizeof(ListT));
	l->first = NULL;
	l->last = NULL;
	l->key = key;
	l->color = WHITE;
	l->index = UNDEFINED;
	l->onStack = false;
	return l;
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

void insert(ListT *list, int key, int fin)
{
	NodeT *node = (NodeT *)malloc(sizeof(NodeT));
	node->key = key;
	node->fin = fin;
	node->next = NULL;
	if (list->first == NULL)
	{
		list->first = list->last = node;
	}
	else
	{
		node->next = list->first;
		list->first = node;
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

bool addEdgeOriented(ListT **graph, int start, int end)
{
	if (start == end)
		return false;
	if (find(graph[start - 1], end))
		return false;
	append(graph[start - 1], end);
	//append(graph[b - 1], a);
	return true;
}

bool addEdgeNOriented(ListT **graph, int start, int end)
{
	if (start == end)
		return false;
	if (find(graph[start - 1], end))
		return false;
	append(graph[start - 1], end);
	append(graph[end - 1], start);
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

void dfsVisit(GraphT g, ListT *u, int *time)
{
	(*time)++;
	u->disc = *time;
	u->color = GRAY;
	ops++;
	NodeT *walker = u->first;
	while (walker != NULL)
	{
		ops++;
		if (g.nodes[walker->key - 1]->color == WHITE)
		{
			g.nodes[walker->key - 1]->parent = u;
			dfsVisit(g, g.nodes[walker->key - 1], time);
			ops++;
		}
		walker = walker->next;
	}
	u->color = BLACK;
	(*time)++;
	u->fin = *time;
}

void dfsVisitDemo(GraphT g, ListT *u, int *time, int *parent)
{
	(*time)++;
	u->disc = *time;
	u->color = GRAY;
	//printf("%d ", u->key);
	NodeT *walker = u->first;
	while (walker != NULL)
	{
		if (g.nodes[walker->key - 1]->color == WHITE)
		{
			g.nodes[walker->key - 1]->parent = u;
			parent[walker->key] = u->key;
			dfsVisitDemo(g, g.nodes[walker->key - 1], time, parent);
		}
		walker = walker->next;
	}
	u->color = BLACK;
	(*time)++;
	u->fin = *time;
}

void dfs(GraphT g)
{
	for (int i = 0; i < g.size; i++)
	{
		ops++;
		g.nodes[i]->color = WHITE;
		g.nodes[i]->parent = NULL;
	}
	int time = 0;
	for (int i = 0; i < g.size; i++)
	{
		ops++;
		if (g.nodes[i]->color == WHITE)
		{
			dfsVisit(g, g.nodes[i],&time);
		}
	}
}

void dfsDemo(GraphT g, int *parent)
{
	for (int i = 0; i < g.size; i++)
	{
		g.nodes[i]->color = WHITE;
		g.nodes[i]->parent = NULL;
		parent[i] = -1;
	}
	int time = 0;
	for (int i = 0; i < g.size; i++)
	{
		if (g.nodes[i]->color == WHITE)
		{
			dfsVisitDemo(g, g.nodes[i], &time, parent);
			//printf("\n");
		}
	}
}

int minn(int a, int b)
{
	if (a > b)
		return b;
	else
		return a;
}

void strongConnect(GraphT g, ListT *v, int *index, int *stack, int *end, int *components, bool print)
{
	v->index = ++(*index);
	v->lowlink = *index;
	stack[(*end)++] = v->key;
	//for (int i = 0; i < *end; i++)
		//printf("%d ", stack[i]);
	//printf("\n");
	v->onStack = true;
	NodeT *walker = v->first;
	while (walker != NULL)
	{
		if (g.nodes[walker->key - 1]->index == UNDEFINED)
		{
			strongConnect(g, g.nodes[walker->key - 1], index, stack, end, components, print);
			v->lowlink = minn(v->lowlink, g.nodes[walker->key - 1]->lowlink);
		}
		else
			if (g.nodes[walker->key - 1]->onStack == true)
			{
				v->lowlink = minn(v->lowlink, g.nodes[walker->key - 1]->index);
			}
		walker = walker->next;
	}
	if (v->lowlink == v->index)
	{
		if(print)
			printf("\nStarting component from %d with lowlink %d: ",v->key,v->lowlink);
		(*components)++;
		ListT *w;
		do 
		{
			w = g.nodes[stack[*end - 1] - 1];
			if(print)
				printf("%d ", w->key);
			(*end)--;
			//for (int i = 0; i < *end; i++)
				//printf("%d ", stack[i]);
			//printf("\n");
			w->onStack = false;

		} while (v != w);
	}
}

int tarjan(GraphT g, bool print)
{
	int components = 0;
	int index = 0;
	int *stack = (int *)calloc(g.size + 1, sizeof(int));
	int end = 0;
	for (int i = 0; i < g.size; i++)
	{
		if (g.nodes[i]->index == UNDEFINED)
		{
			strongConnect(g, g.nodes[i],&index,stack,&end,&components,print);
		}
	}
	return components;
}

void dfsVisitTopological(GraphT g, ListT *u, int *time,ListT *sorted)
{
	(*time)++;
	u->disc = *time;
	u->color = GRAY;
	NodeT *walker = u->first;
	while (walker != NULL)
	{
		if (g.nodes[walker->key - 1]->color == WHITE)
		{
			g.nodes[walker->key - 1]->parent = u;
			dfsVisitTopological(g, g.nodes[walker->key - 1], time, sorted);
		}
		walker = walker->next;
	}
	u->color = BLACK;
	(*time)++;
	u->fin = *time;
	insert(sorted, u->key,u->fin);
}

bool checkForInGoing(GraphT g, int x)
{
	for (int i = 0; i < g.size; i++)
	{
		NodeT *walker = g.nodes[i]->first;
		while (walker != NULL)
		{
			if (walker->key == x)
				return 0;
			walker = walker->next;
		}
	}
	return 1;
}

void dfsTopological(GraphT g, ListT *sorted)
{
	for (int i = 0; i < g.size; i++)
	{
		g.nodes[i]->color = WHITE;
		g.nodes[i]->parent = NULL;
	}
	int time = 0;
	for (int i = 0; i < g.size; i++)
	{
		if (g.nodes[i]->color == WHITE && checkForInGoing(g,g.nodes[i]->key))
		{
			dfsVisitTopological(g, g.nodes[i], &time,sorted);
		}
	}
}

ListT *topologicalSort(GraphT g)
{
	ListT *sorted = createList(0);
	if (tarjan(g,false) == g.size)
		dfsTopological(g, sorted);
	return sorted;
}

void demo()
{
	int n = 9;
	ListT **graph = (ListT **)calloc(n + 1, sizeof(ListT*));
	for (int i = 0; i < n; i++)
	{
		graph[i] = createList(i + 1);
	}
	addEdgeNOriented(graph, 1, 2);
	addEdgeNOriented(graph, 2, 3);
	addEdgeNOriented(graph, 1, 3);
	//addEdgeNOriented(graph, 1, 5);
	addEdgeNOriented(graph, 3, 4);
	//addEdgeNOriented(graph, 4, 5);
	addEdgeNOriented(graph, 3, 1);
	addEdgeNOriented(graph, 2, 3);
	addEdgeNOriented(graph, 5, 7);
	addEdgeNOriented(graph, 5, 8);
	addEdgeNOriented(graph, 5, 9);
	addEdgeNOriented(graph, 9, 6);
	for (int i = 0; i < n; i++)
		printList(graph[i]);
	printf("\n\n\n");
	GraphT g1;
	g1.nodes = graph;
	g1.size = n;
	int *parents = (int *)calloc(n + 2, sizeof(int));
	dfsDemo(g1, parents);
	int ind = 0;
	//for (int i = 1; i <= n; i++)
		//printf("%d ", parents[i]);
	MNodeT **roots = createMTree(parents, n, &ind);
	for (int i = 0; i < ind; i++)
		MPreOrder1(roots[i], 0);
	GraphT g2;
	g2.size = n;
	g2.nodes = (ListT **)calloc(n + 2, sizeof(ListT *));
	for (int i = 0; i < n; i++)
	{
		g2.nodes[i] = createList(i + 1);
	}
	addEdgeOriented(g2.nodes, 1, 2);
	addEdgeOriented(g2.nodes, 2, 3);
	addEdgeOriented(g2.nodes, 3, 1);
	addEdgeOriented(g2.nodes, 1, 9);
	addEdgeOriented(g2.nodes, 2, 6);
	addEdgeOriented(g2.nodes, 2, 4);
	addEdgeOriented(g2.nodes, 4, 5);
	addEdgeOriented(g2.nodes, 5, 4);
	addEdgeOriented(g2.nodes, 6, 7);
	addEdgeOriented(g2.nodes, 7, 8);
	addEdgeOriented(g2.nodes, 8, 6);
	addEdgeOriented(g2.nodes, 7, 5);
	printf("\n\n");
	for (int i = 0; i < n; i++)
		printList(g2.nodes[i]);
	printf("\n\n");
	tarjan(g2,true);
	printf("\n\n");
	GraphT g3;
	g3.size = 6;
	g3.nodes = (ListT **)calloc(n + 2, sizeof(ListT *));
	for (int i = 0; i < n; i++)
	{
		g3.nodes[i] = createList(i + 1);
	}
	addEdgeOriented(g3.nodes, 6, 1);
	addEdgeOriented(g3.nodes, 5, 1);
	addEdgeOriented(g3.nodes, 6, 3);
	addEdgeOriented(g3.nodes, 5, 2);
	addEdgeOriented(g3.nodes, 3, 4);
	addEdgeOriented(g3.nodes, 4, 2);
	for (int i = 0; i < g2.size; i++)
		printList(g3.nodes[i]);
	printf("\n");
	ListT *sorted = topologicalSort(g3);
	NodeT *walker = sorted->first;
	while (walker != NULL)
	{
		printf("%d/%d ", walker->key, walker->fin);
		walker = walker->next;
	}
	//bfs_demo(g1);
	//MPreOrder1(root, 0);
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
		if (addEdgeNOriented(graph.nodes, random[0], random[1]) == true)
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
		dfs(g);
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
		dfs(g);
		freeGraph(g);
		profiler.countOperation("variableVertices", n, ops);
	}
	profiler.showReport();
}

int main()
{
    //std::cout << "Hello World!\n"; 
	demo();
	//analysis();
}

