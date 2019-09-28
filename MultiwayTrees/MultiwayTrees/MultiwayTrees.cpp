/*
the conversion from a parent vector to a multiway tree takes place in liniar time (O(n)) (actually O(n^2) because of the realloc)
but it takes additional space, an array of already created nodes (O(n))

the conversion from a multiway tree to a binary can be done in two ways: with an auxiliary array which stores the preorder traversal
of the mulitway tree
or directly, using recursion

both methods are liniar regarding time (O(n)), but the direct method doesn't take extra space, unlike the first one
which holds an array with all the keys
*/

#include "pch.h"
#include <iostream>

typedef struct n
{
	int key;
	n **children;
	int nextChild;
	int childSize;
}MNodeT;

typedef struct m
{
	int key;
	m *left, *right;
}BNodeT;

MNodeT *initNode(int key)				//multiway tree node
{
	MNodeT *n = (MNodeT *)malloc(sizeof(MNodeT));
	n->key = key;
	n->nextChild = 0;
	n->children = (MNodeT **)calloc(3, sizeof(MNodeT*));
	n->childSize = 2;
	return n;
}

BNodeT *initBNode(int key)				//binary tree node
{
	BNodeT *n = (BNodeT *)malloc(sizeof(BNodeT));
	n->key = key;
	n->left = n->right = NULL;
	return n;
}

void createNode(int *parent, int i, MNodeT **taken, MNodeT **root)		//recursive function
{																		//which fills the auxiliary array
	if (taken[i] != NULL)												//it goes up to the root (and it sets it)
		return;															//than it links the nodes to their parents
	taken[i] = initNode(i);												//when exiting recursion
	if (parent[i] == -1)
	{
		*root = taken[i];
		return;
	}
	if (taken[parent[i]] == NULL)
	{
		createNode(parent, parent[i], taken, root);
	}
	MNodeT *n = taken[parent[i]];
	if (n->nextChild >= n->childSize)
	{
		n->childSize += 3;
		n->children = (MNodeT **)realloc(n->children, (n->childSize + 1) * sizeof(MNodeT*));
	}
	n->children[n->nextChild++] = taken[i];
}

MNodeT *createMTree(int *parent, int n)									//function which initializes the array of nodes
{																		//and than goes through the parent vector 
	MNodeT **taken = (MNodeT **)calloc(n + 2, sizeof(MNodeT*));			//creating each corresponding node
	for (int i = 1; i <= n; i++)
	{
		taken[i] = NULL;
	}

	MNodeT *root = NULL;
	for (int i = 1; i <= n; i++)
	{
		createNode(parent, i, taken, &root);
	}
	return root;
}

BNodeT *createBTreeAux(int *arr, int *i)				//function which goes through the preorder array 
{
	int key = arr[*i];
	if (key == -1)
		return NULL;
	BNodeT *root = initBNode(key);
	*i = *i + 1;
	root->left = createBTreeAux(arr, i);
	*i = *i + 1;
	root->right = createBTreeAux(arr, i);
	return root;
}
void MPreOrder(MNodeT *root, int *arr, int *i)		//function which creates the preorder array
{
	if (root == NULL)
	{
		arr[*i] = -1;
		*i = *i + 1;
		return;
	}
	arr[*i] = root->key;
	*i = *i + 1;
	for (int j = 0; j < root->nextChild; j++)
		MPreOrder(root->children[j], arr, i);
	arr[*i] = -1;
	*i = *i + 1;
}

BNodeT *createBTree(MNodeT *p)					//function which creates the binary tree from the preorder array
{
	int *arr = (int *)calloc(1000, sizeof(int));
	int *i = (int *)malloc(sizeof(int));
	*i = 0;
	MPreOrder(p, arr, i);
	arr[*i] = -1;
	*i = *i + 1;
	*i = 0;
	BNodeT *root = createBTreeAux(arr, i);
	free(arr);
	free(i);
	return root;
}

void addChildren(BNodeT *bRoot, MNodeT *mRoot)			//function which creates the left child of a binary tree
{														//than its right child, but goes recursively for each
	if (mRoot->children[0] == NULL)						//of the multiway's next children to the right of the binary tree
		return;
	bRoot->left = initBNode(mRoot->children[0]->key);
	BNodeT *current = bRoot->left;
	for (int i = 1; i < mRoot->nextChild; i++)
	{
		current->right = initBNode(mRoot->children[i]->key);
		addChildren(current, mRoot->children[i-1]);
		current = current->right;
	}
}

BNodeT *createBTree2(MNodeT *p)
{
	BNodeT *root = initBNode(p->key);
	addChildren(root, p);
	return root;
}

void MPreOrder1(MNodeT *root,int level)
{
	if (root == NULL)
		return;
	printf("%*s%d\n", 3 * level, "", root->key);
	for (int i = 0; i < root->nextChild; i++)
		MPreOrder1(root->children[i],level+1);
}

void showTreePreorder(BNodeT *pNode, int level)
{
	if (pNode == NULL)
		return;
	printf("%*s%d\n", 3 * level, "", pNode->key);
	showTreePreorder(pNode->left, level + 1);
	showTreePreorder(pNode->right, level);
}

void demo1()
{
	int parent[11] = { 0,2,7,5,2,7,7,-1,5,2,1 };
	int sze = 10;
	for (int i = 1; i <= sze; i++)
		printf("%d ", parent[i]);
	printf("\n\n");
	MNodeT *root = createMTree(parent, sze);
	MPreOrder1(root, 0);
	printf("\n");
	//BNodeT *bRoot1 = createBTree2(root);
	//showTreePreorder(bRoot1, 0);
	//printf("\n");
	BNodeT *bRoot2 = createBTree(root);
	showTreePreorder(bRoot2, 0);
	printf("\n");
}

void demo2()
{
	int parent[16] = { 0, -1, 1, 1, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5 };
	int sze = 15;
	for (int i = 1; i <= sze; i++)
		printf("%d ", parent[i]);
	printf("\n\n");
	MNodeT *root = createMTree(parent, sze);
	MPreOrder1(root, 0);
	printf("\n");
	//BNodeT *bRoot1 = createBTree2(root);
	//showTreePreorder(bRoot1, 0);
	//printf("\n");
	BNodeT *bRoot2 = createBTree(root);
	showTreePreorder(bRoot2, 0);
	printf("\n");
}

int main()
{
	printf("\nFirst example:\n\n");
	demo1();
	printf("\nSecond example:\n\n");
	demo2();
}

