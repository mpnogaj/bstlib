#include "bst.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

BSTNode *createBSTNode(int value, BSTNode *left, BSTNode *right);
BSTNode *copySubtree(BSTNode *node);
void deleteSubtree(BSTNode *node);
BSTNode *findBSTNode(BSTNode *root, int value, BSTNode **parent);
BSTNode *deleteBSTNode(BSTNode *parent, BSTNode *node);
int arrayEmpty(BSTNode **tab, int size);

int *updateWidths(int *widths, int newLvl);
void printLevel(int n, BSTNode **tab, int *widths);
void printConnection(int lvl, BSTNode **tab, int *widths);

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

	int size = 1;
	int maxH = log2(width);
	BSTNode **buff[2];
	buff[0] = calloc(size, sizeof(BSTNode));

	// cannot realloc ptr to NULL
	buff[1] = calloc(size, sizeof(BSTNode));
	int *widths = calloc(size, sizeof(int));

	widths[0] = width;
	buff[0][0] = tree->root;
	void *tmp = NULL;
	for (int i = 0; i <= maxH && arrayEmpty(buff[0], size) == 0; i++)
	{
		if (i > 0)
		{
			printConnection(i - 1, buff[0], widths);

			// Update widths
			tmp = updateWidths(widths, i);
			if (tmp == NULL)
			{
				fprintf(stderr, "Nie udalo sie zaalokowac wystarczjacej ilosci pamieci. Przerywanie rysowania\n");
				break;
			}
			free(widths);
			widths = (int *)tmp;

			// Extend helper buffer
			tmp = realloc(buff[1], size * 2 * sizeof(BSTNode));
			if (tmp == NULL)
			{
				fprintf(stderr, "Nie udalo sie zaalokowac wystarczjacej ilosci pamieci. Przerywanie rysowania\n");
				break;
			}
			buff[1] = (BSTNode **)tmp;

			// fill helper buffer
			for (int j = 0; j < size; j++)
			{
				buff[1][j * 2] = NULL;
				buff[1][j * 2 + 1] = NULL;
				BSTNode *node = buff[0][j];
				if (node == NULL)
					continue;
				buff[1][j * 2] = node->left;
				buff[1][j * 2 + 1] = node->right;
			}

			// extend main buffer
			size *= 2;
			tmp = realloc(buff[0], size * sizeof(BSTNode));
			if (tmp == NULL)
			{
				fprintf(stderr, "Nie udalo sie zaalokowac wystarczjacej ilosci pamieci. Przerywanie rysowania\n");
				break;
			}
			buff[0] = (BSTNode **)tmp;

			// move data from helper to main buffer
			for (int j = 0; j < size; j++)
				buff[0][j] = buff[1][j];

			if(arrayEmpty(buff[0], size)) break;
		}
		printLevel(i, buff[0], widths);
	}

	free(widths);
	free(buff[0]);
	if (buff[1] != NULL)
		free(buff[1]);
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

BSTNode *findMinValueNode(BSTNode *root, BSTNode **parent)
{
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
		minNodeParent = deleteBSTNode(minNodeParent, minNode);
		return minNodeParent;
	}
}

int arrayEmpty(BSTNode **tab, int size)
{
	int empty = 1;
	for (int i = 0; i < size; i++)
		if (tab[i] != NULL)
			empty = 0;
	return empty;
}

int isLeaf(BSTNode *node)
{
	if (node == NULL)
		return 0;
	return node->left == NULL && node->right == NULL;
}

int idLen(int id)
{
	if(id == 0) return 1;
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

void printLevel(int n, BSTNode **tab, int *widths)
{
	int ids = 1 << n;
	int maxH = 1;
	for (int i = 0; i < ids; i++)
	{
		if (tab[i] == NULL)
			continue;
		maxH = max(maxH, idLen(tab[i]->value) > widths[i] ? idLen(tab[i]->value) : 1);
	}
	for (int h = 0; h < maxH; h++)
	{
		for (int i = 0; i < ids; i++)
		{
			if (tab[i] == NULL)
			{
				for (int j = 0; j < widths[i]; j++)
					putchar(' ');
				putchar(' ');
				continue;
			}
			int vertical = idLen(tab[i]->value) > widths[i];

			if (vertical)
			{
				int mid = widths[i] / 2;
				int pos = 0;
				for (; pos < mid; pos++)
					putchar(' ');

				if (h < idLen(tab[i]->value))
				{
					int res = (tab[i]->value / fast10Pow(idLen(tab[i]->value) - 1 - h));
					putchar(res % 10 + '0');
				}
				// leaf check when printing '|'
				else if (!isLeaf(tab[i]))
					putchar('|');
				else
					putchar(' ');

				pos++;
				for (; pos < widths[i]; pos++)
					putchar(' ');
			}
			else
			{
				int mid = widths[i] / 2;
				int pos = 0;
				if (h == 0)
				{

					int l = mid - (idLen(tab[i]->value) / 2);

					for (; pos < l; pos++)
						putchar(' ');
					printf("%d", tab[i]->value);
					pos += idLen(tab[i]->value);
					for (; pos < widths[i]; pos++)
						putchar(' ');
				}
				else
				{
					for (; pos < mid; pos++)
						putchar(' ');
					putchar((!isLeaf(tab[i])) ? '|' : ' ');
					pos++;
					for (; pos < widths[i]; pos++)
						putchar(' ');
				}
			}
			if (i != ids - 1)
				putchar(' ');
		}
		putchar('\n');
	}
}

void printConnection(int lvl, BSTNode **tab, int *widths)
{
	int ids = 1 << lvl;
	int stop = 0;
	for (int offset = 0; !stop; offset++)
	{
		stop = 1;
		for (int i = 0; i < ids; i++)
		{
			if (tab[i] == NULL)
			{
				for (int pos = 0; pos < widths[i]; pos++)
					putchar(' ');
				putchar(' ');
				continue;
			}

			int mid = widths[i] / 2;

			int rightWidth = (widths[i] - 1) / 2;
			int leftWidth = (widths[i] - 1) - rightWidth;

			int rightMid = rightWidth / 2;
			int leftMid = leftWidth / 2;

			int pos = 0;
			for (; pos < max(mid - offset, leftMid); pos++)
				putchar(' ');
			if (offset > 0 && tab[i]->left != NULL)
			{
				putchar(leftMid >= mid - offset ? '|' : '/');
				if (leftMid < mid - (offset + 1))
					stop = 0;
				pos++;
			}
			else if (offset > 0)
			{
				putchar(' ');
				pos++;
			}

			for (; pos < mid; pos++)
				putchar(' ');

			if (offset > 0)
				putchar(' ');
			else
			{
				if (tab[i]->left != NULL && tab[i]->right != NULL)
					putchar('^');
				else if (tab[i]->left != NULL)
					putchar('/');
				else if (tab[i]->right != NULL)
					putchar('\\');
				else
					putchar(' ');
			}

			pos++;
			for (; pos < mid + min(offset, rightMid + 1); pos++)
				putchar(' ');
			if (offset > 0 && tab[i]->right != NULL)
			{
				putchar(offset >= rightMid + 1 ? '|' : '\\');
				if (offset + 1 < rightMid + 1)
					stop = 0;
				pos++;
			}
			else if (offset > 0)
			{
				putchar(' ');
				pos++;
			}

			for (; pos < widths[i]; pos++)
				putchar(' ');

			putchar(' ');
			if (offset == 0)
				stop = 0;
		}
		putchar('\n');
	}
}
