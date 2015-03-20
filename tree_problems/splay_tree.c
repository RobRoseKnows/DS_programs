#include <stdio.h>
#include <stdlib.h> //for malloc

/* 
Definition
============= 
It is also a BST and self balancing tree like AVL and red-black trees but is used
in real-world applications where out of 100, 80 times the data which is queried
is 20% data in the database.

This tree implemenation not just balances the tree, to get insertion/deletion/search to complete
in O(log n) time, but also move recently accessed the data to the root. This way
frequent access to the same data will take O(1) time in next access.

Having said that, still the algorithm will result in O(log n) amortized time complexity.



Applications
=============
TODO :


Comparison between AVL, Red-Black and Splay trees
=================================================

AVL and red-black trees only balances tree when tree is changed(insertion or deletion)
On contrary, Splay trees balances tree in read-only operation (like search) also.

Splay tree does not have to keep extra book-keeping information in the data structure like AVL trees (height)
or red-black trees (color)


red-black trees are also balanced but less balanced than AVL trees.
Red-black trees are used in applications where insertion and deletion are frequent operations.

AVL trees are Used frequently for quick searching as searching takes O(Log n) because tree is balanced.

Due to insertion and deletion, tree need rebalancing. Hence if application involves
more insertion and deletion operations then AVL trees is not a best choice.
Use red-black trees instead for quick insertion and deletion.

*/

//Data structure for avl tree
typedef struct splay_node {
	int key;
	struct splay_node *left;	// left child
	struct splay_node *right;	// right  child
} splay_node;

splay_node *root = NULL; 			// denotes root of splay tree
#define NUM_OPS 4

#define DEBUG_ON 0

#if DEBUG_ON
#define debug_printf printf
#else
#define debug_printf do {} while(0);
#endif


int max(int a, int b)
{
	return (a > b) ? a : b;
}

/*
 * Rotate right
 * let x is newly added node. B(R) is right subtree of B
 * A can be a subtree. T is tree above A
 * 
 *		 T
 *		/
 *            A
 *	     /
 *	    B
 *	   / \
 *	  X   B(R)
 *
 */
splay_node *apply_LL(splay_node *A)
{
	splay_node *B;

	debug_printf("%s : ENTER \n", __FUNCTION__);
	B = A->left;
	A->left = B->right;
	B->right = A;

	//set new root of subtree
	return B;
}

/*
 * Rotate Left
 * let x is newly added node. B(L) is left subtree of B
 * A can be a subtree. T is tree above A
 * 
 *		 T
 *		  \
 *            	   A
 *	     	    \
 *	    	     B
 *	   	    / \
 *	          B(L) X
 *
 */
splay_node *apply_RR(splay_node *A)
{
	splay_node *B;
	debug_printf("%s : ENTER \n", __FUNCTION__);

	B = A->right;
	A->right = B->left;
	B->left = A;

	//set new root of subtree
	return B;
}

/*
 * Rotate Left and then rotate right
 * let x is newly added node. B(L) is left subtree of B
 * A can be a subtree. T is tree above A
 * 
 *		 T
 *		/
 *            A
 *	     /
 *	    B
 *	   / \
 *	B(L) X 
 * 
 * Rotate left around B first then, rotate right around A
 *
 */
splay_node *apply_LR(splay_node *A)
{
	debug_printf("%s : ENTER \n", __FUNCTION__);

	A->left = apply_RR(A->left);
	
	return apply_LL(A);

}

/*
 * Rotate Right and then rotate left
 * let x is newly added node. B(R) is right subtree of B
 * A can be a subtree. T is tree above A
 * 
 *		 T
 *		  \
 *            	   A
 *	     	    \
 *	    	     B
 *	   	    / \
 *	           X   B(R)
 * 
 * Rotate right around B first then, rotate left around A
 *
 */
splay_node *apply_RL(splay_node *A)
{
	debug_printf("%s : ENTER \n", __FUNCTION__);
	A->right = apply_LL(A->right);
	
	return apply_RR(A);
}


/*
 * Search Algorithm :
 * Does search as in BST. If find the key, splays(percolate up) the key node to root.
 * Else splays last accessed node during search to become new root
 *
 * If key = root, return root
 * If found key is child of root. rotate right or left depending on case
 * If found key is L child of L parent (LL)
 * If found key is L child of R parent (RL)
 * If found key is R child of L parent (LR)
 * If found key is R child of R parent (RR)
 */
splay_node *splay_adjust(int key, splay_node *root)
{
	debug_printf("%s : ENTER \n", __FUNCTION__);
	if (root == NULL || root->key == key)
		return root;
	
	//key lies in left subtree
	if (key < root->key) {
		debug_printf("%s: key = %d, root is %d\n", __FUNCTION__, key, root->key);
		
		if (root->left == NULL) return root;
	
		//LL case
		if (key < root->left->key) {
			debug_printf("%s: In LL : root left is %d\n", __FUNCTION__, root->left->key);
			
			root->left->left = splay_adjust(key, root->left->left);
			debug_printf("%s: In LL : root left left is %d\n", __FUNCTION__, root->left->left->key);

			//now rotate right
			root = apply_LL(root);
			debug_printf("%s: In LL :After 1st R rotation root=%d\n", __FUNCTION__, root->key);
			

			// if root has left subtree, rotate right again, as this is LL(2 right rotations)
			if (root->left)
				root = apply_LL(root);
		} else { //LR case
			root->left->right = splay_adjust(key, root->left->right);
			
			//rotate left first and then rotate right
			root = apply_LR(root);
		}
	} else {	//key lies in right subtree
		if (root->right == NULL) return root;
		
		//RR case
		if (key > root->right->key) {
			debug_printf("%s: In RR : root right is %d\n", __FUNCTION__, root->right->key);

			root->right->right = splay_adjust(key, root->right->right);
			debug_printf("%s: In RR : root right right is %d\n", __FUNCTION__, root->right->right->key);

			//now rotate left
			root = apply_RR(root);
			debug_printf("%s: In RR :After 1st L rotation root=%d\n", __FUNCTION__, root->key);

			// if root has right subtree, rotate left again, as this is RR(2 left rotations)
			if (root->right)
				root = apply_RR(root);
		} else { //RL case
			root->right->left = splay_adjust(key, root->right->left);
		
			//rotate right first and then rotate left
			root = apply_RL(root);
		}
	}
	
	return root;	
}

void generate_node(int key, splay_node **node)
{
	debug_printf("%s : ENTER \n", __FUNCTION__);
	*node = malloc(sizeof(splay_node));

	(*node)->key = key;				//NOTE : () around node is necessary
	(*node)->left = NULL;
	(*node)->right = NULL;
}

/*
 * Insert Algorithm :
 * 1. If root is NULL : allocate a new node and add to tree
 * 2. Else splay(Call splay adjust) the given key to root.
 *    If key is already present, it will become a new root. Else
 *    last accessed leaf node will become a new root
 * 3. Now check if root->key is equal to key to be inserted.
 *    If yes, dont do anything. Dont insert. Just return root
 * 4. If no, allocate a new node and compare root's key with new key
 *    If new key is greater than root's key, make new_node->left = root and new_node->right = root->right
 *    If new key is smaller than root's key, make new_node->right = root and new_node->left = root->left
 * 5. Make new_node as root and return root
 */
//splay_node *insert_splay(int key, splay_node **root)
void insert_splay(int key, splay_node **root)
{
 	splay_node *node;
	debug_printf("%s : ENTER \n", __FUNCTION__);
	if (*root == NULL) {
		generate_node(key, &(*root));
		//return root;
		return;
	}
	debug_printf("%s: root is %d\n", __FUNCTION__, (*root)->key);

	(*root) = splay_adjust(key, *root);
	debug_printf("%s: returned root is %d\n", __FUNCTION__, (*root)->key);

	if ((*root)->key == key) {
		debug_printf("%s: root key is equal to %d\n", __FUNCTION__, (*root)->key);
		//return root;
		return;
	}
	//else allocate a new node and insert it
	generate_node(key, &node);
	if (key < (*root)->key) {
		node->right = *root;
		node->left = (*root)->left;
		(*root)->left = NULL;
	} else if(key > node->key) {
		node->left = *root;
		node->right = (*root)->right;
		(*root)->right = NULL;
	}
	
	*root = node;
	//return root;
	return;
}

void search(int key, splay_node **node)
{
	//splay_node *found_node;
	
	//use node here and not any local
	//variable(like *found_node) bcoz splay_adjust()
	//returns new root
	debug_printf("%s : ENTER \n", __FUNCTION__);
	(*node) = splay_adjust(key, *node);

	if ((*node)->key == key)
		printf("KEY FOUND\n");
	else
		printf("KEY not found\n");

	debug_printf("%s : After search new root is : %d\n", __FUNCTION__, (*node)->key);
}

//preorder
void preorder_splay(splay_node *node)
{
	if (node != NULL) {
		printf("%d ", node->key);
		preorder_splay(node->left);
		preorder_splay(node->right);
	}
}

void inorder_splay(splay_node *node)
{
	if (node != NULL) {
		inorder_splay(node->left);
		printf("%d ", node->key);
		inorder_splay(node->right);
	}
}

void perform_action(int op)
{
	int x;
	switch (op) {
		case 1: 										//insert a node
			printf("Enter element :");
            		scanf("%d", &x);
            		insert_splay(x, &root);
			debug_printf("ROOT = %d\n", root->key);
			break;
		case 2: 										//TODO: delete a node
			break;

		case 3 :
			printf("Enter element :");
            		scanf("%d", &x);
			
			search(x, &root);
			break;
		case 4: 										//print avl tree
			inorder_splay(root);
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
        printf("1. Insert an element into SPLAY tree\n");
        printf("2. Delete an element from SPLAY tree\n");
        printf("3. Search an element from SPLAY tree\n");
	
        printf("4. Print SPLAY tree (Inorder)\n");

        printf("Enter option :");
        scanf("%d", &op);

        perform_action(op);
    } while(op > 0 && op <= NUM_OPS);

    return 0;
}

