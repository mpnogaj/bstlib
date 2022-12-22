#include <stdio.h>
#include "bst.h"

int main()
{
	BST_t* bst = createTree();
	addNode(bst, 1);
	addNode(bst, 2);
	addNode(bst, 3);
	addNode(bst, 4);
	addNode(bst, 5);
	addNode(bst, 6);
	/*//printf("%d\n", findNode(bst, 8));
	//printf("%d\n", findNode(bst, 10));
	//printf("%d\n", findNode(bst, -1));
	//printf("%d\n", findNode(bst, 5));*/
	BST_t* bst2 = copyTree(bst);

	addNode(bst2, 2137);
	addNode(bst, 69);

	//printf("%d\n", findNode(bst2, 5));


	//printf("%d\n", findNode(bst, 2137));
	//printf("%d\n", findNode(bst2, 2137));
	//printf("%d\n", findNode(bst2, 69));
	

	printf("%d\n", findNode(bst2, 2137));
	deleteNode(bst2, 2136);
	printf("%d\n", findNode(bst2, 2136));

	deleteTree(bst);
	deleteTree(bst2);

	return 0;
}