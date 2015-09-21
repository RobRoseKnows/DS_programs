/* 
 * DFS traversal of tree with some modifications
 *
 * This problem is similar to printing tree in vertical order,
 * The only catch here is that we want to print only first element of
 * vertical level here and skip the rest.
 *
 * How to do that - while going through each vertical level, cache the level num,
 * if in future we get any element of the same level, just skip printing it.
 */

#include <stdio.h>
#include <stdlib.h>             //for malloc
#include <string.h>


#define NUM_OPS 3

typedef struct BST_node {
        int key;
        struct BST_node *left, *right;
} bst_node;

bst_node *root; //root of BST
int level_traversed[100] = {0}; //0th index of array is not considered
unsigned int level_index;       //stores return value of func is_level_already_traversed()
int minwidth = 0, maxwidth = 0;

/*
 * level can be positive or negative
 *
 * What this function does and how ?
 *
 * 0 1    2 3 4 5 6 . . .          <<-- indx of array, 0th indx is empty and not considered in algo
 * - -    - - - - - - - -
 *   0    0 0 0 0 0                <<-- val in array, its a flag, set when level is traversed
 * - -    - - - - - - - -  
 *   -2  -1 0 1 2 3                <<-- vertical level of tree(minwidth = -2, maxwidth = 3)
 *
 * \returns
 * indx_of_level - index of level in array
 * whether the level is alredy traversed or not
 */
int is_level_already_traversed(int level, unsigned int *indx_of_level)
{
        int pos_0_level;     //position of 0th level in array
        //indx_of_level;   //index of level in array
        pos_0_level = (-(minwidth) + 1);        //bcoz minwidth is always <= 0

        *indx_of_level = pos_0_level + level;
        
        return level_traversed[*indx_of_level];



}
void find_min_max_width(bst_node *root,
                        int *minwidth,
                        int *maxwidth,
                        int curr_level)         //its a vertical level
{
        if (curr_level < *minwidth)
                *minwidth = curr_level;

        if (curr_level > *maxwidth)
                *maxwidth = curr_level;

        if (root->left)
                find_min_max_width(root->left, minwidth,
                                   maxwidth, curr_level - 1);
        
        if (root->right)
                find_min_max_width(root->right, minwidth,
                                   maxwidth, curr_level + 1);
 
}

void print_level(bst_node *root, int level, int curr_level)
{
        if (root == NULL) return;       //either check this at start of func
                                        // or check left and right pointers
                                        // before calling recursive call

        if (level == curr_level) {
                if (!is_level_already_traversed(level, &level_index)) { //if level is not traversed
                        level_traversed[level_index] = 1;                
                        printf("%d ", root->key);
                }
        }

        print_level(root->left, level, curr_level - 1);
        print_level(root->right, level, curr_level + 1);
}

void print_top_view(bst_node *root)
{
        //int minwidth = 0, maxwidth = 0;
        int i;
        find_min_max_width(root, &minwidth, &maxwidth, 0);
        
        printf("minwidth = %d, maxwidth = %d\n", minwidth, maxwidth);
        
        for (i = minwidth; i <= maxwidth; i++) {
                print_level(root, i, 0);
        } 
        printf("\n");
}

void print_preorder_BST(bst_node *node)
{
        if (node != NULL) {
                printf("%d ", node->key);
                print_preorder_BST(node->left);
                print_preorder_BST(node->right);
        }
}


void generate_node(int key, bst_node **node)
{
        (*node) = malloc(sizeof(bst_node));
        (*node)->key = key;
        (*node)->left = (*node)->right = NULL;
}


bst_node *insert(int key, bst_node *node)
{
        if (node == NULL) {
                generate_node(key, &node);
        } else if (key < node->key) {
                node->left = insert(key, node->left);
        } else if (key > node->key) {
                node->right = insert(key, node->right);
        }

        return node;
}


void perform_action(int op)
{
        int x;
        switch (op) {
                case 1:                                         //insert a node
                        printf("Enter element :");
                        scanf("%d", &x);
                        root = insert(x, root);
                        printf("key = %d\n", root->key);
                        break;
                case 2:
                        print_top_view(root); 
                        break;
                case 3:
                        print_preorder_BST(root);
                        printf("\n");
                        break;
                default: ;
        }
}

int main()
{
    int op;
    do {
        printf("Operations : \n");
        printf("1. Insert an element into BST\n");
        printf("2. Vertical printing of the tree(DFS traversal)\n");
        printf("3. print tree (preorder)\n");

        printf("Enter option :");
        scanf("%d", &op);

        perform_action(op);
    } while(op > 0 && op <= NUM_OPS);
}

/*
 * Output
Operations :
1. Insert an element into BST
2. Vertical printing of the tree(DFS traversal)
3. print tree (preorder)
Enter option :3
10 9 5 7 6 11 20 19
Operations :
1. Insert an element into BST
2. Vertical printing of the tree(DFS traversal)
3. print tree (preorder)
Enter option :2
minwidth = -2, maxwidth = 2
5 9 10 11 20
Operations :
1. Insert an element into BST
2. Vertical printing of the tree(DFS traversal)
3. print tree (preorder)
Enter option :4

*/
