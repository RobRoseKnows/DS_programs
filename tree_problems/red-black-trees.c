#include <stdio.h>
#include <stdlib.h>


/*
Like AVL trees, Red black trees are also balanced  BST trees but less balanced than
AVL trees. They are used for applications where insertion and deletion operations
are frequent.

Insertion and deletion are faster in red-black trees bcoz adjustments to make tree balanced
and to persist properties are comparitively less than what in AVL trees.

Like AVL trees, red-black trees also take O(log n) to search, insert, or delete.
*/

/* properties of red-black tree
 * ============================
 * 1. Root should always be black
 * 2. adjacent nodes cannot be both red (parent and child)
 * 3. every path from root to NULL nodes has same number of balck nodes
 * 4. NULL nodes are considered as black
 */



//data structure for red-black trees
typedef struct red_black_node {
	int key;
	char color;

	struct red_black_node *parent;
	struct red_black_node *left;
	struct red_black_node *right;
} red_black_node;

red_black_node *root = NULL;	//root of red-black tree

void swap_color(red_black_node *x, red_black_node *y)
{
	char ch = x->color;
	x->color = y->color;
	y->color = ch;
}


//as we have to adjust parent pointers as well, 
//this function is different from AVL LL implementation
void apply_LL(red_black_node **root, red_black_node *A)
{
	red_black_node *B = A->left;
	printf("In %s\n", __FUNCTION__);

	A->left = B->right;
	if (B->right != NULL)
		B->right->parent = A;

	// <start> this is important. Have to take care of A's parent as well
	B->parent = A->parent;
	if (A->parent == NULL)
		(*root) = B;
	else if (A->parent->left == A)
		A->parent->left = B;
	else
		A->parent->right = B;
	// <end>

    B->right = A;	
	A->parent = B;
}

void apply_RR(red_black_node **root, red_black_node *A)
{
	red_black_node *B = A->right;
	printf("In %s\n", __FUNCTION__);

	A->right = B->left;
	if (B->left != NULL)
		B->left->parent = A;

	//this is important
	B->parent = A->parent;
	if (A->parent == NULL)
		(*root) = B;
	else if (A->parent->left == A)
		A->parent->left = B;
	else
		A->parent->right = B;

    B->left = A;	
	A->parent = B;
}

void apply_LR(red_black_node **root, red_black_node *A)
{
	printf("In %s\n", __FUNCTION__);
	apply_RR(root, A->left);
	
	apply_LL(root, A);
}

void apply_RL(red_black_node **root, red_black_node *A)
{
	printf("In %s\n", __FUNCTION__);
	apply_LL(root, A->right);
	
	apply_RR(root, A);
}

void print_tree_preorder(red_black_node *root)
{
	if (root != NULL) {
		printf("(%d, %c), ", root->key, root->color);
		print_tree_preorder(root->left);
		print_tree_preorder(root->right);
	}
}


/*
 * Function which does actual rebalancing after insertion
 *
 * function checks all the following conditions
 * 1. if parent sibling(uncle) is red, do recoloring
 * 2. if uncle is black, do recoloring and/or re-balancing
 */
void rebalance_tree(red_black_node **root, red_black_node *new_node)
{
	red_black_node *uncle;

	while (new_node != (*root) && new_node->parent->color == 'R') {	//NOTE: new_node != root should be checked first always as
																	// when new_node is changed to parent of parent. It may be
																	// possible that new new_node's parent is NUll as new_node
																	// is now root

		//get reference to uncle node
		//
		//This condn will determine whether new node is in left subree or right, bcoz
		//uncle will in other subtree
		if (new_node->parent->key <  new_node->parent->parent->key)
			uncle = new_node->parent->parent->right;
		else
			uncle = new_node->parent->parent->left;
			

		//change parent and uncle as black and grand-parent as red
		if (uncle != NULL && uncle->color == 'R') {

			//recoloring
			new_node->parent->color = 'B';
			uncle->color = 'B';
			new_node->parent->parent->color = 'R';

			new_node = new_node->parent->parent;
		} else { //if uncle color is black, even NULL is considered as black

			//recoloring and rebalancing

			//LL rotation case
			if (new_node->parent->parent->left == new_node->parent &&
				new_node == new_node->parent->left) {
				
				//swap colors of parent and grand-parent
				swap_color(new_node->parent, new_node->parent->parent);

				apply_LL(root, new_node->parent->parent);
				goto end;			//This is important, else it may try to get into next condn and may get segfault, if new_node's parent is NULL
			}

			//LR rotation case
			if (new_node->parent->parent->left == new_node->parent &&
				new_node == new_node->parent->right) {

				//swap colors of new_node and grand-parent
				swap_color(new_node, new_node->parent->parent);

				apply_LR(root, new_node->parent->parent);
				goto end;

			}
			
			//RR rotation case
			if (new_node->parent->parent->right == new_node->parent &&
				new_node == new_node->parent->right) {

				//swap colors of parent and grand-parent
				swap_color(new_node->parent, new_node->parent->parent);

				apply_RR(root, new_node->parent->parent);
				goto end;

			}

			//RL rotation case
			if (new_node->parent->parent->right == new_node->parent &&
				new_node == new_node->parent->left) {

				//swap colors of new_node  and grand-parent
				swap_color(new_node, new_node->parent->parent);

				apply_RL(root, new_node->parent->parent);
				goto end;

			}

		} // end else
end: ;
	} //end of while

	(*root)->color = 'B'; //root is always black
}


void insert(red_black_node **root, int key)
{
	red_black_node *new_node;
	red_black_node *temp_node2;
	red_black_node *temp_parent;

	new_node = malloc(sizeof(*new_node));

	new_node->key = key;
	new_node->parent = new_node->left = new_node->right = NULL;

	if ((*root) == NULL) {
		new_node->color = 'B';
		(*root) = new_node;
	} else {
		//insert as in BST
		
		temp_node2 = (*root);

		while (temp_node2 != NULL) {

			temp_parent = temp_node2;		//save current as parent as current will soon be changed

			if (key < temp_node2->key)
				temp_node2 = temp_node2->left;
			else
				temp_node2 = temp_node2->right;
		}

		//set new node's parent
		new_node->parent = temp_parent;

		//connect the new node to the parent
		if (key < temp_parent->key)
				temp_parent->left = new_node;
		else
				temp_parent->right = new_node;
		
		//new node inserted will always be RED
		new_node->color = 'R';
		
		//now the most important step; rebalance the tree, if it voilated any property
		rebalance_tree(root, new_node);
		
	} //end else
}

void perform_action(int op)
{
    int x;
    switch (op) {
        case 1:                                         //insert a node
            printf("Enter element :");
            scanf("%d", &x);
            insert(&root, x);
            printf("key = %d\n", root->key);
            break;
        case 2:                                         //TODO: delete a node
            break;

        case 3:                                         //print red-black tree
				print_tree_preorder(root);
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
    } while(op > 0 && op <= 3);

}

/*
 * Example data set (10, 5, 2, 1, 12, 6, 7, 11, 9)
 * Preorder sequence : (5, B), (2, B), (1, R), (10, R), (7, B), (6, R), (9, R), (12, B), (11, R)
 */

