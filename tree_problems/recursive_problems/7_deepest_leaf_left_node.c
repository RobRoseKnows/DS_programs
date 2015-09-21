/* 
 * Find the deepest left leaf node in the tree 
 *
 * Algo : no need to calculate height and iterate through levels
 * Instead, just check if a node is leaf(i.e. left and right pointers
 * are null) and just inc level at each recusrive call for left nodes
 *
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


void find_deepest_left_leaf(bst_node *root, int curr_level,
                            int *maxlevel, bst_node **result,
                            int is_leaf)
{

        if (root == NULL) return;

        if (is_leaf && root->left == NULL && root->right == NULL &&
                                                curr_level > *maxlevel) {
                *maxlevel = curr_level;
                *result = root;
                return;                 //why return
        }

        find_deepest_left_leaf(root->left, curr_level + 1, maxlevel, result, 1); //for left nodes, is_leaf=1
        find_deepest_left_leaf(root->right, curr_level + 1, maxlevel, result, 0); //for right nodes, is_leaf=0

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
        //construct tree
        int x;
        int isleaf = 0;
        int maxlevel = 0;
        bst_node *deepest_node = NULL;
        switch (op) {
                case 1:                                         //insert a node
                        printf("Enter element :");
                        scanf("%d", &x);
                        root = insert(x, root);
                        printf("key = %d\n", root->key);
                        break;
                case 2:
                        find_deepest_left_leaf(root, 0, &maxlevel,
                                               &deepest_node, isleaf);
                        if (deepest_node == NULL)
                                printf("deepest left leaf cannot be found\n");

                        printf("Left Leaf node at level %d is %d\n",
                               maxlevel, deepest_node->key);
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
        printf("2. Find deepest left leaf node\n");
        printf("3. print tree (preorder)\n");

        printf("Enter option :");
        scanf("%d", &op);

        perform_action(op);
    } while(op > 0 && op <= NUM_OPS);
}

/*
 * Output
 *
 * Operations :
 * 1. Insert an element into BST
 * 2. Find deepest left leaf node
 * 3. print tree (preorder)
 * Enter option :3
 * 10 9 5 7 6 11 20 19
 * Operations :
 * 1. Insert an element into BST
 * 2. Find deepest left leaf node
 * 3. print tree (preorder)
 * Enter option :2
 * Left Leaf node at level 4 is 6   <<------- level is starting from 0th in program
 *
 */
