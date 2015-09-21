/*
 * Boundary traversal of tree
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


void print_left_boundary(bst_node *node)
{
        if (node == NULL) return;

        if (node->left) {
                printf("%d ", node->key);               //TOP-BOTTOM order
                print_left_boundary(node->left);
        } else if (node->right) {               //when there is only right nodes in left subtree
                printf("%d ", node->key);
                print_left_boundary(node->right);
        }
}


void print_right_boundary(bst_node *node)
{
        if (node == NULL) return;
        
        if (node->right) {
                print_right_boundary(node->right);      //BOTTOM-TOP order
                printf("%d ", node->key);
        } else if (node->left) {                //when there is only left nodes in right subtree
                print_right_boundary(node->left);
                printf("%d ", node->key);
        }
}


void print_leaves(bst_node *node)
{
        if (node == NULL) return;

        //inorder
        print_leaves(node->left);

        if (node->left == NULL && node->right == NULL)
                printf("%d ", node->key);

        print_leaves(node->right); 
}


void boundary_traversal(bst_node *root)
{
        if (root == NULL) return;

        printf("%d ", root->key);
        print_left_boundary(root->left);
        print_leaves(root->left);
        print_leaves(root->right);
        print_right_boundary(root->right);
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
                        boundary_traversal(root);
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
        printf("2. level wise printing of the tree\n");
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
 * 2. level wise printing of the tree
 * 3. print tree (preorder)
 * Enter option :3
 * 10 9 5 7 6 11 20 19
 * Operations :
 * 1. Insert an element into BST
 * 2. level wise printing of the tree
 * 3. print tree (preorder)
 * Enter option :2
 * 10 9 5 7 6 19 20 11
 * Operations :
 * 1. Insert an element into BST
 * 2. level wise printing of the tree
 * 3. print tree (preorder)
 *
 */
