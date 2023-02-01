#include "bst.h"
#include <stdlib.h>

BSTNode *createBSTNode(int value, BSTNode *left, BSTNode *right);
BSTNode *copySubtree(BSTNode *node);
void deleteSubtree(BSTNode *node);
BSTNode *findBSTNode(BSTNode *root, int value, BSTNode **parent);
BSTNode *deleteBSTNode(BSTNode *parent, BSTNode *node);

BST_t *createTree()
{
	BST_t *tree = (BST_t *)calloc(1, sizeof(BST_t));
	return tree;
}

BST_t *copyTree(BST_t *tree)
{
	if (tree == NULL)
		return NULL;

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
	if (tree == NULL)
		return;
	deleteSubtree(tree->root);
	free(tree);
}

int addNode(BST_t *tree, int value)
{
	if (tree == NULL)
		return 0;

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
	if (tree == NULL)
		return 0;
	return findBSTNode(tree->root, value, NULL) != NULL;
}

int deleteNode(BST_t *tree, int value)
{
	if (tree == NULL)
		return 0;
	BSTNode *parent;
	BSTNode *nodeToDelete = findBSTNode(tree->root, value, &parent);
	if (nodeToDelete == NULL)
		return 0;
	if (parent == NULL)
		tree->root = deleteBSTNode(parent, nodeToDelete);
	else
		parent = deleteBSTNode(parent, nodeToDelete);
	return 1;
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
	if (node == NULL)
		return;
	deleteSubtree(node->left);
	deleteSubtree(node->right);
	free(node);
}

/**
 * @brief Finds BSTNode with given node, in given subtree
 * 
 * @param root root of the subtree
 * @param value value to find
 * @param parent pointer to node parent, NULL if node is NULL or root
 * @return BSTNode* node, NULL if not found
 */
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

	if (parent != NULL)
		(*parent) = prev;
	return walker;
}

/**
 * @brief Searches for node with lowest value in given subtree
 * 
 * @param root root of the subtree
 * @param parent parent of the minimum node !!IMPORTANT!! requires starting parent
 * @return BSTNode* minimum node
 */
BSTNode *findMinValueNode(BSTNode *root, BSTNode **parent)
{
	if(root == NULL) return NULL;
	BSTNode *walker = root;
	while (walker->left != NULL)
	{
		if (parent != NULL)
			*parent = walker;
		walker = walker->left;
	}
	return walker;
}

BSTNode *deleteBSTNode(BSTNode *parent, BSTNode *node)
{
	if (node->left == NULL || node->right == NULL)
	{
		BSTNode *child = node->left == NULL ? node->right : node->left;
		BSTNode *ret = child;

		if (parent != NULL)
		{
			if (parent->left == node)
				parent->left = child;
			else
				parent->right = child;
			ret = parent;
		}
		free(node);

		return ret;
	}
	else
	{
		BSTNode *minNodeParent = node;
		BSTNode *minNode = findMinValueNode(node->right, &minNodeParent);
		node->value = minNode->value;
		free(minNode);
		if(minNodeParent == node) minNodeParent->right = NULL;
		else minNodeParent->left = NULL;
		return node;
	}
}
