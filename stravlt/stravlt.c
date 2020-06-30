#define SHOW_STEP 0
#define BALANCING 1

#include <stdlib.h> // malloc, rand
#include <stdio.h>
#include <time.h> // time
#include <string.h> //strcmp, strdup

#define max(x, y)	(((x) > (y)) ? (x) : (y))

////////////////////////////////////////////////////////////////////////////////
// AVL_TREE type definition
typedef struct node
{
	char		*data;
	struct node	*left;
	struct node	*right;
	int			height;
} NODE;

typedef struct
{
	NODE	*root;
	int		count;  // number of nodes
} AVL_TREE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a AVL_TREE head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
AVL_TREE* AVL_Create(void) {
	AVL_TREE* newTree = (AVL_TREE*)malloc(sizeof(AVL_TREE));
	newTree->count = 0;
	newTree->root = NULL;
	return newTree;
}

/* Deletes all data in tree and recycles memory
*/
void AVL_Destroy(AVL_TREE* pTree);
static void _destroy(NODE* root) {
	if (root == NULL) {
		return;
	}
	if (root->left == NULL && root->right == NULL) { // if the argument is a leaf node.
		free(root->data);
		free(root);
		return;
	}
	else {
		_destroy(root->left);
		_destroy(root->right);
		free(root->data);
		free(root);
	}
}

/* Inserts new data into the tree
	return	1 success
			0 overflow
*/
int AVL_Insert( AVL_TREE *pTree, char *data);

/* internal function
	This function uses recursion to insert the new data into a leaf node
	return	pointer to new root
*/
static NODE *_insert( NODE *root, NODE *newPtr);

static NODE* _makeNode(char* data) {
	NODE* newNode = (NODE*)malloc(sizeof(NODE));
	newNode->data = strdup(data);
	newNode->height = 0;
	newNode->left = NULL;
	newNode->right = NULL;
	return newNode;
}

/* Retrieve tree for the node containing the requested key
	return	address of data of the node containing the key
			NULL not found
*/
char *AVL_Retrieve( AVL_TREE *pTree, char *key);

/* internal function
	Retrieve node containing the requested key
	return	address of the node containing the key
			NULL not found
*/
static NODE *_retrieve(NODE *root, char *key);

/* Prints tree using inorder traversal
*/
void AVL_Traverse( AVL_TREE *pTree);
static void _traverse( NODE *root);

/* Prints tree using inorder right-to-left traversal
*/
void printTree( AVL_TREE *pTree);
/* internal traversal function
*/
static void _infix_print( NODE *root, int level);

/* internal function
	return	height of the (sub)tree from the node (root)
*/
static int getHeight(NODE* root) {
	if (root) {
		return root->height;
	}
	else {
		return 0;
	}
}

/* internal function
	Exchanges pointers to rotate the tree to the right
	updates heights of the nodes
	return	new root
*/
static NODE* rotateRight(NODE* root) {
	NODE* tmp = root->left;
	root->left = (root->left)->right;	
	tmp->right = root;
	//Change the hight of initial root
	root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
	//Change the hight of tmp(left subtree of root)
	tmp->height = max(getHeight(tmp->left), getHeight(tmp->right)) + 1;
	return tmp;
}

/* internal function
	Exchanges pointers to rotate the tree to the left
	updates heights of the nodes
	return	new root
*/
static NODE* rotateLeft(NODE* root) {
	NODE* tmp = root->right;
	root->right = (root->right)->left;
	tmp->left = root;
	//Change the hight of initial root
	root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
	//Change the hight of tmp(left subtree of root)
	tmp->height = max(getHeight(tmp->left), getHeight(tmp->right)) + 1;
	return tmp;
}

/*
	Initial function
	Balance the AVL tree when the left subtree is higher.(left of left, right of left cases)
*/
NODE* _leftBalance(NODE* root) {
	if (getHeight((root->left)->left)>getHeight((root->left)->right)) {
		root = rotateRight(root); // left of left case
	}
	else {
		root->left = rotateLeft(root->left); // right of left case
		root = rotateRight(root);
	}
	return root;
}

/*
	Initial function
	Balance the AVL tree when the right subtree is higher.(left of right, right of right cases)
*/
NODE* _rightBalance(NODE* root) {
	if (getHeight((root->right)->left) > getHeight((root->right)->right)) {
		root->right = rotateRight(root->right);
		root = rotateLeft(root); // left of right
	}
	else {
		root = rotateLeft(root); //right of right
	}
	return root;
}
////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	AVL_TREE *tree;
	char str[1024];
	
	if (argc != 2)
	{
		fprintf( stderr, "Usage: %s FILE\n", argv[0]);
		return 0;
	}
	
	// creates a null tree
	tree = AVL_Create();
	
	 if (!tree)
	{
		fprintf( stderr, "Cannot create tree!\n");
		return 100;
	} 

	FILE *fp = fopen( argv[1], "rt");
	if (fp == NULL)
	{
		fprintf( stderr, "Cannot open file! [%s]\n", argv[1]);
		return 200;
	}

	while(fscanf( fp, "%s", str) != EOF)
	{

#if SHOW_STEP
		fprintf( stdout, "Insert %s>\n", str);
#endif		
		// insert function call
		AVL_Insert( tree, str);

#if SHOW_STEP
		fprintf( stdout, "Tree representation:\n");
		printTree( tree);
#endif
	}
	
	fclose( fp);
	
#if SHOW_STEP
	fprintf( stdout, "\n");

	// inorder traversal
	fprintf( stdout, "Inorder traversal: ");
	AVL_Traverse( tree);
	fprintf( stdout, "\n");

	// print tree with right-to-left infix traversal
	fprintf( stdout, "Tree representation:\n");
	printTree(tree);
#endif
	fprintf( stdout, "Height of tree: %d\n", tree->root->height);
	fprintf( stdout, "# of nodes: %d\n", tree->count);
	
	// retrieval
	char *key;
	fprintf( stdout, "Query: ");
	while( fscanf( stdin, "%s", str) != EOF)
	{
		key = AVL_Retrieve( tree, str);
		
		if (key) fprintf( stdout, "%s found!\n", key);
		else fprintf( stdout, "%s NOT found!\n", str);
		
		fprintf( stdout, "Query: ");
	}
	
	// destroy tree
	AVL_Destroy( tree);

	return 0;
}

////////////////////////////////////////////////////////////
void AVL_Destroy(AVL_TREE* pTree) {
	if (pTree)
	{
		_destroy(pTree->root);
	}

	free(pTree);
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
			printf("%s\n", root->data);
			return;
		}
		_infix_print(root->right, level + 1);
		for (int i = 0; i < level; i++) {
			printf("\t");
		}
		printf("%s\n", root->data);
		_infix_print(root->left, level + 1);
	}
}

void printTree(AVL_TREE* pTree) {
	_infix_print(pTree->root, 0);
	return;
}

static void _traverse(NODE* root) {
	if (!root) {
		return;
	}
	else {
		if (root->left == NULL && root->right == NULL) {
			printf("%s ", root->data);
			return;
		}
		else {
			_traverse(root->left);
			printf("%s ", root->data);
			_traverse(root->right);
		}
	}
}

void AVL_Traverse(AVL_TREE* pTree) {
	_traverse(pTree->root);
	printf("\n");
	return;
}

static NODE* _insert(NODE* root, NODE* newPtr) {
	if (root == NULL) {
		newPtr->height = 1;
		return newPtr;
	}
	else {
		if (strcmp(root->data, newPtr->data) > 0) { // insert the node in the left subtree
			root->left = _insert(root->left, newPtr);
			root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
			if (BALANCING&&(getHeight(root->left) - getHeight(root->right) >= 2)){
				root = _leftBalance(root);
			}
		}
		else { // insert the node in the right subtree
			root->right = _insert(root->right, newPtr);
			root->height = max(getHeight(root->left), getHeight(root->right)) + 1;
			if (BALANCING&&(getHeight(root->right) - getHeight(root->left) >= 2)) {
				root = _rightBalance(root);
			}
		}
		return root;
	}
}

int AVL_Insert(AVL_TREE* pTree, char* data) {
	NODE* newNode = _makeNode(data);
	if (newNode) {
		pTree->root = _insert(pTree->root, newNode);
		pTree->count++;
		return 1;
	}
	return 0;
}

static NODE* _retrieve(NODE* root, char* key) {
	if (root == NULL) {
		return root;
	}
	else {
		if (strcmp(root->data, key) > 0) {
			return _retrieve(root->left, key);
		}
		else if(strcmp(root->data,key)==0){
			return root;
		}
		else {
			return _retrieve(root->right, key);
		}
	}
}

char* AVL_Retrieve(AVL_TREE* pTree, char* key) {
	NODE* foundNode = _retrieve(pTree->root, key);
	if (foundNode) {
		return foundNode->data;
	}
	else {
		return NULL;
	}
}
