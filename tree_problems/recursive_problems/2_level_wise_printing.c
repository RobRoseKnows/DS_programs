/* BFS traversal of tree */

#include <stdio.h>
#include <stdlib.h>             //for malloc
#include <string.h>


#define NUM_OPS 3

typedef struct BST_node {
        int key;
        struct BST_node *left, *right;
} bst_node;

bst_node *root; //root of BST

void print_level(bst_node *root, int level)
{
        if (level == 1) {
                printf("%d ", root->key);
        } else if (level > 1) {
                if (root->left)
                        print_level(root->left, level - 1);
                if (root->right)
                        print_level(root->right, level - 1);
        }
}

void level_wise_print(bst_node *root, int height)
{
        int i;
        for (i = 1; i <= height; i++) {
                print_level(root, i);
                printf("\n");
        }      
}

int max(int x, int y)
{
        return ((x > y) ? x : y);
}

int calc_height(bst_node *root)
{
        if (root == NULL)
                return 0;

        return 1 + max(calc_height(root->left), calc_height(root->right));
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
        int h;
        switch (op) {
                case 1:                                         //insert a node
                        printf("Enter element :");
                        scanf("%d", &x);
                        root = insert(x, root);
                        printf("key = %d\n", root->key);
                        break;
                case 2:
                        h = calc_height(root);         
                        printf("height of tree is %d\n", h);
                        level_wise_print(root, h);
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
        printf("2. level wise printing of the tree\n");
        printf("3. print tree\n");

        printf("Enter option :");
        scanf("%d", &op);

        perform_action(op);
    } while(op > 0 && op <= NUM_OPS);
}
