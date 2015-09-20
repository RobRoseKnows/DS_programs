/*
 * Find diameter of tree
 *
 * It is longest distance between two leaves of a tree
 *
 * Algo :
 * It is max of :
 * 1. Diameter of left subtree
 * 2. Diameter of right subtree
 * 3. Height of left subtree + Height of right subtree
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

#if 0
int l_diameter, r_diameter;
int l_height, r_height;
#endif

int max(int x, int y)
{
        return ((x >= y) ? x : y);
}

int calc_height(bst_node *root)
{
        if (root == NULL)
                return 0;

        return 1 + max(calc_height(root->left), calc_height(root->right));
}

int find_max_of_left_right_diameter(bst_node *root)
{
        int l_diameter, r_diameter; //these 4 variables ought to be local.
                                    //If u declare them global, they will be
                                    //overwritten by the next recursive call
                                    //and algo will not work as expected
        int l_height, r_height;

        if (root == NULL) return 0;
        
        l_height = calc_height(root->left);
        r_height = calc_height(root->right);
        
        l_diameter = find_max_of_left_right_diameter(root->left);
        r_diameter = find_max_of_left_right_diameter(root->right);

        return max((l_height + r_height + 1), max(l_diameter, r_diameter));
}

#if 0
int find_max_of_left_right_diameter2(bst_node *root)
{
        if (root == NULL) return 0;
        
        l_diameter = find_max_of_left_right_diameter2(root->left);
        r_diameter = find_max_of_left_right_diameter2(root->right);

        return max(l_diameter, r_diameter);
}
#endif


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
        int h_left, h_right;
        int d, final_diameter;
        switch (op) {
                case 1:                                         //insert a node
                        printf("Enter element :");
                        scanf("%d", &x);
                        root = insert(x, root);
                        printf("key = %d\n", root->key);
                        break;
                case 2:
#if 0
                        h_left = calc_height(root->left); 
                        h_right = calc_height(root->right);
        
                        d = find_max_of_left_right_diameter2(root);
                        printf("d = %d\n", d);
                        final_diameter = max(h_left + h_right + 1, d);
                        printf("Final diameter = %d\n", final_diameter);
#endif
                        //l_diameter = 0; r_diameter = 0;
                        d = find_max_of_left_right_diameter(root);
                        printf("d = %d\n", d);
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
