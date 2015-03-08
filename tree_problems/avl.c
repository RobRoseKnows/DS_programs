#include <stdio.h>
#include <stdlib.h> //for malloc

/* 
Definition : AVL tree is a BST in which height of left and right subtrees 
differ atmost by 1; and left and right subtrees should also be AVL trees.

Height of node in AVL = |H(L) - H(R)| <= 1

Note : Height of each node in tree can be +1, 0, -1. Else tree has to be 
balanced using LL, LR, RR, RL rotations.

Applications : 
Used frequently for quick searching as searching takes O(Log n) because tree is balanced.

Due to insertion and deletion, tree need rebalancing. Hence if application involves
more insertion and deletion operations then AVL trees is not a best choice.
Use red-black trees instead for quick insertion and deletion.

FYI: red-black trees are also balanced but less balanced than AVL trees.
Red-black trees are used in applications where insertion and deletion are frequent operations.

And AVL trees are used for quicj=k searching operations by applications.
Some usecases of AVL trees :
1. Used in Linux kernel to manage memory regions(vm_struct_areas) of processes. Searching the memory region faster while swapping in and swapping out.
2. count number of smaller elements on right side of an element in a array
3. median of a stream of integers
4. max of all subarrays of size k in an array

*/

//Data structure for avl tree
typedef struct avl_node {
	int key;
	int height;				// height of the node
	struct avl_node *left;	// left child
	struct avl_node *right;	// right  child
} avl_node;

avl_node *root = NULL; 			// denotes root of avl tree
#define NUM_OPS 3


//return height of node
int height(avl_node *node)
{
	if (node == NULL)
		return 0;

	return node->height;
}

int max(int a, int b)
{
	return (a > b) ? a : b;
}

avl_node *apply_LL(avl_node *A)
{
	avl_node *B;

	printf("In %s\n", __FUNCTION__);
	B = A->left;
	A->left = B->right;
	B->right = A;

	//not forget to adjust heights
	A->height = max(height(A->left), height(A->right)) + 1;
	B->height = max(height(B->left), height(B->right)) + 1;

	//set new root of subtree
	return B;
}

avl_node *apply_RR(avl_node *A)
{
	avl_node *B;
	printf("In %s\n", __FUNCTION__);

	B = A->right;
	A->right = B->left;
	B->left = A;

	//not forget to adjust heights
	A->height = max(height(A->left), height(A->right)) + 1;
	B->height = max(height(B->left), height(B->right)) + 1;

	//set new root of subtree
	return B;
}

avl_node *apply_LR(avl_node *A)
{
	printf("In %s\n", __FUNCTION__);

	A->left = apply_RR(A->left);
	
	return apply_LL(A);

}

avl_node *apply_RL(avl_node *A)
{
	printf("In %s\n", __FUNCTION__);
	A->right = apply_LL(A->right);
	
	return apply_RR(A);
}

//just for debugging
void print_heights(avl_node *node)
{
	printf("key = %d, h(node) = %d, h(node->left) = %d, h(node->right) = %d \n\n",
			node->key, height(node), height(node->left), height(node->right));
}

void generate_node(int key, avl_node **node)
{
	*node = malloc(sizeof(avl_node));

	(*node)->key = key;				//NOTE : () around node is necessary
	(*node)->height = 1;
	(*node)->left = NULL;
	(*node)->right = NULL;
}

//recursive insert
avl_node *insert_AVL(int key, avl_node *node)
{
	if (node == NULL) {
		generate_node(key, &node);
		printf("%d\n", node->key);
	} else if (key < node->key) {		//insert node in left subtree
		node->left = insert_AVL(key, node->left);

		//after adding node to left, update height of its ancestor
		node->height = max(height(node->left), height(node->right)) + 1;
		print_heights(node);
  
		//now check if after insertion, the subtree gets imbalanced
		if ((height(node->left) - height(node->right)) > 1) {
			printf("inserted in left\n");		
			//now check whether node is inserted in left of left OR right of left
			if(key < node->left->key) {
				return apply_LL(node);
			} else {
				return apply_LR(node);
			}
		}

	} else if(key > node->key) {		//insert node in right subtree
		node->right = insert_AVL(key, node->right);

		//after adding node to left, update height of its ancestor
		node->height = max(height(node->left), height(node->right)) + 1;

		print_heights(node);

		//now check if after insertion, the subtree gets imbalanced
		if ((height(node->left) - height(node->right)) < -1) {
			printf("inserted in right\n");		
		
			//now check whether node is inserted in right of right OR left of right
			if(key > node->right->key) {
				return apply_RR(node);
			} else {
				return apply_RL(node);
			}
		}
	}

	return node;
}

//preorder
//TODO: levelwise
void print_AVL(avl_node *node)
{
	if (node != NULL) {
		printf("%d ", node->key);
		print_AVL(node->left);
		print_AVL(node->right);
	}
}


void perform_action(int op)
{
	int x;
	switch (op) {
		case 1: 										//insert a node
			printf("Enter element :");
            scanf("%d", &x);
            root = insert_AVL(x, root);
			printf("key = %d\n", root->key);
			break;
		case 2: 										//TODO: delete a node
			break;

		case 3: 										//print avl tree
			print_AVL(root);
			printf("\n");
			break;

		default :
			; //nothing
	}
}


int main()
{
	    int op;

    do {
        printf("Operations : \n");
        printf("1. Insert an element into AVL\n");
        printf("2. Delete an element from AVL\n");
        printf("3. Print AVL tree\n");

        printf("Enter option :");
        scanf("%d", &op);

        perform_action(op);
    } while(op > 0 && op <= NUM_OPS);

    return 0;
}

