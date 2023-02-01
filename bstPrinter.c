#include "bstPrinter.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int *updateWidths(int *widths, int newLvl);
void printLevel(int lvl, BSTNode **tab, int *widths);
int printConnection(int lvl, BSTNode ***tab, int **widths);
int lastLevel(BSTNode **tab, int size);
int isLeaf(BSTNode *node);
int idLen(int id);
int fast10Pow(int x);
int min(int a, int b);
int max(int a, int b);

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