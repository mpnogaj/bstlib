#include "bst.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

BSTNode *createBSTNode(int value, BSTNode *left, BSTNode *right);
BSTNode *copySubtree(BSTNode *node);
void deleteSubtree(BSTNode *node);
BSTNode *findBSTNode(BSTNode *root, int value, BSTNode **parent);
BSTNode *deleteBSTNode(BSTNode *parent, BSTNode *node);
int lastLevel(BSTNode **tab, int size);

int *updateWidths(int *widths, int newLvl);
void printLevel(int lvl, BSTNode **tab, int *widths);
int printConnection(int lvl, BSTNode ***tab, int **widths);

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

void printTree(BST_t *tree, int width)
{
	if (tree == NULL)
		return;

	int *widths = calloc(1, sizeof(int));
	BSTNode **buff = calloc(1, sizeof(BSTNode));

	buff[0] = tree->root;
	widths[0] = width;

	int maxH = log2(width / 2);

	for (int i = 0; i <= maxH; i++)
	{
		printLevel(i, buff, widths);
		if (i == maxH || lastLevel(buff, 1<<i))
			break;
		int res = printConnection(i, &buff, &widths);
		if (!res)
			break;
	}

	free(buff);
	free(widths);
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

int lastLevel(BSTNode **tab, int size)
{
	for (int i = 0; i < size; i++)
		if (tab[i] != NULL && (tab[i]->left != NULL || tab[i]->right != NULL))
			return 0;
	return 1;
}

int isLeaf(BSTNode *node)
{
	if (node == NULL)
		return 0;
	return node->left == NULL && node->right == NULL;
}

int idLen(int id)
{
	if (id == 0)
		return 1;
	return log10(abs(id)) + 1 + (id < 0 ? 1 : 0);
}

int fast10Pow(int x)
{
	if (x == 0)
		return 1;
	if (x == 1)
		return 10;
	if (x % 2 == 0)
	{
		int w = fast10Pow(x / 2);
		return w * w;
	}
	else
		return 10 * fast10Pow(x - 1);
}

int min(int a, int b)
{
	return a < b ? a : b;
}

int max(int a, int b)
{
	return a > b ? a : b;
}

int *updateWidths(int *widths, int newLvl)
{
	int n = 1 << (newLvl - 1);
	int m = n * 2;
	int *res = (int *)calloc(m, sizeof(int));
	if (res == NULL)
		return NULL;

	int i = 0;
	for (int j = 0; j < m; j += 2)
	{
		int parentW = widths[i++] - 1;
		res[j] = parentW / 2 + parentW % 2;
		res[j + 1] = parentW - res[j];
	}
	return res;
}

BSTNode **updateNodes(BSTNode **nodes, int newLvl)
{
	int n = 1 << (newLvl - 1);
	int m = n * 2;
	BSTNode **res = (BSTNode **)calloc(m, sizeof(BSTNode *));
	if (res == NULL)
		return NULL;

	int i = 0;
	for (int j = 0; j < m; j += 2)
	{
		res[j] = nodes[i] == NULL ? NULL : nodes[i]->left;
		res[j + 1] = nodes[i] == NULL ? NULL : nodes[i]->right;
		i++;
	}

	return res;
}

void printLevel(int lvl, BSTNode **nodes, int *widths)
{
	if (nodes == NULL)
		return;
	int ids = 1<<lvl;

	int maxH = 1;
	for(int i = 0; i < ids; i++)
	{
		if(nodes[i] == NULL) continue;
		int len = idLen(nodes[i]->value);

		if(len > widths[i])
			maxH = max(maxH, len);
	}

	for(int h = 0; h < maxH; h++)
	{
		for (int i = 0; i < ids; i++)
		{
			if(i > 0)
				putchar(' ');

			int pos = 0;
			if(nodes[i] == NULL)
			{
				for(; pos < widths[i]; pos++)
					putchar(' ');
				continue;
			}

			int mid = widths[i] / 2;

			int len = idLen(nodes[i]->value);
			if(len > widths[i])
			{
				for (; pos < mid; pos++)
					putchar(' ');

				char c = '|';

				if(h == 0 && nodes[i]->value < 0)
					c = '-';
				else if(len > h)
					c = (abs(nodes[i]->value) / fast10Pow(len - h - 1)) % 10 + '0';
				else if(isLeaf(nodes[i]))
					c = ' ';

				putchar(c);
				pos++;

				for (; pos < widths[i]; pos++)
					putchar(' ');
			}
			else
			{
				if (h == 0)
				{
					int l = mid - len / 2;

					for (; pos < l; pos++)
						putchar(' ');
					printf("%d", nodes[i]->value);
					pos += idLen(nodes[i]->value);
					for (; pos < widths[i]; pos++)
						putchar(' ');
				}
				else
				{
					for (; pos < mid; pos++)
						putchar(' ');
					putchar((!isLeaf(nodes[i])) ? '|' : ' ');
					pos++;
					for (; pos < widths[i]; pos++)
						putchar(' ');
				}
			}
		}
		putchar('\n');
	}
}

int printConnection(int lvl, BSTNode ***nodes, int **widths)
{
	int *currentW = *widths;
	int *newW = updateWidths(currentW, lvl + 1);
	if (newW == NULL)
		return 0;

	BSTNode **currentN = *nodes;
	BSTNode **newN = updateNodes(currentN, lvl + 1);

	if (newN == NULL)
		return 0;

	int ids = 1 << lvl;
	for (int i = 0; i < ids; i++)
	{
		if(i > 0)
			putchar(' ');
		int pos = 0;
		
		if(currentN[i] == NULL)
		{
			for(; pos < currentW[i]; pos++)
				putchar(' ');
			continue;
		}

		int mid = currentW[i] / 2;
		int midL = newW[i * 2] / 2;
		int midR = newW[i * 2 + 1] / 2;

		for (; pos < midL; pos++)
			putchar(' ');

		putchar(newN[i * 2] == NULL ? ' ' : '/');
		pos++;

		for (; pos < mid; pos++)
			putchar(newN[i * 2] == NULL ? ' ' : '-');

		if(newN[i * 2] != NULL && newN[i * 2 + 1] != NULL)
			putchar('^');
		else if(newN[i * 2] == NULL && newN[i * 2 + 1] == NULL)
			putchar(' ');
		else if(newN[i * 2] == NULL)
			putchar('\\');
		else
			putchar('/');
		pos++;

		for (; pos < mid + midR + 1; pos++)
			putchar(newN[i * 2 + 1] == NULL ? ' ' : '-');

		putchar(newN[i * 2 + 1] == NULL ? ' ' : '\\');
		pos++;

		for (; pos < currentW[i]; pos++)
			putchar(' ');
	}
	putchar('\n');

	free(currentW);
	free(currentN);

	*widths = newW;
	*nodes = newN;
	return 1;
}