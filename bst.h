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

/**
 * @brief Create a Tree object
 * 
 * @return Pointer to new bst tree struct
 */
BST_t *createTree();

/**
 * @brief Copys tree
 * 
 * @param tree pointer to tree to be copied
 * @return BST_t* Pointer to new copy of the tree
 */
BST_t *copyTree(BST_t *tree);

/**
 * @brief Deletes tree and it's contents
 * 
 * @param tree Pointer to tree to be deleted
 */
void deleteTree(BST_t *tree);

/** 
 * @brief Adds node to given BST
 * 
 * @param tree pointer to BST to which to add node
 * @param value new node's value
 * @return true if successfull, false if not
 */
int addNode(BST_t *tree, int value);

/** 
 * @brief Searches for node with given value in BST
 * 
 * @param tree pointer to BST in which to look for a node with given value 
 * @param value node value 
 * @return true if found, false if not
 */
int findNode(BST_t *tree, int value);

/**
 * @brief Deletes node with given value (if exists)
 * 
 * @param tree pointer to BST in which to delete node with given value 
 * @param value node value
 * @return true if deleted, false if not 
 */
int deleteNode(BST_t *tree, int value);

/**
 * @brief Prints tree
 * 
 * @param tree pointer to tree to print 
 * @param width terminal width
 */
void printTree(BST_t *tree, int width);

#endif // BST
