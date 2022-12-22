#ifndef BST
#define BST

typedef struct BSTNode
{
	int value;
	struct BSTNode *left, *right;
} BSTNode;


typedef struct BST_t
{
	BSTNode *root;	
} BST_t;


BST_t *createTree();
BST_t *copyTree(BST_t *tree);
void deleteTree(BST_t *tree);

/// @brief Adds node to given BST
/// @param tree pointer to BST to which to add node
/// @param value new node's value
/// @return true if successfull, false if not
int addNode(BST_t *tree, int value);

/// @brief Searches for node with given value in BST
/// @param tree pointer to BST in which to look for a node with given value 
/// @param value node value 
/// @return true if found, false if not
int findNode(BST_t *tree, int value);
int deleteNode(BST_t *tree, int value);

void printTree(BSTNode* tree);

#endif // BST
