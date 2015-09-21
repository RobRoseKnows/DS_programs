/*
 * Check whether tree is foldable.
 *
 * Tree is said to be foldable if left subtree is
 * mirror image of right subtree
 *
 * Algo : create mirror image of left subtree and 
 * compare the _node_ _position_ of mirrored left subtree with
 * right subtree
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

int is_subtree_same(bst_node *x, bst_node *y)
{
        if (x == NULL && y == NULL)
                return 1;       //true
        
        if (x != NULL && y != NULL &&
                        is_subtree_same(x->left, y->left) &&
                        is_subtree_same(y->right, y->right))
                return 1;

        return 0;       //false
}

int is_tree_foldable(bst_node *root)
{
        int result;
        if (root == NULL) return;
        
        //mirror left subtree
        in_place_mirror(root->left);
        
        //compare it with right subtree
        result = is_subtree_same(root->left, root->right);

        //restore left subtree
        in_place_mirror(root->left);
        return result;
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
        int res;
        switch (op) {
                case 1:                                         //insert a node
                        printf("Enter element :");
                        scanf("%d", &x);
                        root = insert(x, root);
                        printf("root = %d\n", root->key);
                        break;
                case 2:
                        res = is_tree_foldable(root);
                        printf("is tree foldable : ");
                        res ? printf("yes\n") : printf("no\n");
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
        printf("2. Check if tree foldable\n");
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
 * 2. Check if tree foldable
 * 3. print tree (preorder)
 * Enter option :3
 * 10 5 7 20 15
 * Operations :
 * 1. Insert an element into BST
 * 2. Check if tree foldable
 * 3. print tree (preorder)
 * Enter option :2
 * is tree foldable : yes
 *
 *
 * Operations :
 * 1. Insert an element into BST
 * 2. Check if tree foldable
 * 3. print tree (preorder)
 * Enter option :3
 * 10 5 7 11 12
 * Operations :
 * 1. Insert an element into BST
 * 2. Check if tree foldable
 * 3. print tree (preorder)
 * Enter option :2
 * is tree foldable : no
 *
 */
