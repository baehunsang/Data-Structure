#include <stdlib.h> // malloc, atoi, rand
#include <stdio.h>
#include <assert.h> // assert
#include <time.h> // time

////////////////////////////////////////////////////////////////////////////////
// TREE type definition
typedef struct node
{
	int			data;
	struct node	*left;
	struct node	*right;
} NODE;

typedef struct
{
	NODE	*root;
} TREE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a tree head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
TREE* BST_Create(void) {
	TREE* newHead = (TREE*)malloc(sizeof(TREE));
	newHead->root = NULL;
	return newHead;
}

/* Deletes all data in tree and recycles memory
*/
void BST_Destroy(TREE* pTree);

/* internal function (not mandatory)
*/
static void _destroy(NODE* root) {
	if (root == NULL) {
		return;
	}
	if (root->left == NULL && root->right == NULL) { // if the argument is a leaf node.
		free(root);
		return;
	}
	else {
		_destroy(root->left);
		_destroy(root->right);
		free(root);
	}

}

/* Inserts new data into the tree
	return	1 success
			0 overflow
*/
int BST_Insert( TREE *pTree, int data);

/* internal function (not mandatory)
*/
static void _insert( NODE *root, NODE *newPtr);

NODE* _makeNode(int data) {
	NODE* newNode = (NODE*)malloc(sizeof(NODE));
	newNode->data = data;
	newNode->left = NULL;
	newNode->right = NULL;
	return newNode;
}

/* Deletes a node with dltKey from the tree
	return	1 success
			0 not found
*/
int BST_Delete( TREE *pTree, int dltKey);

/* internal function
	success is 1 if deleted; 0 if not
	return	pointer to root
*/
static NODE *_delete( NODE *root, int dltKey, int *success);

/* Retrieve tree for the node containing the requested key
	return	address of data of the node containing the key
			NULL not found
*/
int *BST_Retrieve( TREE *pTree, int key);

/* internal function
	Retrieve node containing the requested key
	return	address of the node containing the key
			NULL not found
*/
static NODE *_retrieve( NODE *root, int key);

/* prints tree using inorder traversal
*/
void BST_Traverse(TREE* pTree);
static void _traverse(NODE* root);


/* Print tree using inorder right-to-left traversal
*/
void printTree(TREE* pTree);
/* internal traversal function
*/
static void _infix_print( NODE *root, int level);

/* 
	return 1 if the tree is empty; 0 if not
*/
int BST_Empty(TREE* pTree) {
	return pTree->root == NULL;
}

/*
	return the smallest node in a BST.
*/
int _findSmallestBST(NODE* root) {
	if (root->left == NULL) {
		return root->data;
	}
	return _findSmallestBST(root->left);
}

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	TREE *tree;
	int data;
	
	// creates a null tree
	tree = BST_Create();
	
	if (!tree)
	{
		printf( "Cannot create a tree!\n");
		return 100;
	}

	fprintf( stdout, "How many numbers will you insert into a BST: ");

	int numbers;
	scanf( "%d", &numbers);

	fprintf( stdout, "Inserting: ");
	
	srand( time(NULL));
	for (int i = 0; i < numbers; i++)
	{
		data = rand() % (numbers*3) + 1; // random number (1 ~ numbers * 3)
		
		fprintf( stdout, "%d ", data);
		
		// insert funtion call
		int ret = BST_Insert( tree, data);
		if (!ret) break;
 	}
	fprintf( stdout, "\n");
			
	// inorder traversal
	fprintf( stdout, "Inorder traversal: ");
	BST_Traverse( tree);
	fprintf( stdout, "\n");
	
	// print tree with right-to-left infix traversal
	fprintf( stdout, "Tree representation:\n");
	printTree(tree);
	
	while (1)
	{
		fprintf( stdout, "Input a number to delete: "); 
		int num;
		int ret = scanf( "%d", &num);
		if (ret != 1) break;
		
		ret = BST_Delete( tree, num);
		if (!ret)
		{
			fprintf( stdout, "%d not found\n", num);
			continue;
		}
		
		// print tree with right-to-left infix traversal
		fprintf( stdout, "Tree representation:\n");
		printTree(tree);
		
		if (BST_Empty( tree))
		{
			fprintf( stdout, "Empty tree!\n");
			break;
		}
	}
	
	BST_Destroy( tree);

	return 0;
}


static void _insert(NODE* root, NODE* newPtr) {
	if (root->data > newPtr ->data) {
		if (root ->left != NULL) {
			_insert(root->left, newPtr);
			return;
		}
		root->left = newPtr; // Escape reculsion. 
	}
	else {
		if (root->right != NULL) {
			_insert(root->right, newPtr);
			return;
		}
		root->right = newPtr; // Escape reculsion.
	}	
}

int BST_Insert(TREE* pTree, int data) {
	if (pTree->root == NULL) {
		pTree->root = _makeNode(data);
		if (!(pTree->root)) {
			return 0;
		}
	}
	else {
		NODE* newNode = _makeNode(data);
		if (!newNode) {
			return 0;
		}
		_insert(pTree->root, newNode);
	}
	return 1;
}

static NODE* _delete(NODE* root, int dltKey, int* success) {
	if (root ->data == dltKey) {
		if (root->left == NULL) {
			NODE* retRoot = root->right;
			free(root);
			*success = 1;
			return retRoot;
		}
		else if (root->right == NULL) {
			NODE* retRoot = root->left;
			free(root);
			*success = 1;
			return retRoot;
		}
		else {
			int smallest = _findSmallestBST(root->right);
			root->data = smallest;
			root->right = _delete(root->right, smallest, success);
			return root;
		}
	}
	else {
		if (root ->data > dltKey) {
			if (root->left != NULL) {
				NODE* leftRoot=_delete(root->left, dltKey, success);
				root->left = leftRoot;
				return root;
			}
			*success = 0;
			return root;
		}
		else {
			if (root->right != NULL) {
				NODE* rightRoot = _delete(root->right, dltKey, success);
				root->right = rightRoot;
				return root;
			}
			*success = 0;
			return root;
		}
	}
}

/* Deletes a node with dltKey from the tree
	return	1 success
			0 not found
*/
int BST_Delete(TREE* pTree, int dltKey) {
	int success;
	pTree->root = _delete(pTree->root, dltKey, &success);
	return success;
}

static void _infix_print(NODE* root, int level) {
	if (root == NULL) {
		return;
	}
	else {
		if (root->left == NULL && root->right == NULL) {
			for (int i = 0; i < level; i++) {
				printf("\t");
			}
			printf("%d\n", root->data);
			return;
		}
		_infix_print(root->right, level + 1);
		for (int i = 0; i < level; i++) {
			printf("\t");
		}
		printf("%d\n", root->data);
		_infix_print(root->left, level + 1);
	}
}

static void _traverse(NODE* root) {
	if (!root) {
		return;
	}
	else {
		if (root->left == NULL && root->right == NULL) {
			printf("%d ", root->data);
			return;
		}
		else {
			_traverse(root->left);
			printf("%d ", root->data);
			_traverse(root->right);
		}
	}
}

void BST_Destroy(TREE* pTree) {
	if (pTree)
	{
		_destroy(pTree->root);
	}

	free(pTree);
}

void BST_Traverse(TREE* pTree) {
	_traverse(pTree->root);
	printf("\n");
	return;
}

void printTree(TREE* pTree) {
	_infix_print(pTree->root, 0);
	return;
}