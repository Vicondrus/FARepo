/*
the building of the balanced tree takes O(n) time (because it goes trough the the whole array)

but because the tree will not be kept balanced after deleting nodes from it,
as it would take O(h) (h is the height of the tree) time to find a node
osSearch will take somewhere between O(log(n)) and O(n) time (closer to O(log(n))
as we do the search n times

the total time will be O(nlog(n))

it's the same case for osDelete (because one needs to find the node and the replacement)
updating the size for the affected nodes doesn't increase the asiptotic behaviour

*/

#include "pch.h"
#include <iostream>
#include "Profiler.h"

Profiler profiler("osTrees");

int compsS, assignsS;
int compsD, assignsD;
int compsB, assignsB;

typedef struct n
{
	n *left;
	n *right;
	int key;
	int size;
}NodeT;

typedef struct t
{
	NodeT *root;
}TreeT;

NodeT *newNode()
{
	NodeT *n = (NodeT *)malloc(sizeof(NodeT));
	n->left = NULL;
	n->right = NULL;
	n->key = 0;
	n->size = 1;
	assignsB++;
	return n;
}

NodeT *buildPBT(int *a, int left, int right)		//auxiliary recursive function
{
	if (left <= right)								//split the array into halves
	{												//keeping the middle as root
		NodeT *n = newNode();
		int aux = (left + right) / 2;
		n->key = a[aux];
		assignsB++;
		n->left = buildPBT(a, left, aux - 1);
		n->right = buildPBT(a, aux + 1, right);
		compsB += 2;
		if (n->left != NULL)
		{
			n->size += n->left->size;
		}
		if (n->right != NULL)
		{
			n->size += n->right->size;
		}
		return n;
	}
	else
		return NULL;
}

TreeT *buildTree(int n)			//main tree building function
{
	int *a = (int *)calloc(n + 1, sizeof(int));
	for (int i = 0; i < n; i++)
	{
		a[i] = i + 1;
	}
	NodeT *root = buildPBT(a, 0, n - 1);		//recursive function is called
	TreeT *tree = (TreeT *)malloc(sizeof(TreeT));
	tree->root = root;
	assignsB++;
	return tree;
}

NodeT *osSelectAux(NodeT *root, int i)			//resembles quickSelect
{												//checks how many els on left and right
	int r;										//and decides where the ith element should be
	compsS++;
	if (root->left != NULL)
		r = root->left->size + 1;
	else
		r = 1;
	compsS++;
	if (i == r)
	{
		return root;
	}
	else
	{
		compsS++;
		if (i < r)
		{
			return osSelectAux(root->left, i);
		}
		else
		{
			return osSelectAux(root->right, i - r);
		}
	}
}

NodeT *osSelect(TreeT *tree, int i)
{
	return osSelectAux(tree->root, i);
}

void showTreeAux(NodeT *root, int ord)
{
	if (root == NULL)
		return;
	if (ord == -1)
		printf("%d %d\n", root->key, root->size);
	showTreeAux(root->left, ord);
	if (ord == 0)
		printf("%d %d\n", root->key, root->size);
	showTreeAux(root->right, ord);
	if(ord==1)
		printf("%d %d\n", root->key, root->size);
}


void showTree(TreeT *tree, int ord)
{
	if (tree->root == NULL)
		printf("Tree empty\n");
	else
		showTreeAux(tree->root, ord);
}

NodeT *findSuccessor(NodeT *node)
{
	node->size--;
	node = node->right;
	node->size--;
	while (node->left != NULL)
	{
		node = node->left;
		node->size--;
	}
	return node;
}

void osDelete(TreeT *tree, NodeT* node)					//most difficult function ever encountered
{													//many, very many cases
	NodeT *root = tree->root;
	int key = node->key;
	if (root->left == NULL && root->right == NULL && root->key==key)		//if there's only the root
	{
		free(tree->root);
		tree->root = NULL;
		return;
	}
	NodeT* keeper = root, *parent = NULL;
	while (root != NULL)			//searching for the node
	{
		root->size--;				//and decrementing the sizes of the nodes until it is reached
		compsD++;
		if (root->key == key)
			break;
		compsD++;
		if (key > root->key)
		{
			
			parent = root;
			root = root->right;
		}
		else
		{
			parent = root;
			root = root->left;
		}
	}
	if (root == NULL)
	{
		root = keeper;
		printf("Can't find node with key %d\n", key);
		while (root != NULL)
		{
			root->size++;
			if (key > root->key)
			{
				root = root->right;
			}
			else
			{
				root = root->left;
			}
		}
		return;
	}
	NodeT *deleted, *child;
	if (root->left == NULL || root->right == NULL)			//if it has ANY child, than it can be "easily" deleted
	{
		compsD++;
		assignsD++;
		deleted = root;
	}
	else
	{													//if not, than search for the successor, which will take its place
		assignsD++;
		deleted = root;
		//deleted->size--;
		assignsD++;
		parent = deleted;
		assignsD++;
		deleted = deleted->right;
		//deleted->size--;
		compsD++;
		while (deleted->left != NULL)
		{
			compsD++;
			assignsD++;
			deleted->size--;
			assignsD++;
			parent = deleted;
			assignsD++;
			deleted = deleted->left;
		}
	}
	compsD++;
	if (deleted->left != NULL)
	{
		assignsD++;
		child = deleted->left;				//find the deleted node's child
	}
	else
	{
		assignsD++;
		child = deleted->right;
	}
	if (parent == NULL)			//if the deleted node is the root
	{
		compsD++;
		if (root->left == NULL || root->right == NULL)		//if it has only one child
		{													//it becomes the root
			assignsD++;
			tree->root = child;
			free(root);
		}
		else
		{													//if not, it's more complicated
			assignsD++;										//the succesor becomes the root
			tree->root->key = deleted->key;					//its child takes the successor's place
			if (child != NULL)								//and finally, the child is deleted
				deleted->key = child->key;
			assignsD++;
			//tree->root->size = child->size;
			compsD++;
			if (deleted->left != NULL && deleted->left->key == child->key)
				deleted->left = child->left;
			compsD++;
			if (deleted->right != NULL && deleted->right->key == child->key)
				deleted->right = child->right;
			assignsD++;
			if (child != NULL)
				free(child);
		}
	}
	else
	{
		compsD++;								//if it's not the root
		if (parent->left == deleted)			//the parent must be informed which will be its new child
		{
			assignsD++;
			parent->left = child;
		}
		else
		{
			assignsD++;
			parent->right = child;
		}
		root->key = deleted->key;
		assignsD++;
		//root->size = deleted->size;
		free(deleted);
	}
	//root->left = deleted->left;
	//root->right = deleted->right;
}

void prettyPrint(NodeT *pNode, int level)
{
	if (pNode == NULL)
		return;
	prettyPrint(pNode->right, level + 1);
	printf("%*s%d,%d\n", 3 * level, "", pNode->key,pNode->size);
	prettyPrint(pNode->left, level + 1);
}

void demo()
{
	TreeT *t = buildTree(11);
	printf("\n\n\n\n");
	prettyPrint(t->root, 0);
	//system("pause");
	//system("cls");
	printf("\n\n\n");
	printf("Deleting key %d which is the %dth node\n\n", osSelect(t, 6)->key,6);
	osDelete(t, osSelect(t,6));
	prettyPrint(t->root, 0);
	//system("pause");
	//system("cls");
	//printf("\n\n\n");
	//showTree(t, 0);
	//for (int i = 1; i < 10; i++)
	//{
		//printf("%d ", osSelect(t, i)->key);
	//}
	printf("\n\n\n");
	printf("Deleting key %d which is the %dth node\n\n", osSelect(t, 4)->key,4);
	osDelete(t, osSelect(t, 4));
	prettyPrint(t->root, 0);
	//system("pause");
	//system("cls");
	printf("\n\n\n");
	printf("Deleting key %d which is the %dth node\n\n", osSelect(t, 3)->key,3);
	osDelete(t, osSelect(t, 3));
	prettyPrint(t->root, 0);
	//system("pause");
	//system("cls");
	printf("\n\n\n");
	printf("Deleting key %d which is the %dth node\n\n", osSelect(t, 2)->key,2);
	osDelete(t, osSelect(t, 2));
	prettyPrint(t->root, 0);
	//system("cls");
}

void analysis()
{
	for (int n = 100; n <= 10000; n += 100)
	{
		assignsS = compsS = assignsD = compsD = assignsB = compsB = 0;
		for (int m = 0; m < 5; m++)
		{
			//int n = 50;
			printf("\r%d", n / 100);
			TreeT *t = buildTree(n);
			int aux = n;
			while (aux > 1)
			{
				int index[2];
				FillRandomArray(index, 2, 1, aux);
				osDelete(t, osSelect(t, index[0]));
				//showTree(t, 0);
				//printf("\n");
				aux--;
			}
			/*osDelete(t,t->root->key);
			showTree(t, 0);
			printf("\n");
			osDelete(t, t->root->key);
			showTree(t, 0);
			printf("\n");
			osDelete(t, t->root->key);
			showTree(t, 0);
			printf("\n");
			*/
			osDelete(t, t->root);
			//showTree(t, 0);
			//printf("\n");
		}
		profiler.countOperation("totalBuildOps", n, assignsB / 5 + compsB / 5);
		profiler.countOperation("totalSearch&DeleteOps", n, assignsS / 5 + compsS / 5 + assignsD / 5 + compsD / 5);
	}
	//profiler.createGroup("totalOps", "totalSearchOps", "totalDeleteOps");
	profiler.showReport();
}

int main()
{
	demo();
	//analysis();
}


