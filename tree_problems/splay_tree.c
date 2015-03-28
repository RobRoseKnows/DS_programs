#include <stdio.h>
#include <stdlib.h> //for malloc(), rand()
#include <assert.h> //for assert()

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
splay_node *root1 = NULL, *root2 = NULL;			//for join operation
#define NUM_OPS 6

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
 *		    T
 *		   /
 *        A
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
 *			A
 *	     	 \
 *			 B
 *	   	    / \
 *	     B(L) X
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
 *		    T
 *		   /
 *        A
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
 *         A
 *	     	\
 *	    	 B
 *	   	    / \
 *	       X   B(R)
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
		if (key <= root->left->key) {   //NOTE : "<=". Was a bug
			debug_printf("%s: In LL : root left is %d\n", __FUNCTION__, root->left->key);
			
			root->left->left = splay_adjust(key, root->left->left);

			//now rotate right
			if (root->left->left != NULL) {
				debug_printf("%s: In LL : root left left is %d\n", __FUNCTION__, root->left->left->key);
				root = apply_LL(root);
			}
			
			debug_printf("%s: In LL :After 1st R rotation root=%d\n", __FUNCTION__, root->key);

			// if root has left subtree, rotate right again, as this is LL(2 right rotations)
			if (root->left) {
				root = apply_LL(root);
			}

			debug_printf("%s: In RR :After 2nd R rotation root=%d\n", __FUNCTION__, root->key);
		} else { //LR case
			root->left->right = splay_adjust(key, root->left->right);
			
			//rotate left first and then rotate right
			if (root->left->right != NULL)			//Was a bug : this chk is important, otherwise apply_RR will try to access NULL
				root = apply_LR(root);
		}
	} else {	//key lies in right subtree
		if (root->right == NULL) return root;
		
		//RR case
		if (key >= root->right->key) { 	//NOTE : ">=. was a bug
			debug_printf("%s: In RR : root right is %d\n", __FUNCTION__, root->right->key);

			root->right->right = splay_adjust(key, root->right->right);

			//now rotate left
			if (root->right->right != NULL) {
				debug_printf("%s: In RR : root right right is %d\n", __FUNCTION__, root->right->right->key);
				root = apply_RR(root);
			}

			debug_printf("%s: In RR :After 1st L rotation root=%d\n", __FUNCTION__, root->key);

			// if root has right subtree, rotate left again, as this is RR(2 left rotations)
			if (root->right)
				root = apply_RR(root);
			
			debug_printf("%s: In RR :After 2nd L rotation root=%d\n", __FUNCTION__, root->key);

		} else { //RL case
			root->right->left = splay_adjust(key, root->right->left);
		
			//rotate right first and then rotate left
			if (root->right->left != NULL)
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
	} else if(key > (*root)->key) {
		node->left = *root;
		node->right = (*root)->right;
		(*root)->right = NULL;
	}
	
	*root = node;
	//return root;
	return;
}

/**
 * Algorithm :
 * 1. search an element
 * 2. splay it to root, if found
 * 3. return new root
 **/ 
int search(int key, splay_node **node)
{
	int found = 0;
	//splay_node *found_node;
	
	//use node here and not any local
	//variable(like *found_node) bcoz splay_adjust()
	//returns new root
	debug_printf("%s : ENTER \n", __FUNCTION__);
	(*node) = splay_adjust(key, *node);

	if ((*node)->key == key) {
		printf("KEY FOUND\n");
		found = 1;
	} else
		printf("KEY not found\n");

	debug_printf("%s : After search new root is : %d\n", __FUNCTION__, (*node)->key);
	return found;
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

//return the right most element of BST
int find_max(splay_node *root, int max)
{
	splay_node *temp;
	temp = root;
	while (temp->right != NULL)
		temp = temp->right;

	max = temp->key;
	return max;
}


splay_node *join_util(splay_node **root1, splay_node **root2)
{
	int max_elem;

	//Find max in T1
	max_elem = find_max(*root1, 0);
	debug_printf("\nmax elem in T1 is : %d\n", max_elem);

	*root1 = splay_adjust(max_elem, *root1);

	if ((*root1)->right != NULL) {
		debug_printf("This is unexpected : root's right child = %d\n", (*root1)->right->key);
		assert(0);
	}
	
	(*root1)->right = *root2;

	return (*root1);
}

/**
 * Algorithm : Join trees T1 and T2 provided elements in
 * 			   T1 are smaller than elements in T2
 * 1. Find the max element in T1 and splay the element
 *    which means that this new root(max element of T1)
 *    has null right child
 * 2. Now attach T2 as right child of T1
 **/
void join_trees()
{
	int i;
	unsigned int val;
	//create 1st tree
	
	for (i = 0; i < 5; i++) {
		val = (rand() + i) % 10;
		debug_printf("%d\n", val);
		insert_splay(val, &root1);
	}
#if DEBUG_ON
	debug_printf("1st tree :\n");
	preorder_splay(root1);
	printf("\n");
#endif

	debug_printf("creating second tree\n");
	//create 2nd tree
	for (i = 10; i < 15; i++) {
		val = rand() % 20;
		if (val < 10)
			val += i;
		debug_printf("%d\n", val);
		insert_splay(val, &root2);
	}

#if DEBUG_ON
	debug_printf("2nd tree :\n");
	preorder_splay(root2);
	printf("\n");
#endif

	root1 = join_util(&root1, &root2);

	//print new tree
	preorder_splay(root1);
	printf("\n");

	//make root1 as new root for subsequent operations
	root = root1;	
}

/**
 * Algorithm : Given a tree and a key x.
 * 			   Need to split the tree into tree T1 and T2
 * 			   such that elements in T1 are <= x and
 * 			   T2 has elements > x
 *
 * 1. search key x in tree
 * 2. if key is not found, return without splitting tree
 * 3. if key is found, splay the key and split the tree
 *    with T1 containing all nodes of left subtree and root.
 *    And T2 conataining all nodes of right subtree
 **/
void split_tree(splay_node **root, int key)
{
	int found;
	splay_node *root1 = NULL, *root2 = NULL;

	found = search(key, root);

	if (!found) {
		printf("Key not found. Cannot split the tree \n");
		exit(0);
	}
	
	root1 = *root;
	root2 = (*root)->right;

	printf("1st tree :");
	preorder_splay(root1);
	printf("\n");

	printf("2nd tree :");
	preorder_splay(root2);
	printf("\n");
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
		case 4:		//JOIN op
			root1 = NULL;
			root2 = NULL;
			join_trees();
			break;
		case 5:		//Split op
			printf("Enter element :");
            scanf("%d", &x);
			split_tree(&root, x);
			break;
		case 6: 										//print splay tree
			preorder_splay(root);
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
        printf("2. Delete an element from SPLAY tree\n");		//TODO
        printf("3. Search an element from SPLAY tree\n");
        printf("4. Join two trees\n");
        printf("5. Split trees based on given key\n");
	
	
        printf("6. Print SPLAY tree (Inorder)\n");

        printf("Enter option :");
        scanf("%d", &op);

        perform_action(op);
    } while(op > 0 && op <= NUM_OPS);

    return 0;
}



/*
 * BUG :
9 7 7 3 8 15 12 17 22 
Operations : 
1. Insert an element into SPLAY tree
2. Delete an element from SPLAY tree
3. Search an element from SPLAY tree
4. Join two trees
5. Split trees based on given key
6. Print SPLAY tree (Inorder)
Enter option :5
Enter element :15
search : ENTER 
splay_adjust : ENTER 
splay_adjust: In RR : root right is 15
splay_adjust : ENTER 
splay_adjust: key = 15, root is 17
splay_adjust: In RR : root right right is 17
apply_RR : ENTER 
splay_adjust: In RR :After 1st L rotation root=15
apply_RR : ENTER 
splay_adjust: In RR :After 2nd L rotation root=17
KEY not found
search : After search new root is : 17
Key not found. Cannot split the tree 
*/
