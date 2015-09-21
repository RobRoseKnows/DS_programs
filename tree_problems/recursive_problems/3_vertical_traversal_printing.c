/* DFS traversal of tree */

#include <stdio.h>
#include <stdlib.h>             //for malloc
#include <string.h>


#define NUM_OPS 3

typedef struct BST_node {
        int key;
        struct BST_node *left, *right;
} bst_node;

bst_node *root; //root of BST

void find_min_max_width(bst_node *root,
                        int *minwitdth,
                        int *maxwidth,
                        int curr_level)         //its a vertical level
{
        if (curr_level < *minwitdth)
                *minwitdth = curr_level;

        if (curr_level > *maxwidth)
                *maxwidth = curr_level;

        if (root->left)
                find_min_max_width(root->left, minwitdth,
                                   maxwidth, curr_level - 1);
        
        if (root->right)
                find_min_max_width(root->right, minwitdth,
                                   maxwidth, curr_level + 1);
 
}

void print_level(bst_node *root, int level, int curr_level)
{
        if (root == NULL) return;       //either check this at start of func
                                        // or check left and right pointers
                                        // before calling recursive call

        if (level == curr_level) {
                printf("%d ", root->key);
        }

        print_level(root->left, level, curr_level - 1);
        print_level(root->right, level, curr_level + 1);
}

void print_vertical_traversal(bst_node *root)
{
        int minwitdth = 0, maxwidth = 0;
        int i;
        find_min_max_width(root, &minwitdth, &maxwidth, 0);
        
        printf("minwitdth = %d, maxwidth = %d\n", minwitdth, maxwidth);
        
        for (i = minwitdth; i <= maxwidth; i++) {
                print_level(root, i, 0);
                printf("\n");
        } 
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
                        print_vertical_traversal(root); 
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
minwitdth = -2, maxwidth = 2
5 6
9 7
10
11 19
20

*/
