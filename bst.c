#include "bst.h"
#include <stdlib.h>

BSTNode *createBSTNode(int value, BSTNode *left, BSTNode *right);
BSTNode *copySubtree(BSTNode *node);
void deleteSubtree(BSTNode *node);
BSTNode *findBSTNode(BST_t *tree, int value, BSTNode **parent);

BST_t *createTree()
{
	BST_t *tree = (BST_t *)calloc(1, sizeof(BST_t));
	return tree;
}

BST_t *copyTree(BST_t *tree)
{
	BST_t *newTree = createTree();
	if (newTree == NULL)
		return NULL;
	newTree->root = copySubtree(tree->root);
	if (newTree->root == NULL && tree->root != NULL)
		return NULL;

	return newTree;
}

void deleteTree(BST_t *tree)
{
	deleteSubtree(tree->root);
	free(tree);
}

int addNode(BST_t *tree, int value)
{
	BSTNode *parent = NULL;
	BSTNode *walker = tree->root;
	BSTNode *newNode = createBSTNode(value, NULL, NULL);

	if (newNode == NULL)
		return 0;

	while (walker != NULL)
	{
		parent = walker;
		if (value <= walker->value)
			walker = walker->left;
		else
			walker = walker->right;
	}

	// new node is root
	if (parent == NULL)
	{
		tree->root = newNode;
		return 1;
	}

	if (value <= parent->value)
		parent->left = newNode;
	else
		parent->right = newNode;

	return 1;
}

int findNode(BST_t *tree, int value)
{
	return findBSTNode(tree, value, NULL) != NULL;
}

/* PRIVATE METHODS */
BSTNode *copySubtree(BSTNode *node)
{
	BSTNode *leftCpy = NULL, *rightCpy = NULL;
	if (node->left != NULL)
	{
		leftCpy = copySubtree(node->left);
		if (leftCpy == NULL)
			return NULL;
	}
	if (node->right != NULL)
	{
		rightCpy = copySubtree(node->right);
		if (rightCpy == NULL)
			return NULL;
	}

	return createBSTNode(node->value, leftCpy, rightCpy);
}

BSTNode *createBSTNode(int value, BSTNode *left, BSTNode *right)
{
	BSTNode *res = calloc(1, sizeof(BSTNode));
	if (res == NULL)
		return NULL;
	res->left = left;
	res->right = right;
	res->value = value;

	return res;
}

void deleteSubtree(BSTNode *node)
{
	if (node->left != NULL)
		deleteSubtree(node->left);
	if (node->right != NULL)
		deleteSubtree(node->right);
	free(node);
}

BSTNode *findBSTNode(BST_t *tree, int value, BSTNode **parent)
{
	BSTNode *prev = NULL;
	BSTNode *walker = tree->root;

	while (walker != NULL && walker->value != value)
	{
		prev = walker;
		if (value < walker->value)
			walker = walker->left;
		else
			walker = walker->right;
	}

	if(parent != NULL) (*parent) = prev; 
	return walker;
}

int isLeaf(BSTNode *node)
{
	return node->left != NULL && node->right != NULL;
}