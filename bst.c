#include "bst.h"
#include <stdlib.h>

BSTNode *createBSTNode(int value, BSTNode *left, BSTNode *right);
BSTNode *copySubtree(BSTNode *node);
void deleteSubtree(BSTNode *node);
BSTNode *findBSTNode(BSTNode *root, int value, BSTNode **parent);
int deleteBSTNode(BSTNode **parent, BSTNode *node);

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
	return findBSTNode(tree->root, value, NULL) != NULL;
}

int deleteNode(BST_t *tree, int value)
{
	BSTNode *parent;
	BSTNode *nodeToDelete = findBSTNode(tree->root, value, &parent);
	return deleteBSTNode(&parent, nodeToDelete);
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

BSTNode *findBSTNode(BSTNode *root, int value, BSTNode **parent)
{
	BSTNode *prev = NULL;
	BSTNode *walker = root;

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

BSTNode *findMinValueNode(BSTNode *root, BSTNode **parent)
{
	BSTNode *walker = root;
	while(walker->left != NULL) 
	{
		if(parent != NULL) *parent = walker;
		walker = walker->left;
	}
	return walker;
}

int deleteBSTNode(BSTNode **parent, BSTNode *node)
{
	if(node->left == NULL || node->right == NULL)
	{
		BSTNode *child = node->left == NULL ? node->right : node->left;
		if(parent == NULL) *parent = child;
		else
		{
			if((*parent)->left == node) (*parent)->left = child;
			else (*parent)->right = child;
		}
		free(node);
	}
	else
	{
		BSTNode *minNodeParent;
		BSTNode *minNode = findMinValueNode(node->right, &minNodeParent);
		node->value = minNode->value;
		return deleteBSTNode(&minNodeParent, minNode);
	}
	return 1;
}

int isLeaf(BSTNode *node)
{
	return node->left != NULL && node->right != NULL;
}