/* Print mirror image of tree */

#include <stdio.h>
#include <stdlib.h>             //for malloc
#include <string.h>


#define NUM_OPS 3

typedef struct BST_node {
        int key;
        struct BST_node *left, *right;
} bst_node;

bst_node *root; //root of BST
bst_node *temp_node;

void print_preorder_BST(bst_node *node)
{
        if (node != NULL) {
                printf("%d ", node->key);
                print_preorder_BST(node->left);
                print_preorder_BST(node->right);
        }
}

void in_place_mirror(bst_node *root)
{
        if (root ==  NULL) return;

        in_place_mirror(root->left);
        in_place_mirror(root->right);

        //swap pointers
        temp_node = root->left;
        root->left = root->right;
        root->right = temp_node;
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
        int h;
        switch (op) {
                case 1:                                         //insert a node
                        printf("Enter element :");
                        scanf("%d", &x);
                        root = insert(x, root);
                        printf("root = %d\n", root->key);
                        break;
                case 2:
                        in_place_mirror(root);
                        print_preorder_BST(root);
                        printf("\n");
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
        printf("2. Print mirror image of the tree\n");
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
 * 2. Print mirror image of the tree
 * 3. print tree (preorder)
 * Enter option :3
 * 10 5 3 7 20 15 30
 * Operations :
 * 1. Insert an element into BST
 * 2. Print mirror image of the tree
 * 3. print tree (preorder)
 * Enter option :2
 * 10 20 30 15 5 7 3
 *
 */
