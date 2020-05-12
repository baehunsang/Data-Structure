#include <stdlib.h> // malloc, atoi
#include <stdio.h>
#include <ctype.h> // isdigit
#include <assert.h> // assert

#define MAX_STACK_SIZE	50

////////////////////////////////////////////////////////////////////////////////
// LIST type definition
typedef struct node
{
	char		data;
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
TREE *createTree(void) {
	TREE* newHead = (TREE*)malloc(sizeof(TREE));
	newHead->root = NULL;
	return newHead;
}

/* Deletes all data in tree and recycles memory
*/
void destroyTree( TREE *pTree);

static void _destroy(NODE* root) {
	if (root == NULL) {
		return;
	}
	if (root->left ==NULL && root->right ==NULL) { // if the argument is a leaf node.
		free(root);
		return;
	}
	else {
		_destroy(root->left);
		_destroy(root->right);
		free(root);
	}
	
}

/*  Allocates dynamic memory for a node and returns its address to caller
	return	node pointer
			NULL if overflow
*/
static NODE *_makeNode(char ch) {
	NODE* newNode = (NODE*)malloc(sizeof(NODE));
	newNode->data = ch;
	newNode->left = NULL;
	newNode->right = NULL;
	return newNode;
}

/* converts postfix expression to binary tree
	return	1 success
			0 invalid postfix expression
*/
int postfix2tree( char *expr, TREE *pTree);

/* Print node in tree using inorder traversal
*/
void traverseTree( TREE *pTree);

/* internal traversal function
	an implementation of ALGORITHM 6-6
*/
static void _traverse(NODE* root) {
	if (root == NULL) {
		return;
	}
	else {
		if (isdigit(root->data)) {
			printf("%c", root->data);
		}
		else {
			printf("(");
			_traverse(root->left);
			printf("%c", root->data);
			_traverse(root->right);
			printf(")");
		}
	}
};

/* Print tree using inorder right-to-left traversal
*/
void printTree( TREE *pTree);

/* internal traversal function
*/
static void _infix_print(NODE* root, int level) {
	if (root == NULL) {
		return;
	}
	else {
		if (root->left == NULL && root->right == NULL) {
			for (int i = 0; i < level; i++) {
				printf("\t");
			}
			printf("%c\n", root->data);
			return;
		}
		_infix_print(root->right, level + 1);
		for (int i = 0; i < level; i++) {
			printf("\t");
		}
		printf("%c\n", root->data);
		_infix_print(root->left, level + 1);
	}
}

/* evaluate postfix expression
	return	value of expression
*/
float evalPostfix( char *expr);

////////////////////////////////////////////////////////////////////////////////
void destroyTree( TREE *pTree)
{
	if (pTree)
	{
		_destroy( pTree->root);
	}
		
	free( pTree);
}

////////////////////////////////////////////////////////////////////////////////
void printTree( TREE *pTree)
{
	_infix_print(pTree->root, 0);
	
	return;
}

////////////////////////////////////////////////////////////////////////////////
void traverseTree( TREE *pTree)
{
	_traverse(pTree->root);
	
	return;
}

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	TREE *tree;
	char expr[1024];
	
	fprintf( stdout, "\nInput an expression (postfix): ");
	
	while (fscanf( stdin, "%s", expr) == 1)
	{
		// creates a null tree
		tree = createTree();
		
		if (!tree)
		{
			printf( "Cannot create tree\n");
			return 100;
		}
		
		//postfix expression -> expression tree
		int ret = postfix2tree( expr, tree);
		if (!ret)
		{
			fprintf( stdout, "invalid expression!\n");
			destroyTree( tree);
			continue;
		}
		
		// expression tree -> infix expression
		fprintf( stdout, "\nInfix expression : ");
		traverseTree( tree);
		
		// print tree with right-to-left infix traversal
		fprintf( stdout, "\n\nTree representation:\n");
		printTree(tree);
		
		// evaluate postfix expression
		float val = evalPostfix( expr);
		fprintf( stdout, "\nValue = %f\n", val);
		
		// destroy tree
		destroyTree( tree);
		
		fprintf( stdout, "\nInput an expression (postfix): ");
	}
	return 0;
}

/* converts postfix expression to binary tree
	return	1 success
			0 invalid postfix expression
*/
int postfix2tree(char* expr, TREE* pTree){
	NODE* NodeStack[MAX_STACK_SIZE] = {NULL, };
	int NSP = -1; // Work as a stack pointer. Store the index of the stack.
		for (int i = 0; expr[i] != '\0'; i++) {
			if (isdigit(expr[i])) {
				NODE* newNode =_makeNode(expr[i]);
				NodeStack[++NSP] = newNode; // push newNode
			}
			else {
				if (NSP < 0) {
					return 0; // The srack is empty.(Underflow)
				}
				else {
					NODE* operand1 = NodeStack[NSP--];//pop operand1
					if (NSP < 0) {
						_destroy(operand1);
						return 0; // Underflow at the stack.(Only one node)
					}
					NODE* operand2 = NodeStack[NSP--];// pop operand2
					NODE* newNode = _makeNode(expr[i]);
					newNode->right = operand1;
					newNode->left = operand2;
					NodeStack[++NSP] = newNode; // push newNode
				}
			}
		}
		if (NSP != 0){
			for (int i = 0; NodeStack[i] != NULL; i++) {
				_destroy(NodeStack[i]);
			}
			return 0;
		}
		else {
			pTree->root = NodeStack[NSP--]; 
			return 1;
		}

}

float evalPostfix(char* expr) {
	float exprStack[MAX_STACK_SIZE] = { 0, };
	int SP = -1;
	for (int i = 0; expr[i] != '\0'; i++) {
		if (isdigit(expr[i])) {
			char* operand = (char*)malloc(2*sizeof(char));
			operand[0] = expr[i];
			operand[1] = '\0';
			exprStack[++SP] = atoi(operand);
			free(operand);
		}
		else {
			float operand1 = exprStack[SP--];
			float operand2 = exprStack[SP--];
			char operator = expr[i];
			switch (operator) {
				case '+':
					exprStack[++SP] = operand2 + operand1;
					break;
				case '-':
					exprStack[++SP] = operand2 - operand1;
					break;
				case '*':
					exprStack[++SP] = operand2 * operand1;
					break;
				case '/':
					exprStack[++SP] = operand2 / operand1;
					break;
			}
		}
	}
	return exprStack[SP];
}
