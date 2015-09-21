#include <stdio.h>
#include <stdlib.h>             //for malloc
#include <string.h>


#define NUM_OPS 3

typedef struct BST_node {
        int key;
        struct BST_node *left, *right;
} bst_node;

bst_node *root; //root of BST

int ldepth = 0, rdepth = 0;

//TODO : some problem with this function
#if 0
int calc_height(bst_node *root)
{
        if (root == NULL) {
                return 0;
        } else {
                ldepth = calc_height(root->left);
                rdepth = calc_height(root->right);
                
                if (ldepth > rdepth)
                        return ldepth + 1;
                else
                        return rdepth + 1;
        }
}

#endif

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
        switch (op) {
                case 1:                                         //insert a node
                        printf("Enter element :");
                        scanf("%d", &x);
                        root = insert(x, root);
                        printf("key = %d\n", root->key);
                        break;
                case 2:
                        printf("height of tree is %d\n", calc_height(root));
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
        printf("2. calculate height of tree\n");
        printf("3. print tree\n");

        printf("Enter option :");
        scanf("%d", &op);

        perform_action(op);
    } while(op > 0 && op <= NUM_OPS);
}
